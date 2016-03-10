#include "main.h"

static inline void Delay_1us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 13; nCnt != 0; nCnt--);
}

void SPI_Initialization(void){

  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);


  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPI5);
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
  SPI_Init(SPI5, &SPI_InitStructure);

  /* Enable SPI5  */
  SPI_Cmd(SPI5, ENABLE);
  
  /* Configure GPIO PIN for Chip select */
  /*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);*/

  
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_SPI5);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_SPI5);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_SPI5);

  /* Configure GPIO PIN for SPI5 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* Initialize CS pin */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

int main(void)
{

  
  
    lcd_init();
    lcd_drawBackground(20,60,250);
    lcd_drawBGPersimmon(20, 60, 250);
    LCD_SetColors(LCD_COLOR_WHITE-1,LCD_COLOR_WHITE);

    LCD_SetFont(&Font8x12); 

    terminalBufferInitilization();


    Delay_1us(100000);
    SPI_Initialization();

    Delay_1us(100000);
    receivedData=readSPI5(0x8F);
    sprintf(lcd_text_main,"My Name is : %d    \n", receivedData); terminalWrite(lcd_text_main);
    Delay_1us(500000);

    writeSPI5(0x20,0xFF); //CTRL_REG1    
    Delay_1us(100000);
    receivedData=readSPI5(0xA0);
    sprintf(lcd_text_main,"REG1 : %x    \n", receivedData); terminalWrite(lcd_text_main);

    writeSPI5(0x23,0x30); //CTRL_REG4    
    Delay_1us(100000);
    receivedData=readSPI5(0xA3);
    sprintf(lcd_text_main,"REG4 : %x    \n", receivedData); terminalWrite(lcd_text_main);
    //Delay_1us(100000);

    /*receivedData=readSPI5(0xA0);
    if(receivedData==7){
      sprintf(lcd_text_main,"Config Completed \n"); terminalWrite(lcd_text_main);
    }*/
    Delay_1us(500000);

    while(1){
    //receivedData=readSPI5(0xA8);
    //sprintf(lcd_text_main,"Gyro : %d    \n", receivedData); terminalWrite(lcd_text_main);

      readGyro();

  // LCD_Init();
      //sprintf(lcd_text_main,"x : %d , y : %d , z : %d   \n", Gyro_Raw[0], Gyro_Raw[1], Gyro_Raw[2]); 
      //terminalWrite(lcd_text_main); 
   
      // Delay_1us(500000);
    }  
}

uint8_t readGyro(){
  uint8_t buff[6];
  GPIO_ResetBits(GPIOC,GPIO_Pin_1);
      SPI_I2S_SendData(SPI5,0xE8); //XL
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      buff[0]=SPI_I2S_ReceiveData(SPI5);
      
      SPI_I2S_SendData(SPI5,0x00); //XH
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      buff[0]=SPI_I2S_ReceiveData(SPI5);

      SPI_I2S_SendData(SPI5,0x00); //YL
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      buff[1]=SPI_I2S_ReceiveData(SPI5);

      SPI_I2S_SendData(SPI5,0x00); //YH
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      buff[2]=SPI_I2S_ReceiveData(SPI5);

      SPI_I2S_SendData(SPI5,0x00); //ZL
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      buff[3]=SPI_I2S_ReceiveData(SPI5);

      SPI_I2S_SendData(SPI5,0x00); //ZH
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      buff[4]=SPI_I2S_ReceiveData(SPI5);

      SPI_I2S_SendData(SPI5,0x00);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      buff[5]=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC,GPIO_Pin_1);

      Gyro_Raw[0] = (((uint16_t)buff[1] << 8) | (uint16_t)buff[0]);//*sensitivity;
      Gyro_Raw[1] = (((uint16_t)buff[3] << 8) | (uint16_t)buff[2]);//*sensitivity;
      Gyro_Raw[2] = (((uint16_t)buff[5] << 8) | (uint16_t)buff[4]);//*sensitivity;

      sprintf(lcd_text_main,"%ld \n", Gyro_Raw[2]);
      //sprintf(lcd_text_main,"%d , %d , %d , %d , %d , %d \n", buff[0], buff[1], buff[2],buff[3], buff[4], buff[5]); 
      terminalWrite(lcd_text_main);

      /*Gyro_Raw[0] = ((buff[1] << 8) | buff[0])*sensitivity;
      Gyro_Raw[1] = ((buff[3] << 8) | buff[2])*sensitivity;
      Gyro_Raw[2] = ((buff[5] << 8) | buff[4])*sensitivity;*/
}

uint8_t readSPI5(uint16_t addr){ //+8
  uint8_t readData=0;
      GPIO_ResetBits(GPIOC,GPIO_Pin_1);
      SPI_I2S_SendData(SPI5,addr);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      readData=SPI_I2S_ReceiveData(SPI5);

      SPI_I2S_SendData(SPI5,0x00);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      readData=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC,GPIO_Pin_1);

      return readData;
}

uint8_t writeSPI5(uint8_t addr, uint8_t data){
  uint8_t readData=0;
      GPIO_ResetBits(GPIOC,GPIO_Pin_1);
      SPI_I2S_SendData(SPI5,addr);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      readData=SPI_I2S_ReceiveData(SPI5);

      SPI_I2S_SendData(SPI5,data);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      readData=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC,GPIO_Pin_1);

      return readData;
}

