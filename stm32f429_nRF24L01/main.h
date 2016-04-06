
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx.h"
#include "LED.h"
#include "USART.h"
#include "NRF24L01.h"

void RCC_Configuration(void);
static inline void Delay_1us(uint32_t);

#endif /* __MAIN_H */


