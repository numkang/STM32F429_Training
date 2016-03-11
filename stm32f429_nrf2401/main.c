#include "main.h"

static inline void Delay_1us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 13; nCnt != 0; nCnt--);
}

void RCC_Configuration(void)
{
      /* --------------------------- System Clocks Configuration -----------------*/
      /* GPIOx clock enable */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //USART3
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE); //LED
      /* USARTx clock enable */
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
}

/**************************************************************************************/
int main(void)
{
    RCC_Configuration();
    LED_Initialization();    //PG13 (GREEN) & PG14 (RED)
    USART3_Initialization(); //PB10 (TX) & PB11 (RX)
    
    USART3_puts("\r\nHellp World\r\n");
    
    while(1)
    {
        LED3_On();
        Delay_1us(50000);
        LED3_Off();
        Delay_1us(50000);

    }

    while(1); // Don't want to exit
}

