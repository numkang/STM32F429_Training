#include "WIRELESS.h"

void SPI1_Initialization(void){

  GPIO_InitTypeDef GPIO_InitStructure;

  	/*-------------------------- GPIO Configuration for SPI1 ----------------------------*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1); //SPI1_SCK
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1); //SPI1_MISO
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1); //SPI1_MOSI

  SPI_InitTypeDef SPI_InitStructure;

  	/*----------------------------SPI configuration ------------------------------------*/
  	SPI_I2S_DeInit(SPI1);
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
  	SPI_Init(SPI1, &SPI_InitStructure);

	  /* Enable SPI1  */
  	SPI_Cmd(SPI1, ENABLE);
}

void NRF24L01_Initialization(void){

  GPIO_InitTypeDef GPIO_InitStructure;

    /*--------------- CSN (PD6) & CE (PD7) Pin Configuration -----------*/ 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /*-------------- IQR (PE2)Pin Configuration for RX Mode ------------*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP ;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

  NRF24L01_InitTypeDef NRF24L01_InitStructure;

    /*-------------------------- NRF2401 Configuration ----------------------------*/
    NRF24L01_InitStructure.NRF24L01_CRC_Length = NRF24L01_CRC_LENGTH_2_BYTES;
    NRF24L01_InitStructure.NRF24L01_PWR = NRF24L01_PWR_UP;

    NRF24L01_InitStructure.NRF24L01_Mode = NRF24L01_PTX_MODE;
    // NRF24L01_InitStructure.NRF24L01_Mode = NRF24L01_PRX_MODE;

    NRF24L01_InitStructure.NRF24L01_ADDR_Width = NRF24L01_ADDR_W_4_BYTE;
    NRF24L01_InitStructure.NRF24L01_RF_CH = NRF24L01_RF_CH_DEFAULT;
    NRF24L01_InitStructure.NRF24L01_Data_Rate = NRF24L01_RF_DR_SB_2_MBPS;
    NRF24L01_InitStructure.NRF24L01_ADDR_P0 = "ADDR";
    NRF24L01_InitStructure.NRF24L01_DataSize_P0 = 3; //bytes

    NRF24L01_InitStructure.NRF24L01_SPIx = SPI1;
    NRF24L01_InitStructure.NRF24L01_CE_GPIOx = GPIOD;
    NRF24L01_InitStructure.NRF24L01_CE_GPIO_PinSource = GPIO_Pin_7;
    NRF24L01_InitStructure.NRF24L01_CSN_GPIOx = GPIOD;
    NRF24L01_InitStructure.NRF24L01_CSN_GPIO_PinSource = GPIO_Pin_6;
    NRF24L01_InitStructure.NRF24L01_IQR_GPIOx = GPIOE;
    NRF24L01_InitStructure.NRF24L01_IQR_GPIO_PinSource = GPIO_Pin_2;

    NRF24L01_Init(&NRF24L01_InitStructure);    
}