#include "main.h"

// char text_main[100];
// uint16_t receivedData = 0;

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
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //USART1
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //SPI1
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE); //LED
      /* USARTx clock enable */
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
      /* SPIx clock enable */
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
}

/**************************************************************************************/

void Timer2_Initialization(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM5 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel =  TIM2_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  /* -- Timer Configuration --------------------------------------------------- */
  TIM_DeInit(TIM2);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_TimeBaseStruct.TIM_Period = 25000;  //250ms  --> 4Hz
  TIM_TimeBaseStruct.TIM_Prescaler = 90 - 1; // Prescaled by 900 -> = 0.1M(10us)
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1; // Div by one -> 90 MHz (Now RCC_DCKCFGR_TIMPRE is configured to divide clock by two)
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);    // Set interrupt when timer reloads (overflow)
  TIM_ARRPreloadConfig(TIM2, DISABLE);       //Put ARR value into register
  TIM_Cmd(TIM2, ENABLE);
}

uint8_t task = 0;
void TIM2_IRQHandler()
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){

    task = 1;

    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}


int main(void)
{  
    RCC_Configuration();
    LED_Initialization();     //PG13 (GREEN) & PG14 (RED)
    USART1_Initialization();  //PA9 (TX) & PA10 (RX)
    SPI1_Initialization();    //PB3 (SCK) & PB4 (MISO) & PB5 (MOSI)
    Delay_1us(1000000);
    NRF24L01_Initialization();
    Timer2_Initialization();

    LED3_On();
    USART1_puts("\r\nHello World\r\n");
    Delay_1us(500000);
    LED3_Off();
    Delay_1us(500000);   

    char* data = "333";

    while(1)
    {
      if(task == 1){
        LED4_Toggle();

        /* NRF24L01 TX Mode */
        NRF24L01_SendData(data); 
        // NRF24L01_SendData_NOACK(data);       

        /* NRF24L01 RX Mode */
        // NRF24L01_ReceiveData();        

        /* NRF24L01 Read Register */
        // NRF24L01_R_REG(NRF24L01_CONFIG);
        // NRF24L01_R_ADDR(NRF24L01_TX_ADDR);        

        /* NRF24L01 change the sending data to check if MCU freeze */
        if(data == "333") data = "???";
        else if(data == "???") data = "333";

        task = 0;
      }
    }

    while(1); // Don't want to exit
}



