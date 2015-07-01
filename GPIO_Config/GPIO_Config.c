/*
*A4 A5 B12 B13 photoresistance input
*PB9 BULE PB7 GREEN PB6 RED
*
*
*
*
*
*
*/

#include "stm32f10x_lib.h"
#include "GPIO_Config.h"


void PR_GPIO_Config(void)//PR INPUT CONFIG
{
	GPIO_InitTypeDef PR_GPIO_Init[2];
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	PR_GPIO_Init[0].GPIO_Mode = GPIO_Mode_IPU;
	PR_GPIO_Init[0].GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
	PR_GPIO_Init[0].GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOA , &PR_GPIO_Init[0]);
	
	PR_GPIO_Init[1].GPIO_Mode = GPIO_Mode_IPU;
	PR_GPIO_Init[1].GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 ;
	PR_GPIO_Init[1].GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB , &PR_GPIO_Init[1]);
	
	
}

int Get_PRInput(void)
{
  static int PRA4 = 0,PRA5 = 0,PRB12 = 0,PRB13 = 0,PRSTATE = 0;
	static int PRA5_Negate = 0, PRB13_Negate = 0;
	PRA4 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4);
	PRA5 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);
	if(PRA5)
	{
		PRA5_Negate = 0x00;
	}
	else
	{
		PRA5_Negate = 0x01;
	}
	PRB12 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12);
	PRB13 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
	if(PRB13)
	{
		PRB13_Negate = 0x00;
	}
	else
	{
		PRB13_Negate = 0x01;
	}

//printf("A%d|%d|%d|%d\r\n",PRA4,PRA5,PRB12,PRB13);
  PRSTATE = (PRA4<<3) | (PRA5_Negate<<2) | (PRB12<<1) | PRB13_Negate ;
	
	return PRSTATE;
	
}

void LED_GPIO_Config()
{
	GPIO_InitTypeDef LED_GND_GPIO_Init;
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//PB9 BLUE  PB8 GREEN PB7 RED 
	LED_GND_GPIO_Init.GPIO_Mode = GPIO_Mode_AF_PP;
	LED_GND_GPIO_Init.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 ;
	LED_GND_GPIO_Init.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOB , &LED_GND_GPIO_Init);
	
}

void GPIO_Config()
{
	PR_GPIO_Config();
	LED_GPIO_Config();
	/*test
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);*/
	
}