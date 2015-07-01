/*
elevator motor pulse PA2 TH2_CH2 direction PA3 off PA4
door   motor pulse PA7 TH3_CH2 direction  PA6 off PA5
*/

#include"stm32f10x_lib.h"
#include"RCC_Configuration.h"  //ϵͳʱ������
#include"USART_Configuration.h"
#include"stm32f10x_exti.h"
#include"delay.h"
#include"stm32f10x_nvic.h"
#include"SwitchInit.h"
#include <stdio.h>

#define Elevator_Down       GPIO_Setbits(GPIOA,GPIO_Pin_3)//���� Զ����
#define Elevator_Up     GPIO_Resetbits(GPIOA,GPIO_Pin_3)//���� �½����
#define Door_Open         GPIO_Resetbits(GPIOA,GPIO_Pin_6)
#define DOOR_Close        GPIO_Setbits(GPIOA,GPIO_Pin_6)

#define Elevator_StartMotor  TIM_Cmd(TIM2,ENABLE)
#define Elevator_StopMotor   TIM_Cmd(TIM2,DISABLE)
#define Door_StartMotor      TIM_Cmd(TIM3,ENABLE)
#define Door_StopMotor       TIM_Cmd(TIM3,DISABLE)

#define Elevator_Off       GPIO_Setbits(GPIOA,GPIO_Pin_4)//�ѻ�
#define Elevator_On     	 GPIO_Resetbits(GPIOA,GPIO_Pin_4)//��������
#define Door_Off           GPIO_Setbits(GPIOA,GPIO_Pin_5)
#define DOOR_On            GPIO_Resetbits(GPIOA,GPIO_Pin_5)


void Motor_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_Off_InitStructure,GPIO_Dir_InitStructure,GPIO_Pulse_InitStructure;//�ѻ� ���� ����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	
  GPIO_Off_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_4;	 //���PTA5 �ߵ�ƽ�ѻ�
	GPIO_Off_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//�������
	GPIO_Off_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Off_InitStructure);
	
	GPIO_Dir_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_3;	//���������PTA6 �ߵ�ƽ��ת
	GPIO_Dir_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//�������
	GPIO_Dir_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Dir_InitStructure);
	
	GPIO_Pulse_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_2;	//���������PTA7
	GPIO_Pulse_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
	GPIO_Pulse_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Pulse_InitStructure);
	
}

void Motor_TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef Elevator_TIM_TimeBaseStructure;
	TIM_TimeBaseInitTypeDef Door_TIM_TimeBaseStructure;
	
	TIM_OCInitTypeDef Elevator_TIM_OCInitStructure;
	TIM_OCInitTypeDef Door_TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   //������ʱ��ʱ��  
	TIM_DeInit(TIM3);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   //������ʱ��ʱ��
	TIM_DeInit(TIM2);									   //��ʼֵ
	
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

	Elevator_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWMģʽ1
	Elevator_TIM_OCInitStructure.TIM_Pulse= 50; 					//����ռ����50%
	Elevator_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //ʹ������Ƚ�
	Elevator_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;		//�������
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//ʹ��TIM��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC2Init(TIM2, &Elevator_TIM_OCInitStructure);			//װ��TIM2_CH2
	
	Door_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWMģʽ1
	Door_TIM_OCInitStructure.TIM_Pulse= 50; 					//����ռ����50%
	Door_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //ʹ������Ƚ�
	Door_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;		//�������
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);//ʹ��TIM��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC2Init(TIM3, &Door_TIM_OCInitStructure);			//װ��TIM3_CH2
}



