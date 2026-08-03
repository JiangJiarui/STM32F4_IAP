#include "./Timer/Timer.h"


void Timer_Config(void)
{
    RCC_APB1PeriphClockCmd(User_Timer1_clk, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    /*  10000Hz */
    TIM_TimeBaseInitStructure.TIM_Prescaler = 8400 - 1;
//    TIM_TimeBaseInitStructure.TIM_CounterMode = 
    TIM_TimeBaseInitStructure.TIM_Period = 10000-1;
//    TIM_TimeBaseInitStructure.TIM_ClockDivision = 
//    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 

    TIM_TimeBaseInit(User_Timer1, &TIM_TimeBaseInitStructure);
    TIM_ITConfig(User_Timer1, TIM_IT_Update, ENABLE);
}


