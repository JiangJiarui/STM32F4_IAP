#include "./Timer/Timer.h"


void Timer_Config(void)
{
    RCC_APB1PeriphClockCmd(User_Timer1_clk, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    
    TIM_TimeBaseInitStructure.TIM_Prescaler = 16800 - 1; //  5000Hz 
//    TIM_TimeBaseInitStructure.TIM_CounterMode = 
    TIM_TimeBaseInitStructure.TIM_Period = 25000-1;			// 5s
//    TIM_TimeBaseInitStructure.TIM_ClockDivision = 
//    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 

    TIM_TimeBaseInit(User_Timer1, &TIM_TimeBaseInitStructure);
    
}


/* 1s IWDG */
/* not used */
void IWDG_Config(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_64);
	IWDG_SetReload(0x271);
	IWDG_ReloadCounter();
}

