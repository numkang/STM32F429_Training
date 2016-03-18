#include "NRF2401.h"
#include "USART.h"
#include "main.h"
#include "LED.h"

char text_main[100];

SPI_TypeDef* NRF2401_Init_SPIx;
GPIO_TypeDef* NRF2401_Init_CE_GPIOx;
uint16_t NRF2401_Init_CE_GPIO_PinSource;
char *NRF2401_Init_ADDR2, *NRF2401_Init_ADDR1;
uint8_t NRF2401_Init_ADDR_Length, NRF2401_Init_CH2_DataSize, NRF2401_Init_CH1_DataSize;

void Delay_us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 13; nCnt != 0; nCnt--);
}

void NRF2401_Init(NRF2401_InitTypeDef* NRF2401_InitStruct){
	GPIO_ResetBits(NRF2401_InitStruct->NRF2401_PWR_GPIOx, NRF2401_InitStruct->NRF2401_PWR_GPIO_PinSource);
	GPIO_ResetBits(NRF2401_InitStruct->NRF2401_CE_GPIOx, NRF2401_InitStruct->NRF2401_CE_GPIO_PinSource);
	GPIO_ResetBits(NRF2401_InitStruct->NRF2401_CS_GPIOx, NRF2401_InitStruct->NRF2401_CS_GPIO_PinSource);
	Delay_us(50000);

	uint8_t NRF2401_Configuration_Word[18] = {0x8E, 0x08, 0x1C, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x21, 0x0F, 0x04};

	NRF2401_Configuration_Word[0] = NRF2401_RESERVED_1;
	NRF2401_Configuration_Word[1] = NRF2401_RESERVED_2;
	NRF2401_Configuration_Word[2] = (NRF2401_RESERVED_3 << 2) | NRF2401_InitStruct->NRF2401_PLL_CTRL;
	NRF2401_Configuration_Word[3] = NRF2401_InitStruct->NRF2401_CH2_DataSize;
	NRF2401_Configuration_Word[4] = NRF2401_InitStruct->NRF2401_CH1_DataSize;

	uint8_t i, j;
	for(i = 0; i < (NRF2401_InitStruct->NRF2401_ADDR_Length >> 3); i++){
		j = 10 - (NRF2401_InitStruct->NRF2401_ADDR_Length >> 3) + i;
		NRF2401_Configuration_Word[j] = NRF2401_InitStruct->NRF2401_ADDR2[i];

		j = 15 - (NRF2401_InitStruct->NRF2401_ADDR_Length >> 3) + i;
		NRF2401_Configuration_Word[j] = NRF2401_InitStruct->NRF2401_ADDR1[i];
	}

	NRF2401_Configuration_Word[15] = (NRF2401_InitStruct->NRF2401_ADDR_Length << 2) | (NRF2401_InitStruct->NRF2401_CRC_Length << 1) | (NRF2401_InitStruct->NRF2401_IS_CRC_Enable);
	NRF2401_Configuration_Word[16] = (NRF2401_InitStruct->NRF2401_IS_2CH_Enable << 7) | (NRF2401_InitStruct->NRF2401_Communication_Mode << 6) | (NRF2401_InitStruct->NRF2401_Data_Rate << 5) | (NRF2401_InitStruct->NRF2401_Crystal_Frequency << 2) | (NRF2401_InitStruct->NRF2401_Output_Power);
	NRF2401_Configuration_Word[17] = (NRF2401_InitStruct->NRF2401_RF_CH << 1) | NRF2401_InitStruct->NRF2401_Active_Mode;;

	GPIO_SetBits(NRF2401_InitStruct->NRF2401_PWR_GPIOx, NRF2401_InitStruct->NRF2401_PWR_GPIO_PinSource);
	Delay_us(3000);

	GPIO_SetBits(NRF2401_InitStruct->NRF2401_CS_GPIOx, NRF2401_InitStruct->NRF2401_CS_GPIO_PinSource);
	Delay_us(5);

	for(i = 0; i < 18; i++){
		SPI_I2S_SendData(NRF2401_InitStruct->NRF2401_SPIx, NRF2401_Configuration_Word[i]);
	  	while (SPI_I2S_GetFlagStatus(NRF2401_InitStruct->NRF2401_SPIx, SPI_I2S_FLAG_TXE) == RESET);

		while (SPI_I2S_GetFlagStatus(NRF2401_InitStruct->NRF2401_SPIx, SPI_FLAG_RXNE) == RESET);
	  	j = SPI_I2S_ReceiveData(NRF2401_InitStruct->NRF2401_SPIx);

	  	// sprintf(text_main,"%X,%d ", NRF2401_Configuration_Word[i], i);
	  	// USART1_puts(text_main);
	}
	// USART1_puts("\r\n123456789 123456789 123456789\r\n");

	GPIO_ResetBits(NRF2401_InitStruct->NRF2401_CS_GPIOx, NRF2401_InitStruct->NRF2401_CS_GPIO_PinSource);

	NRF2401_Init_SPIx = NRF2401_InitStruct->NRF2401_SPIx;
	NRF2401_Init_CE_GPIOx = NRF2401_InitStruct->NRF2401_CE_GPIOx;
	NRF2401_Init_CE_GPIO_PinSource = NRF2401_InitStruct->NRF2401_CE_GPIO_PinSource;
	NRF2401_Init_ADDR2 = NRF2401_InitStruct->NRF2401_ADDR2;
	NRF2401_Init_ADDR1 = NRF2401_InitStruct->NRF2401_ADDR1;
	NRF2401_Init_ADDR_Length = NRF2401_InitStruct->NRF2401_ADDR_Length;
	NRF2401_Init_CH2_DataSize = NRF2401_InitStruct->NRF2401_CH2_DataSize;
	NRF2401_Init_CH1_DataSize = NRF2401_InitStruct->NRF2401_CH1_DataSize;

	GPIO_ResetBits(NRF2401_InitStruct->NRF2401_PWR_GPIOx, NRF2401_InitStruct->NRF2401_PWR_GPIO_PinSource);
	Delay_us(5000);
	GPIO_SetBits(NRF2401_InitStruct->NRF2401_PWR_GPIOx, NRF2401_InitStruct->NRF2401_PWR_GPIO_PinSource);
	Delay_us(3000);
	GPIO_SetBits(NRF2401_InitStruct->NRF2401_CE_GPIOx, NRF2401_InitStruct->NRF2401_CE_GPIO_PinSource);
	Delay_us(5);
}

void NRF2401_SendData_1CH(uint8_t* Data){
	uint8_t Payload_Size = (NRF2401_Init_ADDR_Length >> 3) + (NRF2401_Init_CH1_DataSize >> 3);
	uint8_t NRF2401_TX_Word[Payload_Size];
	uint8_t i, j;

	for(i = 0; i < (NRF2401_Init_ADDR_Length >> 3); i++){
		NRF2401_TX_Word[i] = NRF2401_Init_ADDR1[i];
	}

	for(i = (NRF2401_Init_ADDR_Length >> 3); i < Payload_Size; i++){
		NRF2401_TX_Word[i] = Data[i - (NRF2401_Init_ADDR_Length >> 3)];
	}

	GPIO_SetBits(NRF2401_Init_CE_GPIOx, NRF2401_Init_CE_GPIO_PinSource);
	Delay_us(5);

	for(i = 0; i < Payload_Size; i++){
		SPI_I2S_SendData(NRF2401_Init_SPIx, NRF2401_TX_Word[i]);
	  	while (SPI_I2S_GetFlagStatus(NRF2401_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

		while (SPI_I2S_GetFlagStatus(NRF2401_Init_SPIx, SPI_FLAG_RXNE) == RESET);
	  	j = SPI_I2S_ReceiveData(NRF2401_Init_SPIx);

	  	// sprintf(text_main,"%c,%d ", NRF2401_TX_Word[i], i);
	  	// USART1_puts(text_main);
	}
	// USART1_puts("\r\n123456789 123456789 123456789\r\n");

	GPIO_ResetBits(NRF2401_Init_CE_GPIOx, NRF2401_Init_CE_GPIO_PinSource);
	Delay_us(195);
}

uint8_t* NRF2401_ReceiveData_1CH(void){
	uint8_t NRF2401_RX_Word[(NRF2401_Init_CH1_DataSize >> 3)];
	uint8_t i;

	GPIO_SetBits(NRF2401_Init_CE_GPIOx, NRF2401_Init_CE_GPIO_PinSource);
	// Delay_us(200);

	while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0){
		LED3_On();
	}

	// if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 1){
		Delay_us(1);
		LED3_Off();
		GPIO_ResetBits(NRF2401_Init_CE_GPIOx, NRF2401_Init_CE_GPIO_PinSource);
		
		for(i = 0; i < (NRF2401_Init_CH1_DataSize >> 3); i++){
			SPI_I2S_SendData(NRF2401_Init_SPIx, 0x00);
	  		while (SPI_I2S_GetFlagStatus(NRF2401_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

			while (SPI_I2S_GetFlagStatus(NRF2401_Init_SPIx, SPI_FLAG_RXNE) == RESET);
		  	NRF2401_RX_Word[i] = SPI_I2S_ReceiveData(NRF2401_Init_SPIx);

	  		// sprintf(text_main,"%d,%d ", NRF2401_RX_Word[i], i);
	  		sprintf(text_main,"%d ", NRF2401_RX_Word[i]);
	  		USART1_puts(text_main);
		}
	// }
	// USART1_puts("\r\n123456789 123456789 123456789\r\n");

	// GPIO_ResetBits(NRF2401_Init_CE_GPIOx, NRF2401_Init_CE_GPIO_PinSource);

	return NRF2401_RX_Word;
}
