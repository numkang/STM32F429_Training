// PID.h
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PID_H
#define __PID_H

#include "stm32f4xx.h"
#include "main.h"
#include "STRCMD.h"

void PID_control(float ang[3]);
void set_Kp_Func(void);
void set_Ki_Func(void);
void set_Kd_Func(void);
void set_setpoint_Func(void);

extern float errorI, errorD, previous_error;

#endif