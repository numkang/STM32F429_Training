#include "STRCMD.h"

static strcmd_flag = 0;
static invalid_cmd_flag = 0;
static invalid_digit_flag = 0;

uint8_t uart_buff[USART_BUFFER];
uint8_t buff = 0;

uint8_t uart_data[USART_BUFFER];
char text[100];

typedef char (*CMD_t)(void); // (*CMD_t[])[]
typedef void (*FUNC_t)(void);
typedef FUNC_t (*(*FUNCS_t))(void);

void set_strcmd_flag(void){
	strcmd_flag = 1;
}

void reset_strcmd_flag(void){
	strcmd_flag = 0;
}

uint8_t get_strcmd_flag(void){
	return strcmd_flag;
}

void set_invalid_cmd_flag(void){
	invalid_cmd_flag = 1;
}

void reset_invalid_cmd_flag(void){
	invalid_cmd_flag = 0;
}

uint8_t get_invalid_cmd_flag(void){
	return invalid_cmd_flag;
}

void set_invalid_digit_flag(void){
	invalid_digit_flag = 1;
}

void reset_invalid_digit_flag(void){
	invalid_digit_flag = 0;
}

uint8_t get_invalid_digit_flag(void){
	return invalid_digit_flag;
}

///////////////////////////////////////

////*          LED          *////
const char *led_command[] = {
	"led on",
	"led off",
	"led toggle"
};

FUNC_t led_func[] = {
	LED4_On,
	LED4_Off,
	LED4_Toggle,
};

////*          Arm / Disarm          *////
const char *armdisarm_command[] = {
	"arm",
	"disarm"
};

FUNC_t armdisarm_func[] = {
	Arm_Func,
	Disarm_Func
};

////*          Gain PID          *////
const char *gain_command[] = {
	"set kp ",
	"set ki ",
	"set kd "
};

FUNC_t gain_func[] = {
	set_Kp_Func,
	set_Ki_Func,
	set_Kd_Func
};

////*          SetPoint          *////
const char *setPoint_command[] = {
	"set setpoint "
};

FUNC_t setPoint_func[] = {
	set_setpoint_Func
};

///////////////////////////////////////

CMD_t *commands[] = {
	led_command,
	armdisarm_command,
	gain_command,
	setPoint_command
};

FUNCS_t functions[] = {
	led_func,
	armdisarm_func,
	gain_func,
	setPoint_func
};

///////////////////////////////////////

uint8_t size_commands = sizeof(commands) / sizeof(*commands);
uint8_t ii, jj;
int16_t compare_result; 
float val = 0.0;

void get_USART_data(USART_TypeDef* USARTx){
	uart_buff[buff] = USART_ReceiveData(USARTx);
    if(uart_buff[buff++] == 0x0A){
    	set_strcmd_flag();
    	buff = 0;
 	}
}

void return_captured_char(uint8_t *data)
{
	uint8_t i = 0;
	while(uart_buff[i] != 0x0A){
		data[i] = uart_buff[i];
		i++;
	}
	while(data[i]){
		data[i] = 0;
		i++;
	}
}

void command_print(uint8_t *data){
	sprintf(text,"Command Input: ");
  	USART3_puts(text);
  	do{
		sprintf(text,"%c", *data);
  		USART3_puts(text);
  	}while(*data++);
  	sprintf(text,"\r\n");
  	USART3_puts(text);
}

void get_number(uint8_t *data)
{
	while(!(isdigit(*data) || *data == '-' || *data == '+' || *data == '.')){
  		data++;
  	} //skip the alphabet, but reserve negative/positive sign and point

  	float rez = 0, fact = 1;
  	if (*data == '-'){ //check if negative or positive value
  		*data = NULL;
    	data++;
    	fact = -1;
  	}
  	else if(*data == '+'){
  		*data = NULL;
    	data++;
    	fact = +1;
  	} 

  	uint8_t *ptr;
  	ptr = data;
  	reset_invalid_digit_flag();
  	uint8_t isPoint = 0;

  	for(; *ptr; ptr++){  		
  		if(isdigit(*ptr)){}
  		else if(*ptr == '.' && isPoint == 0) isPoint = 1; //must have only one point
  		else set_invalid_digit_flag();
  	} //check whether the remainings are number

	if(!get_invalid_digit_flag()){
  		for (isPoint = 0; *data; data++){
   	  		if (*data == '.'){
  	    		isPoint = 1;
    		};

    		int d = *data - '0';
    		if (d >= 0 && d <= 9){
      			if (isPoint) fact /= 10.0f;
      			rez = rez * 10.0f + (float)d;
    		};
    		*data = NULL;
  		};
  		val = rez * fact;
	}
}

void return_val(float *data){
	if(!get_invalid_digit_flag()){
		
		if(ii == Gain_cmd){
			*data = val;
			sprintf(text,"The gain is set to %.4f\r\n", *data);
		}
		else if (ii == Setpoint_cmd){
			if(val >= -90.0 && val <= 90.0){
				*data = val;
				sprintf(text,"The setpoint is set to %.4f\r\n", *data);
			}
			else{
				sprintf(text,"The setpoint must not be exceeded +-90 degree\r\n");
			}
		} 
		USART3_puts(text);
	}
	else{
		sprintf(text,"The number is Invalid\r\n");
		USART3_puts(text);
	}
}

void compare_func(uint8_t *data){
	reset_invalid_cmd_flag();
		
	uint8_t i,j;
	for(i = 0; i < size_commands; i++)
	{
		for (j = 0; j <= _sizeof(i); j++)
		{
			compare_result = strcmp(data, commands[i][j]);
			if(compare_result == 0) goto ValidCommand;
		}
	}
	sprintf(text,"Command Output: Invalid Command\r\n");
	USART3_puts(text);
	set_invalid_cmd_flag();

	ValidCommand:
	// sprintf(text,"%s %d %d %d\r\n", commands[i][j], i, j, compare_result);

	if(!get_invalid_cmd_flag()){
		sprintf(text,"Command Output: %s\r\n", commands[i][j]);
		USART3_puts(text);
		ii = i;
		jj = j;
		functions[i][j]();
	}
}

uint8_t _sizeof(uint8_t a){
	uint8_t b = 0;
	while(strncmp(commands[a][b], "a", 1) >= 0 && strncmp(commands[a][b], "a", 1) <= 25){
		b++;
	}
	return b;
}

void strcmd_main(void){
	if(get_strcmd_flag()){
		return_captured_char(uart_data);
		command_print(uart_data);
		get_number(uart_data);
		compare_func(uart_data);	

		sprintf(text,"\r\n");
    	USART3_puts(text);
    	reset_strcmd_flag();
    }  
}