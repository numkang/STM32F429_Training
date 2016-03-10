#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"
#include "LED.h"
#include "main.h"
#include "STRCMD.h"

void USART3_Configuration(void);
void USART3_puts(char* s);

#endif