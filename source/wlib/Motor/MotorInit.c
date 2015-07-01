/*

rotor Up pa1 TH2_ch2
rotor Down pa2 th2_ch3

rotor left_1 pa6 th3_ch1  ��ת
rotor left_2 pa7 th3_ch2  ��ת

rotor  right_1 pb6 th4_th1 ��ת
rotor  right_2 pb7 th4_th2  ��ת
set 
*/

#include "stm32f10x_lib.h"
#include"RCC_Configuration.h"  //ϵͳʱ������
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
	GPIO_Pitch_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;	//���������PTA1 2
	GPIO_Pitch_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
	GPIO_Pitch_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//stop set to gpio
	GPIO_Pitch_Stop_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;	//���������PTA1 2
	GPIO_Pitch_Stop_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//�����������
	GPIO_Pitch_Stop_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_Pitch_InitStructure);
	
	//YAW  LEFT_1&2
	GPIO_Left1_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	//���������PTA6 7
	GPIO_Left1_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
	GPIO_Left1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//stopsettogpio
	GPIO_Left_Stop_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	//���������PTA6 7
	GPIO_Left_Stop_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//��ͨ���
	GPIO_Left_Stop_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_Left1_InitStructure);
	
	//YAW RIGHT_1&2
	GPIO_Right1_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	//���������PTB6 7
	GPIO_Right1_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
	GPIO_Right1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//stopsettogpio
	GPIO_Right_Stop_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	//���������PTA6 7
	GPIO_Right_Stop_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//�����������
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   //����TIM2��ʱ��ʱ��  
	TIM_DeInit(TIM2);	
	
	Pitch_TIM_TimeBaseStructure.TIM_Period = 99;
	Pitch_TIM_TimeBaseStructure.TIM_Prescaler = 719*7;
	Pitch_TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	Pitch_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2,&Pitch_TIM_TimeBaseStructure);

	Up_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWMģʽ1
	Up_TIM_OCInitStructure.TIM_Pulse= Duty_Up; 					//����ռ����50%
	Up_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //ʹ������Ƚ�
	Up_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//�������
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//ʹ��TIM��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC2Init(TIM2, &Up_TIM_OCInitStructure);			//װ��TIM2_CH2
	
	Down_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWMģʽ1
	Down_TIM_OCInitStructure.TIM_Pulse= Duty_Down; 					//����ռ����50%
	Down_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //ʹ������Ƚ�
	Down_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//�������
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);//ʹ��TIM��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3Init(TIM2, &Down_TIM_OCInitStructure);			//װ��TIM2_CH2
	
	
	//yaw left
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   //����TIM2��ʱ��ʱ��  
	TIM_DeInit(TIM3);	
	
	Left_TIM_TimeBaseStructure.TIM_Period = 99;
	Left_TIM_TimeBaseStructure.TIM_Prescaler = 719*7;
	Left_TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	Left_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3,&Left_TIM_TimeBaseStructure);

	Left1_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWMģʽ1
	Left1_TIM_OCInitStructure.TIM_Pulse= Duty_Left1; 					//����ռ����50%
	Left1_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //ʹ������Ƚ�
	Left1_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//�������
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);//ʹ��TIM��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC1Init(TIM3, &Left1_TIM_OCInitStructure);			//װ��TIM3_CH1
	
	Left2_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWMģʽ1
	Left2_TIM_OCInitStructure.TIM_Pulse= Duty_Left2; 					//����ռ����50%
	Left2_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //ʹ������Ƚ�
	Left2_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//�������
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);//ʹ��TIM��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC2Init(TIM3, &Left2_TIM_OCInitStructure);			//װ��TIM3_CH2
	
	//Yaw right
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   //����TIMM��ʱ��ʱ��  
	TIM_DeInit(TIM4);	
	
	Right_TIM_TimeBaseStructure.TIM_Period = 99;
	Right_TIM_TimeBaseStructure.TIM_Prescaler = 719*7;
	Right_TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	Right_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4,&Right_TIM_TimeBaseStructure);

	Right1_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWMģʽ1
	Right1_TIM_OCInitStructure.TIM_Pulse= Duty_Right1; 					//����ռ����50%
	Right1_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //ʹ������Ƚ�
	Right1_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//�������
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);//ʹ��TIM��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC1Init(TIM4, &Right1_TIM_OCInitStructure);			//װ��TIM4_CH2
	
	Right2_TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//PWMģʽ1
	Right2_TIM_OCInitStructure.TIM_Pulse= Duty_Right2; 					//����ռ����50%
	Right2_TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	 //ʹ������Ƚ�
	Right2_TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//�������
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);//ʹ��TIM��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC2Init(TIM4, &Right2_TIM_OCInitStructure);			//װ��TIM4_CH2
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



