/*
elevator motor pulse PA2 TH2_CH2 direction PA3 off PA4
door   motor pulse PA7 TH3_CH2 direction  PA6 off PA5
*/

#include"stm32f10x_lib.h"
#include"RCC_Configuration.h"  //系统时钟配置
#include"USART_Configuration.h"
#include"stm32f10x_exti.h"
#include"delay.h"
#include"stm32f10x_nvic.h"
#include"SwitchInit.h"
#include <stdio.h>

#define Elevator_Down       GPIO_Setbits(GPIOA,GPIO_Pin_3)//反向 远离电机
#define Elevator_Up     GPIO_Resetbits(GPIOA,GPIO_Pin_3)//正向 下降电机
#define Door_Open         GPIO_Resetbits(GPIOA,GPIO_Pin_6)
#define DOOR_Close        GPIO_Setbits(GPIOA,GPIO_Pin_6)

#define Elevator_StartMotor  TIM_Cmd(TIM2,ENABLE)
#define Elevator_StopMotor   TIM_Cmd(TIM2,DISABLE)
#define Door_StartMotor      TIM_Cmd(TIM3,ENABLE)
#define Door_StopMotor       TIM_Cmd(TIM3,DISABLE)

#define Elevator_Off       GPIO_Setbits(GPIOA,GPIO_Pin_4)//脱机
#define Elevator_On     	 GPIO_Resetbits(GPIOA,GPIO_Pin_4)//正常工作
#define Door_Off           GPIO_Setbits(GPIOA,GPIO_Pin_5)
#define DOOR_On            GPIO_Resetbits(GPIOA,GPIO_Pin_5)


void Motor_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_Off_InitStructure,GPIO_Dir_InitStructure,GPIO_Pulse_InitStructure;//脱机 方向 脉冲
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	
  GPIO_Off_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_4;	 //输出PTA5 高电平脱机
	GPIO_Off_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//推挽输出
	GPIO_Off_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Off_InitStructure);
	
	GPIO_Dir_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_3;	//方向输出端PTA6 高电平反转
	GPIO_Dir_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//推挽输出
	GPIO_Dir_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Dir_InitStructure);
	
	GPIO_Pulse_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_2;	//脉冲输出端PTA7
	GPIO_Pulse_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_Pulse_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Pulse_InitStructure);
	
}

void Motor_TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef Elevator_TIM_TimeBaseStructure;
	TIM_TimeBaseInitTypeDef Door_TIM_TimeBaseStructure;
	
	TIM_OCInitTypeDef Elevator_TIM_OCInitStructure;
	TIM_OCInitTypeDef Door_TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   //开启定时器时钟  
	TIM_DeInit(TIM3);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   //开启定时器时钟
	TIM_DeInit(TIM2);									   //初始值
	
	Elevator_TIM_TimeBaseStructure.TIM_Period = 999;
	Elevator_TIM_TimeBaseStructure.TIM_Prescaler = 719;
	Elevator_TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	Elevator_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2,&Elevator_TIM_TimeBaseStructure);
	
	Door_TIM_TimeBaseStructure.TIM_Period = 999;
	Door_TIM_TimeBaseStructure.TIM_Prescaler = 719;
	Door_TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	Door_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3,&Door_TIM_TimeBaseStructure);

	Elevator_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWM模式1
	Elevator_TIM_OCInitStructure.TIM_Pulse= 50; 					//调节占控制50%
	Elevator_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //使能输出比较
	Elevator_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;		//输出极性
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能TIM在CCR2上的预装载寄存器
	TIM_OC2Init(TIM2, &Elevator_TIM_OCInitStructure);			//装入TIM2_CH2
	
	Door_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWM模式1
	Door_TIM_OCInitStructure.TIM_Pulse= 50; 					//调节占控制50%
	Door_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //使能输出比较
	Door_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;		//输出极性
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能TIM在CCR2上的预装载寄存器
	TIM_OC2Init(TIM3, &Door_TIM_OCInitStructure);			//装入TIM3_CH2
}



