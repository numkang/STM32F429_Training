#ifndef __STRCMD_H
#define __STRCMD_H

#include "stm32f4xx.h"
#include "LED.h"
#include "USART.h"
#include "PID.h"

#define USART_BUFFER 20
#define Led_cmd 0
#define ArmDisarm_cmd 1
#define Gain_cmd 2
#define Setpoint_cmd 3

void strcmd_main(void);

void set_strcmd_flag(void);
void reset_strcmd_flag(void);
uint8_t get_strcmd_flag(void);
void set_invalid_cmd_flag(void);
void reset_invalid_cmd_flag(void);
uint8_t get_invalid_cmd_flag(void);
void set_invalid_digit_flag(void);
void reset_invalid_digit_flag(void);
uint8_t get_invalid_digit_flag(void);

void get_USART_data(USART_TypeDef* USARTx);
void return_captured_char(uint8_t *data);
void command_print(uint8_t *data);
void get_number(uint8_t *data);
void return_val(float *data);
void compare_func(uint8_t *data);
uint8_t _sizeof(uint8_t i);

#endif