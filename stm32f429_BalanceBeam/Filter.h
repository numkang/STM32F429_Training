// Filter.h
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FILTER_H
#define __FILTER_H

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "main.h"
#include "LED.h"

#define ComplementAlpha 0.998
#define LowPassAlpha 0.0

struct axis_t{
  float X, Y, Z;
};

struct vect_t{
    struct axis_t G, A, GA;
};

extern struct vect_t EstV;
extern float sqX_sqZ, sqY_sqZ;

void Timer5_Initialization(void);
void Filter(int16_t gRaw[3], float *gAng, int16_t aRaw[3], float *aAng, float *Ang);

#endif