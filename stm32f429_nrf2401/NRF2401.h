// NRF2401.h
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NRF2401_H
#define __NRF2401_H

#include "stm32f4xx.h"

typedef struct
{
  uint8_t NRF2401_PLL_CTRL;        	/*!< Specifies the NRF2401 controlling the setting of the PLL for test purposes.
                                        	This parameter can be a value of @ref NRF2401_PLL_CTRL */

  uint8_t NRF2401_CH2_DataSize;    	/*!< Specifies the NRF2401 Length of RF package payload section for receive-channel 2.
                                        	This parameter can be a value of @ref NRF2401_Data_Length_for_Channel2 */

  uint8_t NRF2401_CH1_DataSize;        /*!< Specifies the NRF2401 Length of RF package payload section for receive-channel 1.
                                        	This parameter can be a value of @ref NRF2401_Data_Length_for_Channel1 */

  char* NRF2401_ADDR2;            		/*!< Specifies the NRF2401 Receiver address channel 2, up to 40 bit.
                                        	This parameter can be a value of @ref NRF2401_Channel_Address_for_Channel2 */

  char* NRF2401_ADDR1;            		/*!< Specifies the NRF2401 Receiver address channel 1, up to 40 bit.
                                        	This parameter can be a value of @ref NRF2401_Channel_Address_for_Channel1 */

  uint8_t NRF2401_ADDR_Length;         /*!< Specifies the NRF2401 Number of bits reserved for RX address in ShockBurst™ packages.
                                        	This parameter can be a value of @ref NRF2401_Channel_Address_Width_for_Both_Channels */

  uint8_t NRF2401_CRC_Length;          /*!< Specifies the NRF2401 CRC length to be calculated by nRF2401 in ShockBurst™.
                                        	This parameter can be a value of @ref NRF2401_Cyclic_Redundancy_Check_LENGTH */

  uint8_t NRF2401_IS_CRC_Enable;       /*!< Specifies the NRF2401 Enables on-chip CRC generation (TX) and verification (RX).
                                        	This parameter can be a value of @ref NRF2401_Cyclic_Redundancy_Check */
 
  uint8_t NRF2401_IS_2CH_Enable;   	/*!< Specifies the NRF2401 One channel receive or Two channel receive
                                        	This parameter can be a value of @ref NRF2401_2_Channel_Receive_Mode */

  uint8_t NRF2401_Communication_Mode;  /*!< Specifies the NRF2401 Communication Mode.
                                        	This parameter can be a value of @ref NRF2401_Communication_Mode */

  uint8_t NRF2401_Data_Rate;       	/*!< Specifies the NRF2401 Data Rate.
                                        	This parameter can be a value of @ref NRF2401_RF_Data_Rate */

  uint8_t NRF2401_Crystal_Frequency;   /*!< Specifies the NRF2401 Crystal Frequency.
                                        	This parameter can be a value of @ref NRF2401_Crystal_Frequency */

  uint8_t NRF2401_Output_Power;        /*!< Specifies the NRF2401 Output Power.
                                        	This parameter can be a value of @ref NRF2401_RF_Output_Power */

  uint8_t NRF2401_RF_CH;       		/*!< Specifies the NRF2401 RF Channel Frequency.
                                        	This parameter can be a value of @ref NRF2401_RF_Channel_Direction */

  uint8_t NRF2401_Active_Mode;         /*!< Specifies the NRF2401 Active Mode.
                                        	This parameter can be a value of @ref NRF2401_Active_Mode */

  SPI_TypeDef* NRF2401_SPIx;			/*!< Specifies the NRF2401 SPIx.
                                        	This parameter can be a value of @ref Peripheral_declaration */

  GPIO_TypeDef* NRF2401_CE_GPIOx;		/*!< Specifies the NRF2401 CE GPIOx.
                                        	This parameter can be a value of @ref Peripheral_declaration */

  uint16_t NRF2401_CE_GPIO_PinSource;	/*!< Specifies the NRF2401 CE PinSource.
                                        	This parameter can be a value of @ref GPIO_Pin_sources */

  GPIO_TypeDef* NRF2401_CS_GPIOx;		/*!< Specifies the NRF2401 CS GPIOx.
                                        	This parameter can be a value of @ref Peripheral_declaration */

  uint16_t NRF2401_CS_GPIO_PinSource;	/*!< Specifies the NRF2401 CS PinSource.
                                        	This parameter can be a value of @ref GPIO_Pin_sources */

  GPIO_TypeDef* NRF2401_PWR_GPIOx;		/*!< Specifies the NRF2401 CS GPIOx.
                                        	This parameter can be a value of @ref Peripheral_declaration */

  uint16_t NRF2401_PWR_GPIO_PinSource;	/*!< Specifies the NRF2401 CS PinSource.
                                        	This parameter can be a value of @ref GPIO_Pin_sources*/
}NRF2401_InitTypeDef;

/** @defgroup NRF2401_Reserved_for_testing
  * @{
  */

#define NRF2401_RESERVED_1 						((uint8_t)0x8E) //1000 1110
#define NRF2401_RESERVED_2 						((uint8_t)0x08) //0000 1000
#define NRF2401_RESERVED_3 						((uint8_t)0x07) //0001 11

/** @defgroup NRF2401_PLL_CTRL
* @{
*/

#define NRF2401_PLL_CTRL_DEFAULT 				((uint8_t)0x00) //00
#define NRF2401_PLL_CTRL_OPEN_TX_CLOSED_RX 		((uint8_t)0x00) //00
#define NRF2401_PLL_CTRL_OPEN_TX_OPEN_RX 		((uint8_t)0x01) //01
#define NRF2401_PLL_CTRL_CLOSED_TX_CLOSED_RX 	((uint8_t)0x02) //10
#define NRF2401_PLL_CTRL_CLOSED_TX_OPEN_RX 		((uint8_t)0x03) //11

/** @defgroup NRF2401_Data_Length_for_Channel2
* @{
*/

#define NRF2401_DATA2_W_DEFAULT					((uint8_t)0x20) //0010 0000
#define NRF2401_DATA2_W_1_BYTE					((uint8_t)0x08) //0000 1000
#define NRF2401_DATA2_W_2_BYTE					((uint8_t)0x10) //0001 0000
#define NRF2401_DATA2_W_4_BYTE					((uint8_t)0x20) //0010 0000
#define NRF2401_DATA2_W_8_BYTE					((uint8_t)0x40) //0100 0000
#define NRF2401_DATA2_W_12_BYTE					((uint8_t)0x60) //0110 0000
#define NRF2401_DATA2_W_16_BYTE					((uint8_t)0x80) //1000 0000
#define NRF2401_DATA2_W_20_BYTE					((uint8_t)0xA0) //1010 0000
#define NRF2401_DATA2_W_24_BYTE					((uint8_t)0xC0) //1100 0000

/** @defgroup NRF2401_Data_Length_for_Channel1
* @{
*/

#define NRF2401_DATA1_W_DEFAULT					((uint8_t)0x20) //0010 0000
#define NRF2401_DATA1_W_1_BYTE					((uint8_t)0x08) //0000 1000
#define NRF2401_DATA1_W_2_BYTE					((uint8_t)0x10) //0001 0000
#define NRF2401_DATA1_W_4_BYTE					((uint8_t)0x20) //0010 0000
#define NRF2401_DATA1_W_8_BYTE					((uint8_t)0x40) //0100 0000
#define NRF2401_DATA1_W_16_BYTE					((uint8_t)0x80) //1000 0000
#define NRF2401_DATA1_W_20_BYTE					((uint8_t)0xA0) //1010 0000
#define NRF2401_DATA1_W_24_BYTE					((uint8_t)0xC0) //1100 0000

/** @defgroup NRF2401_Channel_Address_for_Channel2
* @{
*/

#define NRF2401_ADDR2_DEFAULT					((uint8_t)0xE7) //1110 0111

/** @defgroup NRF2401_Channel_Address_for_Channel1
* @{
*/

#define NRF2401_ADDR1_DEFAULT					((uint8_t)0xE7) //1110 0111

/** @defgroup NRF2401_Channel_Address_Width_for_Both_Channels
* @{
*/

#define NRF2401_ADDR_W_DEFAULT					((uint8_t)0x08) //0010 00
#define NRF2401_ADDR_W_1_BYTE					((uint8_t)0x08) //0010 00
#define NRF2401_ADDR_W_2_BYTE					((uint8_t)0x10) //0100 00
#define NRF2401_ADDR_W_3_BYTE					((uint8_t)0x18) //0110 00
#define NRF2401_ADDR_W_4_BYTE					((uint8_t)0x20) //1000 00
#define NRF2401_ADDR_W_5_BYTE					((uint8_t)0x28) //1010 00

#define NRF2401_ADDR_W_5_BITS					((uint8_t)0x05) //0001 01
#define NRF2401_ADDR_W_10_BITS					((uint8_t)0x0A) //0010 10
#define NRF2401_ADDR_W_15_BITS					((uint8_t)0x0F) //0011 11
#define NRF2401_ADDR_W_20_BITS					((uint8_t)0x14) //0101 00
#define NRF2401_ADDR_W_25_BITS					((uint8_t)0x19) //0110 01
#define NRF2401_ADDR_W_30_BITS					((uint8_t)0x1E) //0111 10
#define NRF2401_ADDR_W_38_BITS					((uint8_t)0x26) //0101 10

/** @defgroup NRF2401_Cyclic_Redundancy_Check_LENGTH
* @{
*/

#define NRF2401_CRC_LENGTH_8_BITS				((uint8_t)0x00) //0
#define NRF2401_CRC_LENGTH_16_BITS				((uint8_t)0x01) //1

/** @defgroup NRF2401_Cyclic_Redundancy_Check
* @{
*/

#define NRF2401_CRC_DISABLED					((uint8_t)0x00) //0
#define NRF2401_CRC_ENABLED						((uint8_t)0x01) //1

/** @defgroup NRF2401_2_Channel_Receive_Mode
* @{
*/

#define NRF2401_RX2_DISABLED					((uint8_t)0x00) //0
#define NRF2401_RX2_ENABLED						((uint8_t)0x01) //1

/** @defgroup NRF2401_Communication_Mode
* @{
*/

#define NRF2401_CM_DIRECT_MODE					((uint8_t)0x00) //0
#define NRF2401_CM_SHOCKBURST_MODE				((uint8_t)0x01) //1

/** @defgroup NRF2401_RF_Data_Rate
* @{
*/

#define NRF2401_RFDR_SB_250_KBPS				((uint8_t)0x00) //0
#define NRF2401_RFDR_SB_1_MBPS					((uint8_t)0x01) //1

/** @defgroup NRF2401_Crystal_Frequency
* @{
*/

#define NRF2401_XO_F_DEFAULT					((uint8_t)0x03) //011
#define NRF2401_XO_F_4_MHZ						((uint8_t)0x00) //000
#define NRF2401_XO_F_8_MHZ						((uint8_t)0x01) //001
#define NRF2401_XO_F_12_MHZ						((uint8_t)0x02) //010
#define NRF2401_XO_F_16_MHZ						((uint8_t)0x03) //011
#define NRF2401_XO_F_20_MHZ						((uint8_t)0x04) //100

/** @defgroup NRF2401_RF_Output_Power
* @{
*/

#define NRF2401_RF_PWR_DEFAULT					((uint8_t)0x03) //11
#define NRF2401_RF_PWR_MINUS_20_DB				((uint8_t)0x00) //00
#define NRF2401_RF_PWR_MINUS_10_DB				((uint8_t)0x01) //01
#define NRF2401_RF_PWR_MINUS_5_DB				((uint8_t)0x02) //10
#define NRF2401_RF_PWR_MINUS_0_DB				((uint8_t)0x03) //11

/** @defgroup NRF2401_RF_Channel_Direction
* @{
*/

#define NRF2401_RF_CH_DEFAULT					((uint8_t)0x02) //0000 010
#define NRF2401_RF_CH_2400_MHZ					((uint8_t)0x00) //0000 000
#define NRF2401_RF_CH_2401_MHZ					((uint8_t)0x01) //0000 001
#define NRF2401_RF_CH_2402_MHZ					((uint8_t)0x02) //0000 010
#define NRF2401_RF_CH_2404_MHZ					((uint8_t)0x04) //0000 100
#define NRF2401_RF_CH_2408_MHZ					((uint8_t)0x08) //0001 000
#define NRF2401_RF_CH_2416_MHZ					((uint8_t)0x10) //0010 000
#define NRF2401_RF_CH_2432_MHZ					((uint8_t)0x20) //0100 000
#define NRF2401_RF_CH_2464_MHZ					((uint8_t)0x40) //1000 000

/** @defgroup NRF2401_Active_Mode
* @{
*/

#define NRF2401_TX_MODE							((uint8_t)0x00) //0
#define NRF2401_RX_MODE							((uint8_t)0x01) //1

void Delay_us(uint32_t nCnt_1us);

/* Initialization and Configuration functions *********************************/
void NRF2401_Init(NRF2401_InitTypeDef* NRF2401_InitStruct);
// void I2S_Init(SPI_TypeDef* SPIx, I2S_InitTypeDef* I2S_InitStruct);
// void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct);
// void I2S_StructInit(I2S_InitTypeDef* I2S_InitStruct);
// void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState);
// void I2S_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState);
// void SPI_DataSizeConfig(SPI_TypeDef* SPIx, uint16_t SPI_DataSize);
// void SPI_BiDirectionalLineConfig(SPI_TypeDef* SPIx, uint16_t SPI_Direction);
// void SPI_NSSInternalSoftwareConfig(SPI_TypeDef* SPIx, uint16_t SPI_NSSInternalSoft);
// void SPI_SSOutputCmd(SPI_TypeDef* SPIx, FunctionalState NewState);
// void SPI_TIModeCmd(SPI_TypeDef* SPIx, FunctionalState NewState);

// void I2S_FullDuplexConfig(SPI_TypeDef* I2Sxext, I2S_InitTypeDef* I2S_InitStruct);

// /* Data transfers functions ***************************************************/ 
void NRF2401_SendData_1CH(uint8_t* Data);
uint8_t* NRF2401_ReceiveData_1CH(void);

// /* Hardware CRC Calculation functions *****************************************/
// void SPI_CalculateCRC(SPI_TypeDef* SPIx, FunctionalState NewState);
// void SPI_TransmitCRC(SPI_TypeDef* SPIx);
// uint16_t SPI_GetCRC(SPI_TypeDef* SPIx, uint8_t SPI_CRC);
// uint16_t SPI_GetCRCPolynomial(SPI_TypeDef* SPIx);

// /* DMA transfers management functions *****************************************/
// void SPI_I2S_DMACmd(SPI_TypeDef* SPIx, uint16_t SPI_I2S_DMAReq, FunctionalState NewState);

// /* Interrupts and flags management functions **********************************/
// void SPI_I2S_ITConfig(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT, FunctionalState NewState);
// FlagStatus SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG);
// void SPI_I2S_ClearFlag(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG);
// ITStatus SPI_I2S_GetITStatus(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT);
// void SPI_I2S_ClearITPendingBit(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT);

#endif /* __NRF2401_H */