/*
 Elevator Limit Switch  UP PB5 DOWN PB6
 Door Limit Swith  open PB0  colse PB1
 Load  Limit Switch PB15 
 Light Swith   up PB7 DOWN PB8  ����Ƿ��е����
*/
#include "stm32f10x_lib.h"
#include"RCC_Configuration.h"  //ϵͳʱ������
#include"USART_Configuration.h"
#include"stm32f10x_exti.h"
#include"delay.h"
#include"stm32f10x_nvic.h"
#include"SwitchInit.h"
#include"MotorInit.h"
#include <stdio.h>

u8 Load_Flag = 0;
void Switch_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_Limit_Init;
	GPIO_InitTypeDef GPIO_Light_Init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	/*Pin*/
	GPIO_Limit_Init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_15 | GPIO_Pin_5 | GPIO_Pin_6 ;
	GPIO_Light_Init.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 ;
	/*Mode*/
	GPIO_Limit_Init.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_Light_Init.GPIO_Mode = GPIO_Mode_IPU;//
	/*Speed*/
	GPIO_Limit_Init.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Light_Init.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB,&GPIO_Limit_Init);
	GPIO_Init(GPIOB,&GPIO_Light_Init);
	
}

void Switch_NVIC_Config()//�ж���������
{
	NVIC_InitTypeDef Limit_NVIC_Init;
	NVIC_InitTypeDef Light_NVIC_Init;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	Limit_NVIC_Init.NVIC_IRQChannel = EXTI0_IRQChannel ;//����ͨ��
	Limit_NVIC_Init.NVIC_IRQChannelPreemptionPriority = 0;//�������ȼ�
	Limit_NVIC_Init.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�
	Limit_NVIC_Init.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&Limit_NVIC_Init);//װ��
	
	Limit_NVIC_Init.NVIC_IRQChannel =  EXTI1_IRQChannel ;//����ͨ��
	Limit_NVIC_Init.NVIC_IRQChannelPreemptionPriority = 0;//�������ȼ�
	Limit_NVIC_Init.NVIC_IRQChannelSubPriority = 1;//��Ӧ���ȼ�
	Limit_NVIC_Init.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&Limit_NVIC_Init);//װ��	

	Limit_NVIC_Init.NVIC_IRQChannel = EXTI15_10_IRQChannel;//����ͨ��
	Limit_NVIC_Init.NVIC_IRQChannelPreemptionPriority = 1;//�������ȼ�
	Limit_NVIC_Init.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�
	Limit_NVIC_Init.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&Limit_NVIC_Init);//װ��
	
	Light_NVIC_Init.NVIC_IRQChannel = EXTI9_5_IRQChannel;
	Light_NVIC_Init.NVIC_IRQChannelPreemptionPriority = 1;
	Light_NVIC_Init.NVIC_IRQChannelSubPriority = 1;
	Light_NVIC_Init.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&Light_NVIC_Init);
	
}

void Switch_EXTI_Config()
{
	EXTI_InitTypeDef Limit_EXTI_Init;
	EXTI_InitTypeDef Light_EXTI_Init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	Switch_NVIC_Config();
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1 );
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5 );
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6 );
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0 );
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15 );//GPIO_PinSource1 | GPIO_PinSource5 | GPIO_PinSource6|GPIO_PinSource15);
	//ѡ���ж�����
	Limit_EXTI_Init.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line5 | EXTI_Line6 | EXTI_Line15;
	Limit_EXTI_Init.EXTI_Mode = EXTI_Mode_Interrupt;
	Limit_EXTI_Init.EXTI_Trigger = EXTI_Trigger_Falling;//�½����ж�
	Limit_EXTI_Init.EXTI_LineCmd = ENABLE ;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
	
	Light_EXTI_Init.EXTI_Line = EXTI_Line7 ;
	Light_EXTI_Init.EXTI_Mode = EXTI_Mode_Interrupt;
	Light_EXTI_Init.EXTI_Trigger = EXTI_Trigger_Falling;
	Light_EXTI_Init.EXTI_LineCmd =ENABLE ;
	
	EXTI_Init(&Limit_EXTI_Init);
  EXTI_Init(&Light_EXTI_Init);
}


void Switch_Configration(void)
{
	Switch_GPIO_Init();
	Switch_EXTI_Config();
}

short int Is_Load_Complete()//�����Źر�
{
	short int status;
	status = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);
	return status;
}




void EXTI0_IRQHandler()//ȡ���ſ�����λ
{
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)
	{
		Door_StopMotor;
		Door_Off;
	}
	EXTI_ClearFlag(EXTI_Line0);  
	EXTI_ClearITPendingBit(EXTI_Line0);
}
void EXTI1_IRQHandler()//ȡ���Źر���λ
{
	if(EXTI_GetITStatus(EXTI_Line1)!= RESET)
	{
		Door_StopMotor;
		Door_Off;
	}
	EXTI_ClearFlag(EXTI_Line1);  
	EXTI_ClearITPendingBit(EXTI_Line1);
}
void EXTI9_5_IRQHandler()//������λ ���
{
	if(EXTI_GetITStatus(EXTI_Line5)!= RESET)
	{
		Elevator_StopMotor;
		Elevator_Off;
		EXTI_ClearFlag(EXTI_Line5);  
		EXTI_ClearITPendingBit(EXTI_Line5);	
	}
	else if(EXTI_GetITStatus(EXTI_Line6)!= RESET)
	{
		Elevator_StopMotor;
		Elevator_Off;
		if(Load_Flag == 0 )
		{
		Elevator_On;
		Elevator_Up;
		Elevator_StartMotor;
		}
		EXTI_ClearFlag(EXTI_Line6);
		EXTI_ClearITPendingBit(EXTI_Line6);	
	}
	else if(EXTI_GetITStatus(EXTI_Line7)!= RESET)//ȡ��������⿪�� ��⵽����֮���Զ����� 
	{
		Elevator_On;
		Elevator_Up;
		Elevator_StartMotor;
		EXTI_ClearFlag(EXTI_Line7);  
	  EXTI_ClearITPendingBit(EXTI_Line7);	
	}

}
void EXTI15_10_IRQHandler()//����װ��
{
	if(EXTI_GetITStatus(EXTI_Line15)!= RESET)
	{
		Door_StopMotor;
		Door_Off;
		Elevator_Down;
		Elevator_On;
		Elevator_StartMotor;
	}
	EXTI_ClearFlag(EXTI_Line15);  
	EXTI_ClearITPendingBit(EXTI_Line15);
}