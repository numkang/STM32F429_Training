#include "NRF24L01.h"

char text_main[100];

SPI_TypeDef* NRF24L01_Init_SPIx;
GPIO_TypeDef *NRF24L01_Init_CE_GPIOx, *NRF24L01_Init_CSN_GPIOx, *NRF24L01_Init_IQR_GPIOx;
uint16_t NRF24L01_Init_CE_GPIO_PinSource, NRF24L01_Init_CSN_GPIO_PinSource, NRF24L01_Init_IQR_GPIO_PinSource;
uint8_t NRF24L01_Init_ADDR_Width, NRF24L01_Init_DataSize;
char* NRF24L01_Init_ADDR_P0;

void Delay_us(uint32_t nCnt_1us){
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 13; nCnt != 0; nCnt--);
}

void NRF24L01_W_REG(uint8_t REG, uint8_t DAT){
	uint8_t status, buff;

	GPIO_ResetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	SPI_I2S_SendData(NRF24L01_Init_SPIx, NRF24L01_W_REGISTER | REG);
	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
	status = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);

	SPI_I2S_SendData(NRF24L01_Init_SPIx, DAT);
	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
	buff = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);

	GPIO_SetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);
	Delay_us(10);
}

void NRF24L01_R_REG(uint8_t REG){
	uint8_t status, reg;

	GPIO_ResetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	SPI_I2S_SendData(NRF24L01_Init_SPIx, NRF24L01_R_REGISTER | REG);
	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
	status = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);

	SPI_I2S_SendData(NRF24L01_Init_SPIx, 0x00);
	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
	reg = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);

	Delay_us(10);
	GPIO_SetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	sprintf(text_main,"%d,%d ", status, reg);
	USART1_puts(text_main);

}

void NRF24L01_W_ADDR(uint8_t REG, char *DAT){
	uint8_t status, buff, i;

	GPIO_ResetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	SPI_I2S_SendData(NRF24L01_Init_SPIx, NRF24L01_W_REGISTER | REG);
	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
	status = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);

	for(i = 0; i < NRF24L01_Init_ADDR_Width; i++){
		SPI_I2S_SendData(NRF24L01_Init_SPIx, DAT[NRF24L01_Init_ADDR_Width - 1 - i]);
		while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

		while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
		buff = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);
	}

	GPIO_SetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);
	Delay_us(10);
}

void NRF24L01_R_ADDR(uint8_t REG){
	uint8_t status, i;
	uint8_t addr[NRF24L01_Init_ADDR_Width];

	GPIO_ResetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	SPI_I2S_SendData(NRF24L01_Init_SPIx, NRF24L01_R_REGISTER | REG);
	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
	status = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);

	for(i = 0; i < NRF24L01_Init_ADDR_Width; i++){
		SPI_I2S_SendData(NRF24L01_Init_SPIx, 0x00);
		while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

		while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
		addr[NRF24L01_Init_ADDR_Width - 1 - i] = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);
	}

	GPIO_SetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	sprintf(text_main,"%d,%d,%d,%d ", status, addr[0], addr[1], addr[2]);
	USART1_puts(text_main);
}

void NRF24L01_ACTIVATE_FUNC(){
	uint8_t status, buff;

	GPIO_ResetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	SPI_I2S_SendData(NRF24L01_Init_SPIx, NRF24L01_ACTIVATE);
	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
	status = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);

	SPI_I2S_SendData(NRF24L01_Init_SPIx, 0x73);
	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
	buff = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);

	GPIO_SetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);
	Delay_us(10);
}

void NRF24L01_Init(NRF24L01_InitTypeDef* NRF24L01_InitStruct){
	uint8_t data;

	NRF24L01_Init_SPIx = NRF24L01_InitStruct->NRF24L01_SPIx;
	NRF24L01_Init_CE_GPIOx = NRF24L01_InitStruct->NRF24L01_CE_GPIOx;
	NRF24L01_Init_CE_GPIO_PinSource = NRF24L01_InitStruct->NRF24L01_CE_GPIO_PinSource;
	NRF24L01_Init_CSN_GPIOx = NRF24L01_InitStruct->NRF24L01_CSN_GPIOx;
	NRF24L01_Init_CSN_GPIO_PinSource = NRF24L01_InitStruct->NRF24L01_CSN_GPIO_PinSource;
	NRF24L01_Init_IQR_GPIOx = NRF24L01_InitStruct->NRF24L01_IQR_GPIOx;
	NRF24L01_Init_IQR_GPIO_PinSource = NRF24L01_InitStruct->NRF24L01_IQR_GPIO_PinSource;
	NRF24L01_Init_ADDR_Width = NRF24L01_InitStruct->NRF24L01_ADDR_Width + 2;
	NRF24L01_Init_DataSize = NRF24L01_InitStruct->NRF24L01_DataSize_P0;
	NRF24L01_Init_ADDR_P0 = NRF24L01_InitStruct->NRF24L01_ADDR_P0;

	data = NRF24L01_CONFIG_RESET_VALUE | NRF24L01_InitStruct->NRF24L01_CRC_Length | NRF24L01_InitStruct->NRF24L01_PWR | NRF24L01_InitStruct->NRF24L01_Mode;
	NRF24L01_W_REG(NRF24L01_CONFIG, data);

	// data = 0x00; //Turn Off ACK
	// NRF24L01_W_REG(NRF24L01_EN_AA, data);

	data = NRF24L01_InitStruct->NRF24L01_ADDR_Width;
	NRF24L01_W_REG(NRF24L01_SETUP_AW, data);

	data = NRF24L01_InitStruct->NRF24L01_RF_CH;
	NRF24L01_W_REG(NRF24L01_RF_CH_REG, data);

	data = NRF24L01_RF_SETUP_RESET_VALUE | NRF24L01_InitStruct->NRF24L01_Data_Rate;
	NRF24L01_W_REG(NRF24L01_RF_SETUP, data);

	data = NRF24L01_InitStruct->NRF24L01_DataSize_P0;
	NRF24L01_W_REG(NRF24L01_RX_PW_P0, data);

	NRF24L01_ACTIVATE_FUNC();

	NRF24L01_W_ADDR(NRF24L01_RX_ADDR_P0, NRF24L01_Init_ADDR_P0);
	NRF24L01_W_ADDR(NRF24L01_TX_ADDR, NRF24L01_Init_ADDR_P0);
}

void NRF24L01_SendData(uint8_t* Data){
	uint8_t status, buff, i;

	GPIO_ResetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	SPI_I2S_SendData(NRF24L01_Init_SPIx, NRF24L01_W_TX_PAYLOAD);
	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
	status = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);

	for(i = 0; i < NRF24L01_Init_DataSize; i++){
		SPI_I2S_SendData(NRF24L01_Init_SPIx, Data[NRF24L01_Init_DataSize - 1 - i]);
		while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

		while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
		buff = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);
	}

	GPIO_SetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	GPIO_SetBits(NRF24L01_Init_CE_GPIOx, NRF24L01_Init_CE_GPIO_PinSource);
	Delay_us(10);
	GPIO_ResetBits(NRF24L01_Init_CE_GPIOx, NRF24L01_Init_CE_GPIO_PinSource);

	// if(GPIO_ReadInputDataBit(NRF24L01_Init_IQR_GPIOx, NRF24L01_Init_IQR_GPIO_PinSource) == 0){
	// 	NRF24L01_W_REG(NRF24L01_STATUS, NRF24L01_STATUS_Clear_Bits);
	// }

	NRF24L01_W_REG(NRF24L01_STATUS, NRF24L01_STATUS_Clear_Bits);

	// sprintf(text_main,"%d ", buff);
	// USART1_puts(text_main);
}

uint8_t* NRF24L01_ReceiveData(void){
	uint8_t NRF2401_RX_Word[NRF24L01_Init_DataSize];
	uint8_t status, i;

	GPIO_SetBits(NRF24L01_Init_CE_GPIOx, NRF24L01_Init_CE_GPIO_PinSource);

	while(GPIO_ReadInputDataBit(NRF24L01_Init_IQR_GPIOx, NRF24L01_Init_IQR_GPIO_PinSource) == 1){
		LED3_On();
	}

	LED3_Off();
	GPIO_ResetBits(NRF24L01_Init_CE_GPIOx, NRF24L01_Init_CE_GPIO_PinSource);

	GPIO_ResetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	SPI_I2S_SendData(NRF24L01_Init_SPIx, NRF24L01_R_RX_PAYLOAD);
	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
	status = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);

	for(i = 0; i < NRF24L01_Init_DataSize; i++){
		SPI_I2S_SendData(NRF24L01_Init_SPIx, 0x00);
	 	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

		while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
	  	NRF2401_RX_Word[NRF24L01_Init_DataSize - 1 - i] = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);

		// sprintf(text_main,"%d,%d,%d ", status, NRF2401_RX_Word[i], i);
		// USART1_puts(text_main);
	}

	Delay_us(10);
	GPIO_SetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	// if(GPIO_ReadInputDataBit(NRF24L01_Init_IQR_GPIOx, NRF24L01_Init_IQR_GPIO_PinSource) == 0){
	// 	NRF24L01_W_REG(NRF24L01_STATUS, NRF24L01_STATUS_Clear_Bits);
	// }

	NRF24L01_W_REG(NRF24L01_STATUS, NRF24L01_STATUS_Clear_Bits);

	return NRF2401_RX_Word;
}

void NRF24L01_SendData_ACK(uint8_t* Data){
	uint8_t status, buff, i;

	GPIO_ResetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	SPI_I2S_SendData(NRF24L01_Init_SPIx, NRF24L01_W_ACK_PAYLOAD); //data pipe 0
	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
	status = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);

	for(i = 0; i < NRF24L01_Init_DataSize; i++){
		SPI_I2S_SendData(NRF24L01_Init_SPIx, Data[NRF24L01_Init_DataSize - 1 - i]);
		while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

		while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
		buff = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);
	}

	GPIO_SetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	GPIO_SetBits(NRF24L01_Init_CE_GPIOx, NRF24L01_Init_CE_GPIO_PinSource);
	Delay_us(10);
	GPIO_ResetBits(NRF24L01_Init_CE_GPIOx, NRF24L01_Init_CE_GPIO_PinSource);

	// if(GPIO_ReadInputDataBit(NRF24L01_Init_IQR_GPIOx, NRF24L01_Init_IQR_GPIO_PinSource) == 0){
	// 	NRF24L01_W_REG(NRF24L01_STATUS, NRF24L01_STATUS_Clear_Bits);
	// }

	NRF24L01_W_REG(NRF24L01_STATUS, NRF24L01_STATUS_Clear_Bits);

	// sprintf(text_main,"%d ", buff);
	// USART1_puts(text_main);
}

void NRF24L01_SendData_NOACK(uint8_t* Data){
	uint8_t status, buff, i;

	GPIO_ResetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	SPI_I2S_SendData(NRF24L01_Init_SPIx, NRF24L01_W_TX_PAYLOAD_NOACK);
	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
	status = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);

	for(i = 0; i < NRF24L01_Init_DataSize; i++){
		SPI_I2S_SendData(NRF24L01_Init_SPIx, Data[NRF24L01_Init_DataSize - 1 - i]);
		while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_I2S_FLAG_TXE) == RESET);

		while (SPI_I2S_GetFlagStatus(NRF24L01_Init_SPIx, SPI_FLAG_RXNE) == RESET);
		buff = SPI_I2S_ReceiveData(NRF24L01_Init_SPIx);
	}

	GPIO_SetBits(NRF24L01_Init_CSN_GPIOx, NRF24L01_Init_CSN_GPIO_PinSource);

	GPIO_SetBits(NRF24L01_Init_CE_GPIOx, NRF24L01_Init_CE_GPIO_PinSource);
	Delay_us(10);
	GPIO_ResetBits(NRF24L01_Init_CE_GPIOx, NRF24L01_Init_CE_GPIO_PinSource);

	// if(GPIO_ReadInputDataBit(NRF24L01_Init_IQR_GPIOx, NRF24L01_Init_IQR_GPIO_PinSource) == 0){
	// 	NRF24L01_W_REG(NRF24L01_STATUS, NRF24L01_STATUS_Clear_Bits);
	// }

	NRF24L01_W_REG(NRF24L01_STATUS, NRF24L01_STATUS_Clear_Bits);

	// sprintf(text_main,"%d ", buff);
	// USART1_puts(text_main);
}
