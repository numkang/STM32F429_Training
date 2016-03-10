// IMU.h
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IMU_H
#define __IMU_H

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "terminal.h"
#include <stdio.h>
#include "lcd.h"
#include "main.h"

void SPI_Initialization(void);
uint8_t readSPI4(uint16_t addr);
uint8_t writeSPI4(uint8_t addr, uint8_t data);
void readIMU(int16_t *raw, uint8_t reg);
void Timer4_Initialization(void);
void gyro_remove_offset(int16_t *gRaw);

#endif