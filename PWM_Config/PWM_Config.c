/*
*PB9 BLUE PB8 GREEN PB7 RED 
*
*
*
*/


#include"stm32f10x_lib.h"
#include"PWM_Config.h"




void LED_PWM_Config()
{
  TIM_TimeBaseInitTypeDef LED_TIM_TimeBaseInit;

  TIM_OCInitTypeDef BLUE_TIM_OCInitStructure, Green_TIM_OCInitStructure, Red_TIM_OCInitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	//crystal oscallator 72K
	LED_TIM_TimeBaseInit.TIM_Period = 999;
	LED_TIM_TimeBaseInit.TIM_Prescaler = 68;//7.2K
	LED_TIM_TimeBaseInit.TIM_ClockDivision = 0;
	LED_TIM_TimeBaseInit.TIM_CounterMode = TIM_CounterMode_Down;
	
  TIM_TimeBaseInit(TIM4,&LED_TIM_TimeBaseInit);	
	
	BLUE_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	BLUE_TIM_OCInitStructure.TIM_Pulse = 0;
	BLUE_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	BLUE_TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC2Init(TIM4,&BLUE_TIM_OCInitStructure);
	
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC3Init(TIM4,&BLUE_TIM_OCInitStructure);
	
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC4Init(TIM4,&BLUE_TIM_OCInitStructure);
	
	
	
	
}



