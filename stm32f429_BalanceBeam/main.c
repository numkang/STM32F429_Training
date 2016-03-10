#include "main.h"

char lcd_text_main[100];
uint16_t receivedData = 0;

int16_t gyroRaw[3];
float gyroAngle[3];

int16_t accRaw[3];
float accAngle[3];

float Angle[3];

struct flag f;

static inline void Delay_1us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 13; nCnt != 0; nCnt--);
}

void RCC_Configuration(void)
{
      /* --------------------------- System Clocks Configuration -----------------*/
      /* GPIOx clock enable */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
      /* USARTx clock enable */
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
}

void PushButton_Initialization(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*-------------------------- GPIO Configuration for Push Button ----------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD ;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t PushButton_Read(void){

    return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
}

void LCD_Initialization(){
  lcd_init();
  lcd_drawBackground(20,60,250);
  lcd_drawBGPersimmon(20, 60, 250);
  LCD_SetColors(LCD_COLOR_WHITE-1,LCD_COLOR_BLACK);
  LCD_SetFont(&Font8x12); 
}

void IMU_Initialization(){
  // receivedData = readSPI4(0x75); //75 WHO_AM_I
  // sprintf(lcd_text_main,"My Name is : %d    \n", receivedData); terminalWrite(lcd_text_main);
  // Delay_1us(500000);

  writeSPI4(0x6B,0x80); //PWR_MGMT_1   
  Delay_1us(100000);
  writeSPI4(0x6B,0x03); //PWR_MGMT_1   
  //writeSPI4(0x1A,0x02); //CONFIG
  writeSPI4(0x1B,0x18); //GYRO_CONFIG  

  // receivedData = readSPI4(0x6B);
  // sprintf(lcd_text_main,"PWR_MGMT_1 : %X    \n", receivedData); terminalWrite(lcd_text_main);
  // Delay_1us(100000);

  // receivedData = readSPI4(0x1A);
  // sprintf(lcd_text_main,"CONFIG : %X    \n", receivedData); terminalWrite(lcd_text_main);
  // Delay_1us(100000);

  // receivedData = readSPI4(0x1B);
  // sprintf(lcd_text_main,"GYRO_CONFIG : %X    \n", receivedData); terminalWrite(lcd_text_main);
  // Delay_1us(100000);

  EstV.G.X = 0.0; //pitch
  EstV.G.Y = 0.0; //roll 0
  EstV.G.Z = -1.0; //yaw 1
}

void Timer2_Initialization(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM5 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel =  TIM2_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  /* -- Timer Configuration --------------------------------------------------- */
  TIM_DeInit(TIM2);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_TimeBaseStruct.TIM_Period = 20000;  //250ms  --> 4Hz
  TIM_TimeBaseStruct.TIM_Prescaler = 900 - 1; // Prescaled by 900 -> = 0.1M(10us)
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1; // Div by one -> 90 MHz (Now RCC_DCKCFGR_TIMPRE is configured to divide clock by two)
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);    // Set interrupt when timer reloads (overflow)
  TIM_ARRPreloadConfig(TIM2, DISABLE);       //Put ARR value into register
  TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler()
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){

    f.print = 1;

    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}

int main(void)
{
  PWM_Initialization();
  TIM1->CCR2 = 1000;
  TIM1->CCR3 = 1000;
  Delay_1us(500000);

  RCC_Configuration();
  PushButton_Initialization();
  LED_Initialization();
  //LCD_Initialization();
  //terminalBufferInitilization();

  Delay_1us(100000);
  SPI_Initialization();
  Delay_1us(100000);
  IMU_Initialization();
  Delay_1us(100000);
  Timer5_Initialization(); //Filter
  Timer2_Initialization(); //Print
  Timer4_Initialization(); //Read IMU

  USART3_Configuration();
  USART3_puts("\r\nHello World!\r\n");
    
  while(1)
  {
    if(PushButton_Read()){
      if(f.arm == 0){ 
        f.arm = 1;
        Delay_1us(500000);
      }
      else if(f.arm == 1){
        f.arm = 0;
        Delay_1us(500000);
      }
    }

    if(f.imu == 1){
      //LED3_Toggle();
      readIMU(gyroRaw, GYRO_DATA_REGISTER);
      gyro_remove_offset(gyroRaw);
      readIMU(accRaw, ACC_DATA_REGISTER);
      f.imu = 0;
    // }  

    // if(f.filter == 1){
      //LED4_Toggle();
      Filter(gyroRaw, gyroAngle, accRaw, accAngle, Angle);
      if(f.arm == 1){
        PID_control(Angle);
      }
      else{
        TIM1->CCR2 = 1000;
        TIM1->CCR3 = 1000;

        errorI = 0;
        errorD = 0;
        previous_error = 0;
      }

      //f.filter = 0;
    }

    strcmd_main();      

    //if(f.print == 1){
      
      // sprintf(lcd_text_main,"%.4f %.4f %d", Angle[0], Angle[1], f.arm);
      // //sprintf(lcd_text_main,"G: %.3f %.3f %.3f", EstV.G.X, EstV.G.Y, EstV.G.Z);
      // LCD_DisplayStringLine(LINE(1), lcd_text_main);
      // //sprintf(lcd_text_main,"A: %.3f %.3f %.3f", EstV.A.X, EstV.A.Y, EstV.A.Z);
      // //sprintf(lcd_text_main,"A: %.3f %.3f", sqX_sqZ, EstV.GA.X*EstV.GA.X + EstV.GA.Z*EstV.GA.Z);
      // // sprintf(lcd_text_main,"%.4f %.4f %.4f \n", gyroAngle[0], gyroAngle[1], gyroAngle[2]);
      // sprintf(lcd_text_main,"%d     ", gyroRaw[2]);
      // LCD_DisplayStringLine(LINE(2), lcd_text_main);
      // sprintf(lcd_text_main,"GA: %.3f %.3f %.3f", EstV.GA.X, EstV.GA.Y, EstV.GA.Z);
      // LCD_DisplayStringLine(LINE(3), lcd_text_main);
      //sprintf(lcd_text_main,"%.3f %.3f %.3f\n", EstV.G.Z, EstV.A.Z, EstV.GA.Z);
      
      //LCD_DisplayStringLine(LINE(2), (uint8_t*)" Ming6842 @ github");
      //terminalWrite(lcd_text_main);
      //PRINT_USART();
      //f.print = 0;
    //}
  }

  while(1); // Don't want to exit
}

void PRINT_USART(){
  USART_SendData(USART3, 1);
  USART3_puts("\r\n");
}

void Arm_Func(){
  if(f.arm == 0){ 
    f.arm = 1;
    Delay_1us(500000);
  }
}

void Disarm_Func(){
  if(f.arm == 1){
    f.arm = 0;
    Delay_1us(500000);
  }
}