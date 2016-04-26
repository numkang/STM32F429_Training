
// NRF2401.h
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NRF24L01_H
#define __NRF24L01_H

#include "stm32f4xx.h"
#include "USART.h"
#include "main.h"
#include "LED.h"

typedef struct
{
  uint8_t NRF24L01_CRC_Length;          /*!< Specifies the NRF2401 CRC length to be calculated by nRF2401 in ShockBurst™.
                                          This parameter can be a value of @ref NRF2401_Cyclic_Redundancy_Check_LENGTH */

  uint8_t NRF24L01_PWR;        	/*!< Specifies the NRF2401 controlling the setting of the PLL for test purposes.
                                        	This parameter can be a value of @ref NRF2401_PLL_CTRL */

  uint8_t NRF24L01_Mode;         /*!< Specifies the NRF2401 Active Mode.
                                          This parameter can be a value of @ref NRF2401_Active_Mode */

  uint8_t NRF24L01_ADDR_Width;         /*!< Specifies the NRF2401 Number of bits reserved for RX address in ShockBurst™ packages.
                                          This parameter can be a value of @ref NRF2401_Channel_Address_Width_for_Both_Channels */

  uint8_t NRF24L01_RF_CH;          /*!< Specifies the NRF2401 RF Channel Frequency.
                                          This parameter can be a value of @ref NRF2401_RF_Channel_Direction */

  uint8_t NRF24L01_Data_Rate;        /*!< Specifies the NRF2401 Data Rate.
                                          This parameter can be a value of @ref NRF2401_RF_Data_Rate */

  char* NRF24L01_ADDR_P0;                /*!< Specifies the NRF2401 Receiver address channel 1, up to 40 bit.
                                          This parameter can be a value of @ref NRF2401_Channel_Address_for_Channel1 */

  uint8_t NRF24L01_DataSize_P0;        /*!< Specifies the NRF2401 Length of RF package payload section for receive-channel 1.
                                          This parameter can be a value of @ref NRF2401_Data_Length_for_Channel1 */

  SPI_TypeDef* NRF24L01_SPIx;			/*!< Specifies the NRF2401 SPIx.
                                        	This parameter can be a value of @ref Peripheral_declaration */

  GPIO_TypeDef* NRF24L01_CE_GPIOx;		/*!< Specifies the NRF2401 CE GPIOx.
                                        	This parameter can be a value of @ref Peripheral_declaration */

  uint16_t NRF24L01_CE_GPIO_PinSource;	/*!< Specifies the NRF2401 CE PinSource.
                                        	This parameter can be a value of @ref GPIO_Pin_sources */

  GPIO_TypeDef* NRF24L01_CSN_GPIOx;		/*!< Specifies the NRF2401 CS GPIOx.
                                        	This parameter can be a value of @ref Peripheral_declaration */

  uint16_t NRF24L01_CSN_GPIO_PinSource;	/*!< Specifies the NRF2401 CS PinSource.
                                        	This parameter can be a value of @ref GPIO_Pin_sources */

  GPIO_TypeDef* NRF24L01_IQR_GPIOx;   /*!< Specifies the NRF2401 CS GPIOx.
                                          This parameter can be a value of @ref Peripheral_declaration */

  uint16_t NRF24L01_IQR_GPIO_PinSource; /*!< Specifies the NRF2401 CS PinSource.
                                          This parameter can be a value of @ref GPIO_Pin_sources */

} NRF24L01_InitTypeDef;

/** @defgroup NRF24L01_Cyclic_Redundancy_Check_LENGTH
* @{
*/

#define NRF24L01_CRC_LENGTH_1_BYTES                   ((uint8_t)0x00) //0000 0000
#define NRF24L01_CRC_LENGTH_2_BYTES                   ((uint8_t)0x04) //0000 0100

/** @defgroup NRF24L01_PWR
  * @{
  */

#define NRF24L01_PWR_UP 					                    ((uint8_t)0x02) //0000 0010
#define NRF24L01_PWR_DOWN 						                ((uint8_t)0x00) //0000 0000

/** @defgroup NRF24L01_PRIM_RX
* @{
*/

#define NRF24L01_PTX_MODE                             ((uint8_t)0x00) //0000 0000
#define NRF24L01_PRX_MODE                             ((uint8_t)0x01) //0000 0001

/** @defgroup NRF24L01_Address_Width
* @{
*/

#define NRF24L01_ADDR_W_3_BYTE                        ((uint8_t)0x01) //0000 0001
#define NRF24L01_ADDR_W_4_BYTE                        ((uint8_t)0x02) //0000 0010
#define NRF24L01_ADDR_W_5_BYTE                        ((uint8_t)0x03) //0000 0011

/** @defgroup NRF24L01_RF_Channel_Frequency (0-125)
* @{
*/

#define NRF24L01_RF_CH_DEFAULT                        ((uint8_t)0x02) //0000 0010

/** @defgroup NRF24L01_RF_Data_Rate
* @{
*/

#define NRF24L01_RF_DR_SB_1_MBPS                      ((uint8_t)0x00) //0000 0000
#define NRF24L01_RF_DR_SB_2_MBPS                      ((uint8_t)0x08) //0000 1000

/** @defgroup NRF24L01_STATUS_Clear_Bits
* @{
*/

#define NRF24L01_STATUS_Clear_Bits                    ((uint8_t)0x70) //0111 0000

/** @defgroup NRF24L01_ADDR_Data_Pipe
* @{
*/

#define NRF24L01_RX_ADDR_P0_DEFAULT                   ((uint64_t)0xE7E7E7E7E7)
#define NRF24L01_RX_ADDR_P1_DEFAULT                   ((uint64_t)0xC2C2C2C2C2)
#define NRF24L01_RX_ADDR_P2_DEFAULT                   ((uint8_t)0xC3)
#define NRF24L01_RX_ADDR_P3_DEFAULT                   ((uint8_t)0xC4)
#define NRF24L01_RX_ADDR_P4_DEFAULT                   ((uint8_t)0xC5)
#define NRF24L01_RX_ADDR_P5_DEFAULT                   ((uint8_t)0xC6)
#define NRF24L01_TX_ADDR_DEFAULT                      ((uint64_t)0xE7E7E7E7E7)

/** @defgroup NRF24L01_Data_Length (1-32)
* @{
*/

#define NRF24L01_DATA_W_DEFAULT                       ((uint8_t)0x20) //0010 0000

/** ---------------------------- SPI Command ---------------------------- **/

#define NRF24L01_R_REGISTER                           ((uint8_t)0x00) //0000 0000
#define NRF24L01_W_REGISTER                           ((uint8_t)0x20) //0010 0000
#define NRF24L01_R_RX_PAYLOAD                         ((uint8_t)0x61) //0110 0001
#define NRF24L01_W_TX_PAYLOAD                         ((uint8_t)0xA0) //1010 0000
#define NRF24L01_FLUSH_TX                             ((uint8_t)0xE1) //1110 0001
#define NRF24L01_FLUSH_RX                             ((uint8_t)0xE2) //1110 0010
#define NRF24L01_REUSE_TX_PL                          ((uint8_t)0xE3) //1110 0011
#define NRF24L01_ACTIVATE                             ((uint8_t)0x50) //0101 0000
#define NRF24L01_R_RX_PL_WID                          ((uint8_t)0x60) //0110 0000
#define NRF24L01_W_ACK_PAYLOAD                        ((uint8_t)0xA8) //1010 1000
#define NRF24L01_W_TX_PAYLOAD_NOACK                   ((uint8_t)0xB0) //1011 0000
#define NRF24L01_NOP                                  ((uint8_t)0xFF) //1111 1111

/** -------------------------- Register Address ------------------------- **/

#define NRF24L01_CONFIG                               ((uint8_t)0x00)
#define NRF24L01_EN_AA                                ((uint8_t)0x01)
#define NRF24L01_EN_RXADDR                            ((uint8_t)0x02)
#define NRF24L01_SETUP_AW                             ((uint8_t)0x03)
#define NRF24L01_SETUP_RETR                           ((uint8_t)0x04)
#define NRF24L01_RF_CH_REG                            ((uint8_t)0x05)
#define NRF24L01_RF_SETUP                             ((uint8_t)0x06)
#define NRF24L01_STATUS                               ((uint8_t)0x07)
#define NRF24L01_OBSERVE_TX                           ((uint8_t)0x08)
#define NRF24L01_CD                                   ((uint8_t)0x09)
#define NRF24L01_RX_ADDR_P0                           ((uint8_t)0x0A)
#define NRF24L01_RX_ADDR_P1                           ((uint8_t)0x0B)
#define NRF24L01_RX_ADDR_P2                           ((uint8_t)0x0C)
#define NRF24L01_RX_ADDR_P3                           ((uint8_t)0x0D)
#define NRF24L01_RX_ADDR_P4                           ((uint8_t)0x0E)
#define NRF24L01_RX_ADDR_P5                           ((uint8_t)0x0F)
#define NRF24L01_TX_ADDR                              ((uint8_t)0x10)
#define NRF24L01_RX_PW_P0                             ((uint8_t)0x11)
#define NRF24L01_RX_PW_P1                             ((uint8_t)0x12)
#define NRF24L01_RX_PW_P2                             ((uint8_t)0x13)
#define NRF24L01_RX_PW_P3                             ((uint8_t)0x14)
#define NRF24L01_RX_PW_P4                             ((uint8_t)0x15)
#define NRF24L01_RX_PW_P5                             ((uint8_t)0x16)
#define NRF24L01_FIFO_STATUS                          ((uint8_t)0x17)
#define NRF24L01_DYNPD                                ((uint8_t)0x1C)
#define NRF24L01_FEATURE                              ((uint8_t)0x1D)

#define NRF24L01_CONFIG_RESET_VALUE                   ((uint8_t)0x08) //0000 1000
#define NRF24L01_EN_AA_RESET_VALUE                    ((uint8_t)0x3F) //0011 1111
#define NRF24L01_EN_RXADDR_RESET_VALUE                ((uint8_t)0x03) //0000 0011
#define NRF24L01_SETUP_AW_RESET_VALUE                 ((uint8_t)0x03) //0000 0011
#define NRF24L01_SETUP_RETR_RESET_VALUE               ((uint8_t)0x03) //0000 0011
#define NRF24L01_RF_CH_RESET_VALUE                    ((uint8_t)0x02) //0000 0010
#define NRF24L01_RF_SETUP_RESET_VALUE                 ((uint8_t)0x07) //0000 0111
#define NRF24L01_STATUS_RESET_VALUE                   ((uint8_t)0x0E) //0000 1110
#define NRF24L01_OBSERVE_TX_RESET_VALUE               ((uint8_t)0x00) //0000 0000
#define NRF24L01_CD_RESET_VALUE                       ((uint8_t)0x00) //0000 0000
#define NRF24L01_FIFO_STATUS_RESET_VALUE              ((uint8_t)0x11) //0001 0001
#define NRF24L01_DYNPD_RESET_VALUE                    ((uint8_t)0x00) //0000 0000
#define NRF24L01_FEATURE_RESET_VALUE                  ((uint8_t)0x00) //0000 0000

void Delay_us(uint32_t nCnt_1us);
void NRF24L01_W_REG(uint8_t Reg, uint8_t Data);
void NRF24L01_R_REG(uint8_t REG);
void NRF24L01_W_ADDR(uint8_t REG, char *DAT);
void NRF24L01_R_ADDR(uint8_t REG);
void NRF24L01_Init(NRF24L01_InitTypeDef* NRF24L01_InitStruct);
void NRF24L01_ACTIVATE_FUNC();
void NRF24L01_SendData_NOACK(uint8_t* Data);
void NRF24L01_SendData_ACK(uint8_t* Data);

void NRF24L01_SendData(uint8_t* Data);
uint8_t* NRF24L01_ReceiveData(void);

#endif /* __NRF2401_H */