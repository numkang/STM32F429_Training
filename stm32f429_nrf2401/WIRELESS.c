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

void NRF2401_Initialization(void){

  GPIO_InitTypeDef GPIO_InitStructure;

    /*--------------- PWR (PD5) && CS (PD6) & CE (PD7) Pin Configuration -----------*/ 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /*------------ DR1 (PE2) & DR2 (PE3) Pin Configuration for RX Mode ------------*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP ;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

  NRF2401_InitTypeDef NRF2401_InitStructure;

    /*-------------------------- NRF2401 Configuration ----------------------------*/
    NRF2401_InitStructure.NRF2401_PLL_CTRL = NRF2401_PLL_CTRL_DEFAULT;

    NRF2401_InitStructure.NRF2401_CH2_DataSize = NRF2401_DATA2_W_1_BYTE;
    NRF2401_InitStructure.NRF2401_CH1_DataSize = NRF2401_DATA1_W_1_BYTE;

    NRF2401_InitStructure.NRF2401_ADDR2 = "4";
    NRF2401_InitStructure.NRF2401_ADDR1 = "3";

    NRF2401_InitStructure.NRF2401_ADDR_Length = NRF2401_ADDR_W_1_BYTE;
    NRF2401_InitStructure.NRF2401_CRC_Length = NRF2401_CRC_LENGTH_16_BITS;

    NRF2401_InitStructure.NRF2401_IS_CRC_Enable = NRF2401_CRC_ENABLED;
    NRF2401_InitStructure.NRF2401_IS_2CH_Enable = NRF2401_RX2_DISABLED;
    NRF2401_InitStructure.NRF2401_Communication_Mode = NRF2401_CM_SHOCKBURST_MODE;
    NRF2401_InitStructure.NRF2401_Data_Rate = NRF2401_RFDR_SB_1_MBPS;

    NRF2401_InitStructure.NRF2401_Crystal_Frequency = NRF2401_XO_F_DEFAULT;
    NRF2401_InitStructure.NRF2401_Output_Power = NRF2401_RF_PWR_MINUS_0_DB;
    NRF2401_InitStructure.NRF2401_RF_CH = NRF2401_RF_CH_DEFAULT;

    NRF2401_InitStructure.NRF2401_Active_Mode = NRF2401_TX_MODE;
    // NRF2401_InitStructure.NRF2401_Active_Mode = NRF2401_RX_MODE;

    NRF2401_InitStructure.NRF2401_SPIx = SPI1;
    NRF2401_InitStructure.NRF2401_CE_GPIOx = GPIOD;
    NRF2401_InitStructure.NRF2401_CE_GPIO_PinSource = GPIO_Pin_7;
    NRF2401_InitStructure.NRF2401_CS_GPIOx = GPIOD;
    NRF2401_InitStructure.NRF2401_CS_GPIO_PinSource = GPIO_Pin_6;
    NRF2401_InitStructure.NRF2401_PWR_GPIOx = GPIOD;
    NRF2401_InitStructure.NRF2401_PWR_GPIO_PinSource = GPIO_Pin_5;

    NRF2401_Init(&NRF2401_InitStructure);

    if(NRF2401_InitStructure.NRF2401_Active_Mode == NRF2401_RX_MODE){
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }

    
}