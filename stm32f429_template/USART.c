#include "USART.h"

void USART3_Initialization(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    /*-------------------------- GPIO Configuration ----------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Connect USART pins to AF */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    USART_InitTypeDef USART_InitStructure;

    /* USARTx configuration ------------------------------------------------------*/
    /* USARTx configured as follow:
     *  - BaudRate = 57600 baud
     *  - Word Length = 8 Bits
     *  - One Stop Bit
     *  - No parity
     *  - Hardware flow control disabled (RTS and CTS signals)
     *  - Receive and transmit enabled
     */
    USART_InitStructure.USART_BaudRate = 57600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    //START USART
    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);

    //Using Interrupt
    USART_ClearFlag(USART3, USART_FLAG_TC);

    //Enable RX interrupt & Disable TX interrupt
    USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    /* NVIC Initialization */
    NVIC_InitTypeDef NVIC_InitStruct = {
      .NVIC_IRQChannel = USART3_IRQn,
      .NVIC_IRQChannelPreemptionPriority = 0,
      .NVIC_IRQChannelSubPriority = 0,
      .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&NVIC_InitStruct);
}

void USART3_puts(char* s)
{
    while(*s) {
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        USART_SendData(USART3, *s);
        s++;
    }
}

void USART3_IRQHandler(void)
{    
  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) { //==SET
    
  }
}

