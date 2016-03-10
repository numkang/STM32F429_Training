#include "IMU.h"

#define GYRO_TO_SAMPLE 400
int16_t calibratingG = GYRO_TO_SAMPLE;
int16_t GYRO_Zero[3];

void SPI_Initialization(void){

  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);


  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPI4);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  /* SPI baudrate is set to 5.6 MHz (PCLK2/SPI_BaudRatePrescaler = 90/16 = 5.625 MHz)  */

  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI4, &SPI_InitStructure);

  /* Enable SPI4  */
  SPI_Cmd(SPI4, ENABLE);
  
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_SPI5); //SCK
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_SPI5); //MISO
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_SPI5); //MOSI

  /* Configure GPIO PIN for SPI4 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_5 |GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Initialize CS pin */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void readIMU(int16_t *raw, uint8_t reg){
  uint8_t buff[6];

  GPIO_ResetBits(GPIOE,GPIO_Pin_3);
  SPI_I2S_SendData(SPI4,reg | 0x80); //XH
  while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);

  while (SPI_I2S_GetFlagStatus(SPI4, SPI_FLAG_RXNE) == RESET);
  buff[0]=SPI_I2S_ReceiveData(SPI4);
      
  SPI_I2S_SendData(SPI4,reg + 0x01); //XL
  while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);

  while (SPI_I2S_GetFlagStatus(SPI4, SPI_FLAG_RXNE) == RESET);
  buff[0]=SPI_I2S_ReceiveData(SPI4);

  SPI_I2S_SendData(SPI4,reg + 0x02); //YH
  while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);

  while (SPI_I2S_GetFlagStatus(SPI4, SPI_FLAG_RXNE) == RESET);
  buff[1]=SPI_I2S_ReceiveData(SPI4);

  SPI_I2S_SendData(SPI4,reg + 0x03); //YL
  while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);

  while (SPI_I2S_GetFlagStatus(SPI4, SPI_FLAG_RXNE) == RESET);
  buff[2]=SPI_I2S_ReceiveData(SPI4);

  SPI_I2S_SendData(SPI4,reg + 0x04); //ZH
  while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);

  while (SPI_I2S_GetFlagStatus(SPI4, SPI_FLAG_RXNE) == RESET);
  buff[3]=SPI_I2S_ReceiveData(SPI4);

  SPI_I2S_SendData(SPI4,reg + 0x05); //ZL
  while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);

  while (SPI_I2S_GetFlagStatus(SPI4, SPI_FLAG_RXNE) == RESET);
  buff[4]=SPI_I2S_ReceiveData(SPI4);

  SPI_I2S_SendData(SPI4,0xFF);
  while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);

  while (SPI_I2S_GetFlagStatus(SPI4, SPI_FLAG_RXNE) == RESET);
  buff[5]=SPI_I2S_ReceiveData(SPI4);
  GPIO_SetBits(GPIOE,GPIO_Pin_3);

  raw[0] = (((uint16_t)buff[0] << 8) | (uint16_t)buff[1]);
  raw[1] = (((uint16_t)buff[2] << 8) | (uint16_t)buff[3]);
  raw[2] = (((uint16_t)buff[4] << 8) | (uint16_t)buff[5]);
}

uint8_t readSPI4(uint16_t addr){
  uint8_t readData = 0;
  GPIO_ResetBits(GPIOE,GPIO_Pin_3);
  SPI_I2S_SendData(SPI4,addr | 0x80);
  while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);

  while (SPI_I2S_GetFlagStatus(SPI4, SPI_FLAG_RXNE) == RESET);
  readData=SPI_I2S_ReceiveData(SPI4);

  SPI_I2S_SendData(SPI4,0xFF);
  while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);

  while (SPI_I2S_GetFlagStatus(SPI4, SPI_FLAG_RXNE) == RESET);
  readData=SPI_I2S_ReceiveData(SPI4);
  GPIO_SetBits(GPIOE,GPIO_Pin_3);

  return readData;
}

uint8_t writeSPI4(uint8_t addr, uint8_t data){
  uint8_t readData = 0;
  GPIO_ResetBits(GPIOE,GPIO_Pin_3);
  SPI_I2S_SendData(SPI4,addr);
  while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);

  while (SPI_I2S_GetFlagStatus(SPI4, SPI_FLAG_RXNE) == RESET);
  readData=SPI_I2S_ReceiveData(SPI4);

  SPI_I2S_SendData(SPI4,data);
  while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);

  while (SPI_I2S_GetFlagStatus(SPI4, SPI_FLAG_RXNE) == RESET);
  readData=SPI_I2S_ReceiveData(SPI4);
  GPIO_SetBits(GPIOE,GPIO_Pin_3);

  return readData;
}

void Timer4_Initialization(void)
{

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM5 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel =  TIM4_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  /* -- Timer Configuration --------------------------------------------------- */
  TIM_DeInit(TIM4);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_TimeBaseStruct.TIM_Period = 25;  //250ms  --> 4Hz
  TIM_TimeBaseStruct.TIM_Prescaler = 900 - 1; // Prescaled by 900 -> = 0.1M(10us)
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1; // Div by one -> 90 MHz (Now RCC_DCKCFGR_TIMPRE is configured to divide clock by two)
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);    // Set interrupt when timer reloads (overflow)
  TIM_ARRPreloadConfig(TIM5, DISABLE);       //Put ARR value into register
  TIM_Cmd(TIM4, ENABLE);
}

void TIM4_IRQHandler()
{
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
    f.imu = 1;

    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  }
}

void gyro_remove_offset(int16_t *gRaw) {
  static int16_t lastGyro[3] = {0,0,0};
  static int32_t gyro_sample[3];
  uint8_t i;

  if (calibratingG>0) {
    for (i = 0; i < 3; i++) {
      if (calibratingG == GYRO_TO_SAMPLE) gyro_sample[i]=0;
      gyro_sample[i] += gRaw[i];
      gRaw[i]=0;
      GYRO_Zero[i]=0;
      if (calibratingG == 1) {
        GYRO_Zero[i] = gyro_sample[i]/GYRO_TO_SAMPLE;
        f.gyroCalibrated = 1;    
      }
    }
    calibratingG--;
  }

  for (i = 0; i < 3; i++) {
    gRaw[i] -= GYRO_Zero[i];
    //gRaw[i] = constrain(gRaw[i],lastGyro[i]-800,lastGyro[i]+800);  
    if(gRaw[i] < lastGyro[i]-800) gRaw[i] = lastGyro[i] - 800;
    else if(gRaw[i] > lastGyro[i]+800) gRaw[i] = lastGyro[i] + 800;
    lastGyro[i] = gRaw[i];
  }
}