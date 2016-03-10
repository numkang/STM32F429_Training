#include "Filter.h"

struct vect_t EstV, pEstV;
float sqX_sqZ, sqY_sqZ;

void Timer5_Initialization(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM5 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel =  TIM5_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  /* -- Timer Configuration --------------------------------------------------- */
  TIM_DeInit(TIM5);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_TimeBaseStruct.TIM_Period = 250;
  TIM_TimeBaseStruct.TIM_Prescaler = 900 - 1; // Prescaled by 900 -> = 0.1M(10us)
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1; // Div by one -> 90 MHz (Now RCC_DCKCFGR_TIMPRE is configured to divide clock by two)
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStruct);
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);    // Set interrupt when timer reloads (overflow)
  TIM_ARRPreloadConfig(TIM5, DISABLE);       //Put ARR value into register
  TIM_Cmd(TIM5, ENABLE);
}

void TIM5_IRQHandler()
{
  if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET){
  	f.filter = 1;

    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
  }
}

void Filter(int16_t gRaw[3], float *gAng, int16_t aRaw[3], float *aAng, float *Ang)
{
  uint8_t i = 0;
  float gBuff[3], aBuff[3];
  for(i = 0; i < 3; i++){
  	//gAng[i] += (float)gRaw[i]*2000.0/32768.0*0.001;
    gBuff[i] = (float)gRaw[i]*2000.0/32768.0/(961.5f*2.0); //314
  }
  // gAng[0] = gBuff[1]*( 1.0)*3.141592654/180.0; // X
  // gAng[1] = gBuff[0]*( 1.0)*3.141592654/180.0; // Y
  // gAng[2] = gBuff[2]*(-1.0)*3.141592654/180.0; // Z

  gAng[0] = gBuff[1]*( 1.0)*3.141592654/180.0; // X
  gAng[1] = gBuff[2]*(-1.0)*3.141592654/180.0; // Y
  gAng[2] = gBuff[0]*( 1.0)*3.141592654/180.0; // Z

  struct vect_t EstV_temp = EstV;

  /* Euler Rotational Matrix
      |  c  -s   0   |
  Z = |  s   c   0   |
      |  0   0   1   |

      |  c   0   s   |
  Y = |  0   1   0   |
      | -s   0   c   |

      |  1   0   0   |
  X = |  0   c   -s  |
      |  0   s   c   |

      |  X  |
  V = |  Y  |
      |  Z  |

  */

  /*Z*V 
  X = Xcos - Ysin
  Y = Ycos + Xsin
  Z = Z*/

  EstV.G.X = EstV_temp.G.X - EstV_temp.G.Y*gAng[2];
  EstV.G.Y = EstV_temp.G.Y + EstV_temp.G.X*gAng[2];
  EstV_temp = EstV;

  /*Y*V 
  X = Xcos + Zsin
  Y = Y
  Z = Zcos - Xsin*/

  EstV.G.X = EstV_temp.G.X + EstV_temp.G.Z*gAng[1];
  EstV.G.Z = EstV_temp.G.Z - EstV_temp.G.X*gAng[1];
  EstV_temp = EstV;

  /*X*V 
  X = X
  X = Xcos - Zsin
  Z = Zcos + Xsin*/

  EstV.G.Y = EstV_temp.G.Y - EstV_temp.G.Z*gAng[0];
  EstV.G.Z = EstV_temp.G.Z + EstV_temp.G.Y*gAng[0];
  EstV_temp = EstV;

  // EstV.G.Y = EstV.G.Y - EstV.G.Z*gAng[2];
  // EstV.G.X = EstV.G.X + EstV.G.Y*gAng[2];

  // EstV.G.Y = EstV.G.Y + EstV.G.Z*gAng[1];
  // EstV.G.Z = EstV.G.Z - EstV.G.Y*gAng[1];

  // EstV.G.X = EstV.G.X - EstV.G.Z*gAng[0];
  // EstV.G.Z = EstV.G.Z + EstV.G.X*gAng[0];

  //Normalize ACC
  double accR = sqrt((double)(aRaw[0]*aRaw[0]) + (double)(aRaw[1]*aRaw[1]) + (double)(aRaw[2]*aRaw[2]));
  // EstV.A.X = aRaw[1]/accR*( 1.0); //roll
  // EstV.A.Y = aRaw[0]/accR*( 1.0); //pitch
  // EstV.A.Z = aRaw[2]/accR*( 1.0); //yaw

  EstV.A.X = aRaw[1]/accR*( 1.0); //roll
  EstV.A.Y = aRaw[2]/accR*( 1.0); //pitch
  EstV.A.Z = aRaw[0]/accR*( 1.0); //yaw


  //Low-Pass Filter
  EstV.G.X = LowPassAlpha*EstV.G.X + (1.0-LowPassAlpha)*pEstV.G.X;
  EstV.G.Y = LowPassAlpha*EstV.G.Y + (1.0-LowPassAlpha)*pEstV.G.Y;
  EstV.G.Z = LowPassAlpha*EstV.G.Z + (1.0-LowPassAlpha)*pEstV.G.Z;

  EstV.A.X = LowPassAlpha*pEstV.A.X + (1.0-LowPassAlpha)*EstV.A.X;
  EstV.A.Y = LowPassAlpha*pEstV.A.Y + (1.0-LowPassAlpha)*EstV.A.Y;
  EstV.A.Z = LowPassAlpha*pEstV.A.Z + (1.0-LowPassAlpha)*EstV.A.Z;

  //Complementary Filter
  EstV.GA.X = ComplementAlpha*EstV.G.X + (1.0-ComplementAlpha)*EstV.A.X;
  EstV.GA.Y = ComplementAlpha*EstV.G.Y + (1.0-ComplementAlpha)*EstV.A.Y;
  EstV.GA.Z = ComplementAlpha*EstV.G.Z + (1.0-ComplementAlpha)*EstV.A.Z;

  /*EstV.GA.X = LowPassAlpha*pEstV.GA.X + (1.0-LowPassAlpha)*EstV.GA.X;
  EstV.GA.Y = LowPassAlpha*pEstV.GA.Y + (1.0-LowPassAlpha)*EstV.GA.Y;
  EstV.GA.Z = LowPassAlpha*pEstV.GA.Z + (1.0-LowPassAlpha)*EstV.GA.Z;*/

  pEstV = EstV;

  sqX_sqZ = sqrt(EstV.GA.X*EstV.GA.X + EstV.GA.Z*EstV.GA.Z);
  sqY_sqZ = sqrt(EstV.GA.Y*EstV.GA.Y + EstV.GA.Z*EstV.GA.Z);

  Ang[ROLL] = atan2f(EstV.GA.Y, sqX_sqZ) * 180.0 / 3.141592654;
  Ang[PITCH] = atan2f(EstV.GA.X, sqY_sqZ) * 180.0 / 3.141592654;
}