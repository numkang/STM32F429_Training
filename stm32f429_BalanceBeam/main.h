
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "LED.h"
#include "IMU.h"
#include "Filter.h"
#include "USART.h"
#include "PWM.h"
#include "PID.h"
#include "STRCMD.h"

#define GYRO_DATA_REGISTER 0x43
#define ACC_DATA_REGISTER 0x3B

#define ROLL 0
#define PITCH 1
#define YAW 2

void RCC_Configuration(void);
void PushButton_Initialization(void);
uint8_t PushButton_Read(void);
static inline void Delay_1us(uint32_t);
void Timer2_Initialization(void);
void PRINT_USART(void);
void Arm_Func(void);
void Disarm_Func(void);
extern int16_t gyroRaw[3];

struct flag{
  uint8_t imu;
  uint8_t filter;
  uint8_t print;
  uint8_t usart;
  uint8_t gyroCalibrated;
  uint8_t arm;
};

extern struct flag f;

#endif /* __MAIN_H */


