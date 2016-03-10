#include "LED.h"

void LED_Initialization(void){

  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE); //LED3/4 GPIO Port

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;  // LED is connected to PG13/PG14
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

}

void LED3_On(void){

  GPIO_SetBits(GPIOG,GPIO_Pin_13);

}

void LED3_Off(void){

  GPIO_ResetBits(GPIOG,GPIO_Pin_13);

}

void LED4_On(void){

  GPIO_SetBits(GPIOG,GPIO_Pin_14);

}

void LED4_Off(void){

  GPIO_ResetBits(GPIOG,GPIO_Pin_14);

}

void LED3_Toggle(void){


  GPIO_ToggleBits(GPIOG,GPIO_Pin_13);

}

void LED4_Toggle(void){


  GPIO_ToggleBits(GPIOG,GPIO_Pin_14);

}