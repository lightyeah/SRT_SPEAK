
#include "stm32f10x_lib.h"
#include"USART_Configuration.h"
#include"stm32f10x_usart.h"
#include"stm32f10x_gpio.h"
#include <stdio.h>

//#define USART_DEBUG
typedef enum
{
	CheckS, CheckW,CheckD,Check4_0,Check4_1,Check4_2,Check4_3,CheckSum
}STATE;

u8 Usart1_Instruct_Rec[16];
int Usart1_i = 0;
u8 Instruct_Rec[4] = {0};
u8 Usart1_Rec;
STATE Rec_State;
u8 Instruct_Flag = 0;

void USART_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	//管脚配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_Init_NVIC();
	//串口配置
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;	 //奇偶使能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	 //硬件控制流失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //串口接收中断使能    
	USART_Cmd(USART1, ENABLE);
}

void USART_Init_NVIC(void)
{ 	
  NVIC_InitTypeDef NVIC_InitStructure;			

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  
	NVIC_Init(&NVIC_InitStructure);							  
}



int fputc(int ch, FILE *f)
{

  USART_SendData(USART1, (u8) ch);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {
  }
  return ch;
}
 //printf("\r\n Welcome to www.PowerAVR.com \r\n");
 //	printf("\r\n aa=  %d\r\n",aa);

int fgetc(FILE *f)
{
  /* Loop until received a char */
  while(!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET))
  {
		
		
  }
  
    /* Read a character from the USART and RETURN */
  return (USART_ReceiveData(USART1));
}


void USART1_IRQHandler(void)
{
	//#ifdef USART_DEBUG
	//printf("ok_1!\r\n");
	//#endif
			
	if(USART_GetFlagStatus(USART1, USART_IT_RXNE) == SET)
	{ 
		/*Usart1_Instruct_Rec[Usart1_i] = USART_ReceiveData(USART1);
		Usart1_i++;
		#ifdef USART_DEBUG
		//printf("%d\r\n",Usart1_i);
		#endif*/
		/*
		if(Usart1_i == 8)
		{
			if(Usart1_Instruct_Rec[7] == Usart1_Instruct_Rec[6] + Usart1_Instruct_Rec[5] + Usart1_Instruct_Rec[4] + Usart1_Instruct_Rec[3] + Usart1_Instruct_Rec[2])
			{
				Instruct_Rec = Usart1_Instruct_Rec[2];
			}
			else
			{
				Usart1_i = 0;
			}
		}
		*/
	Usart1_Rec = USART_ReceiveData(USART1);
	#ifdef USART_DEBUG
	printf("ok_2!\r\n");
	#endif
		switch(Rec_State)
		{
			case CheckS:
				if(Usart1_Rec == (short int)'S')
				{
					Rec_State = CheckW;
				}
				else
					Rec_State = CheckS;
				break;
			case CheckW:
				if(Usart1_Rec == (short int )'W')
				{

					Rec_State = CheckD;					
				}
				else if(Usart1_Rec == (short int)'S')
					Rec_State = CheckW;
				else
					Rec_State = CheckS;
				break;
			case CheckD:
				Usart1_Instruct_Rec[0] = Usart1_Rec;
				Rec_State = Check4_0;
			  break;
			case Check4_0:
				Usart1_Instruct_Rec[1] = Usart1_Rec;
			  Rec_State = Check4_1;
			  break;
			case Check4_1:
				Usart1_Instruct_Rec[2] = Usart1_Rec;
			  Rec_State = Check4_2;
			  break;
			case Check4_2:
				Usart1_Instruct_Rec[3] = Usart1_Rec;
			  Rec_State = Check4_3;
			  break;
			case Check4_3:
				Usart1_Instruct_Rec[4] = Usart1_Rec;
				Rec_State = CheckSum;
			  break;
			case CheckSum:
				Usart1_Instruct_Rec[5] = Usart1_Rec;
			  if(Usart1_Instruct_Rec[5] == Usart1_Instruct_Rec[0] + Usart1_Instruct_Rec[1] + Usart1_Instruct_Rec[2] + Usart1_Instruct_Rec[3] + Usart1_Instruct_Rec[4])
				{
					Instruct_Rec[0] = Usart1_Instruct_Rec[0];//Up
					Instruct_Rec[1] = Usart1_Instruct_Rec[1];//Down
					Instruct_Rec[2] = Usart1_Instruct_Rec[2];//Left
					Instruct_Rec[3] = Usart1_Instruct_Rec[3];//Right
				}
				Rec_State = CheckS;
				break;
			default:
			  Rec_State = CheckS;
			  break;			
		}  
	
	//USART_ClearFlag(USART1,USART_IT_RXNE);  
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		
  }
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_TXE);
	}
	//溢出-如果发生溢出需要先清空SR的溢出位,再读DR寄存器 则可清除不断入中断的问题
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);     //清溢出位
		USART_ReceiveData(USART1);                        //读DR
	}
}