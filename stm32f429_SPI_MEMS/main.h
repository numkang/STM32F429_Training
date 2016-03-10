
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "terminal.h"
#include <stdio.h>
#include "lcd.h"


static inline void Delay_1us(uint32_t nCnt_1us);
uint8_t readSPI5(uint16_t addr);
uint8_t readGyro();
uint8_t writeSPI5(uint8_t addr, uint8_t data);

char lcd_text_main[100];
uint32_t runner=0;
uint16_t receivedData=0;
float sensitivity = 70/1000;
int16_t Gyro_Raw[3];

#endif /* __MAIN_H */


