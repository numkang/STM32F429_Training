
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

 #include "stm32f4xx.h"

uint8_t uart1_data;
uint8_t i = '0';

void RCC_Configuration(void);
void GPIO_Configuration(void);
void LED_Initialization(void);
void LED3_On(void);
void LED3_Off(void);
void LED3_Toggle(void);
void LED4_On(void);
void LED4_Off(void);
void LED4_Toggle(void);
void USART1_Configuration(void);
void USART1_puts(char* s);

static inline void Delay_1us(uint32_t);
static inline void Delay_1us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 45; nCnt != 0; nCnt--);
}

#endif /* __MAIN_H */

