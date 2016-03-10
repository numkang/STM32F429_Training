
#include "main.h"

//White - TX
//Green - RX

void RCC_Configuration(void)
{
      /* --------------------------- System Clocks Configuration -----------------*/
      /* USART1 clock enable */
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
      /* GPIOA clock enable */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
}
 
/**************************************************************************************/
 
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*-------------------------- GPIO Configuration ----------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Connect USART pins to AF */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);   // USART1_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  // USART1_RX
}
 
/**************************************************************************************/
 
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

void LED3_Toggle(void){

  //GPIOG->ODR ^= GPIO_Pin_13;
  GPIO_ToggleBits(GPIOG,GPIO_Pin_13);

}

void LED4_On(void){

  GPIO_SetBits(GPIOG,GPIO_Pin_14);

}

void LED4_Off(void){

  GPIO_ResetBits(GPIOG,GPIO_Pin_14);

}

void LED4_Toggle(void){

  //GPIOG->ODR ^= GPIO_Pin_13;
  GPIO_ToggleBits(GPIOG,GPIO_Pin_14);

}

void USART1_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;

    /* USARTx configuration ------------------------------------------------------*/
    /* USARTx configured as follow:
     *  - BaudRate = 57600 baud
     *  - Word Length = 8 Bits
     *  - One Stop Bit
     *  - No parity
     *  - Hardware flow control disabled (RTS and CTS signals)
     *  - Receive and transmit enabled
     */
    USART_InitStructure.USART_BaudRate = 57600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    //START USART
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);

    //Using Interrupt
    USART_ClearFlag(USART1, USART_FLAG_TC);

    //Enable RX interrupt & Disable TX interrupt
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /* NVIC Initialization */
    NVIC_InitTypeDef NVIC_InitStruct = {
      .NVIC_IRQChannel = USART1_IRQn,
      .NVIC_IRQChannelPreemptionPriority = 0,
      .NVIC_IRQChannelSubPriority = 0,
      .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&NVIC_InitStruct);

}

void USART1_puts(char* s)
{
    while(*s) {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, *s);
        s++;
    }
}

/**************************************************************************************/
int main(void)
{
    RCC_Configuration();
    GPIO_Configuration();
    USART1_Configuration();
    LED_Initialization();
    USART1_puts("Hello World!\r\n");
    USART1_puts("Just for STM32F429I Discovery verify USART1 with USB TTL Cable\r\n");

    while(1)
    {
        //LED4_Toggle();

        //Delay_1us(10000);
        //USART1_puts(i);        
    }
}


void USART1_IRQHandler(void)
{    
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) { //==SET
    uart1_data = USART_ReceiveData(USART1);      
  
  if(i == '0'){
    if (uart1_data =='a') i = uart1_data;
    else if(uart1_data =='d') i = uart1_data;
    else i = '0';
  }
  else if(i == 'a'){
    i = (uart1_data =='b') ? uart1_data : '0';
  }
  else if(i == 'b'){
    i = (uart1_data =='c') ? uart1_data : '0';
  }
  else if(i == 'c'){
    if (uart1_data =='3') i = uart1_data;
    else if(uart1_data =='4') i = uart1_data;
    else i = '0';
  }
  else if(i == '3'){
    if(uart1_data =='o') LED3_On();
    else if(uart1_data =='f') LED3_Off();
    i = '0';
  }
  else if(i == '4'){
    if(uart1_data =='o') LED4_On();
    else if(uart1_data =='f') LED4_Off();
    i = '0';
  }
  else if(i == 'd'){
    i = (uart1_data =='e') ? uart1_data : '0';
  }
  else if(i == 'e'){
    i = (uart1_data =='f') ? uart1_data : '0';
  }
  else if(i == 'f'){
    if(uart1_data =='3') LED3_Toggle();
    else if(uart1_data =='4') LED4_Toggle();
    i = '0';
  }

  USART_SendData(USART1, i);
}

}