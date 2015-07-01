/*

rotor Up pa1 TH2_ch2
rotor Down pa2 th2_ch3

rotor left_1 pa6 th3_ch1  正转
rotor left_2 pa7 th3_ch2  反转

rotor  right_1 pb6 th4_th1 正转
rotor  right_2 pb7 th4_th2  反转
set 
*/

#include "stm32f10x_lib.h"
#include"RCC_Configuration.h"  //系统时钟配置
#include"USART_Configuration.h"
#include"stm32f10x_exti.h"
#include"delay.h"
#include"stm32f10x_nvic.h"
#include"MotorInit.h"
#include <stdio.h>




int Duty_Up = 0, Duty_Down = 0, Duty_Left1 = 0, Duty_Right1 = 0,Duty_Left2 = 0, Duty_Right2 = 0;

GPIO_InitTypeDef GPIO_Pitch_InitStructure,GPIO_Pitch_Stop_InitStructure;
GPIO_InitTypeDef GPIO_Left1_InitStructure,GPIO_Left2_InitStructure,GPIO_Left_Stop_InitStructure;
	
GPIO_InitTypeDef GPIO_Right1_InitStructure,GPIO_Right2_InitStructure,GPIO_Right_Stop_InitStructure;
	
void Motor_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	
	//Pitch up & down
	GPIO_Pitch_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;	//脉冲输出端PTA1 2
	GPIO_Pitch_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_Pitch_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//stop set to gpio
	GPIO_Pitch_Stop_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;	//脉冲输出端PTA1 2
	GPIO_Pitch_Stop_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//复用推挽输出
	GPIO_Pitch_Stop_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_Pitch_InitStructure);
	
	//YAW  LEFT_1&2
	GPIO_Left1_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	//脉冲输出端PTA6 7
	GPIO_Left1_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_Left1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//stopsettogpio
	GPIO_Left_Stop_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	//脉冲输出端PTA6 7
	GPIO_Left_Stop_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//普通输出
	GPIO_Left_Stop_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_Left1_InitStructure);
	
	//YAW RIGHT_1&2
	GPIO_Right1_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	//脉冲输出端PTB6 7
	GPIO_Right1_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_Right1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//stopsettogpio
	GPIO_Right_Stop_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	//脉冲输出端PTA6 7
	GPIO_Right_Stop_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//复用推挽输出
	GPIO_Right_Stop_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_Right1_InitStructure);
	
	
	
}

void Motor_TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef Pitch_TIM_TimeBaseStructure;
	
	TIM_TimeBaseInitTypeDef Left_TIM_TimeBaseStructure;
	
	TIM_TimeBaseInitTypeDef Right_TIM_TimeBaseStructure;
	
	TIM_OCInitTypeDef Up_TIM_OCInitStructure,Left1_TIM_OCInitStructure,Left2_TIM_OCInitStructure;
	TIM_OCInitTypeDef Down_TIM_OCInitStructure,Right1_TIM_OCInitStructure,Right2_TIM_OCInitStructure;
	
	//PITCH
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   //开启TIM2定时器时钟  
	TIM_DeInit(TIM2);	
	
	Pitch_TIM_TimeBaseStructure.TIM_Period = 99;
	Pitch_TIM_TimeBaseStructure.TIM_Prescaler = 719*7;
	Pitch_TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	Pitch_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2,&Pitch_TIM_TimeBaseStructure);

	Up_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWM模式1
	Up_TIM_OCInitStructure.TIM_Pulse= Duty_Up; 					//调节占控制50%
	Up_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //使能输出比较
	Up_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//输出极性
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能TIM在CCR2上的预装载寄存器
	TIM_OC2Init(TIM2, &Up_TIM_OCInitStructure);			//装入TIM2_CH2
	
	Down_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWM模式1
	Down_TIM_OCInitStructure.TIM_Pulse= Duty_Down; 					//调节占控制50%
	Down_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //使能输出比较
	Down_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//输出极性
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能TIM在CCR2上的预装载寄存器
	TIM_OC3Init(TIM2, &Down_TIM_OCInitStructure);			//装入TIM2_CH2
	
	
	//yaw left
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   //开启TIM2定时器时钟  
	TIM_DeInit(TIM3);	
	
	Left_TIM_TimeBaseStructure.TIM_Period = 99;
	Left_TIM_TimeBaseStructure.TIM_Prescaler = 719*7;
	Left_TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	Left_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3,&Left_TIM_TimeBaseStructure);

	Left1_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWM模式1
	Left1_TIM_OCInitStructure.TIM_Pulse= Duty_Left1; 					//调节占控制50%
	Left1_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //使能输出比较
	Left1_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//输出极性
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能TIM在CCR1上的预装载寄存器
	TIM_OC1Init(TIM3, &Left1_TIM_OCInitStructure);			//装入TIM3_CH1
	
	Left2_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWM模式1
	Left2_TIM_OCInitStructure.TIM_Pulse= Duty_Left2; 					//调节占控制50%
	Left2_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //使能输出比较
	Left2_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//输出极性
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能TIM在CCR2上的预装载寄存器
	TIM_OC2Init(TIM3, &Left2_TIM_OCInitStructure);			//装入TIM3_CH2
	
	//Yaw right
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   //开启TIMM定时器时钟  
	TIM_DeInit(TIM4);	
	
	Right_TIM_TimeBaseStructure.TIM_Period = 99;
	Right_TIM_TimeBaseStructure.TIM_Prescaler = 719*7;
	Right_TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	Right_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4,&Right_TIM_TimeBaseStructure);

	Right1_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWM模式1
	Right1_TIM_OCInitStructure.TIM_Pulse= Duty_Right1; 					//调节占控制50%
	Right1_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //使能输出比较
	Right1_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//输出极性
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能TIM在CCR2上的预装载寄存器
	TIM_OC1Init(TIM4, &Right1_TIM_OCInitStructure);			//装入TIM4_CH2
	
	Right2_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWM模式1
	Right2_TIM_OCInitStructure.TIM_Pulse= Duty_Right2; 					//调节占控制50%
	Right2_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //使能输出比较
	Right2_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//输出极性
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能TIM在CCR2上的预装载寄存器
	TIM_OC2Init(TIM4, &Right2_TIM_OCInitStructure);			//装入TIM4_CH2
}

void Motor_Configration(void)
{
	Motor_GPIO_Init();
	Motor_TIM_Configuration();
}

void Motor_Stop(void)
{
		    GPIO_Init(GPIOA, &GPIO_Left_Stop_InitStructure);
				GPIO_ResetBits(GPIOA,GPIO_Pin_6);
				GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	
		    GPIO_Init(GPIOA, &GPIO_Right_Stop_InitStructure);
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	
	      GPIO_Init(GPIOA, &GPIO_Pitch_Stop_InitStructure);
				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}



