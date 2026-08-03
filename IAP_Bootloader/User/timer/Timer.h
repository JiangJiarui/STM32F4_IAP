#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx.h"                  // Device header


#define User_Timer1             TIM6
#define User_Timer1_clk         RCC_APB1Periph_TIM6

#define User_Timer1_IRQ         TIM6_DAC_IRQn
#define User_Timer1_IRQHandler  TIM6_DAC_IRQHandler


/*  NVIC_Config needed  */
void Timer_Config(void);




#endif		//__TIMER_H
