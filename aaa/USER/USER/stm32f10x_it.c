/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart1.h"
#include "stdlib.h"
#include "mypid.h"
#include "string.h"
extern PID xPID,yPID;
extern vu16 ADC_ConvertedValue[32][2];
char USARTbuf[50],USART_pid[3][10];
int check=0,initEND=0,USARTbuf_n=0,processFLAG=1;
float temp_float[3];

void Delay(__IO uint32_t nCount);
	
void myprocesschar()
{
	int j;
	for(j=0;j<3;j++)
	{
		temp_float[j] = atof(USART_pid[j]);
  }
}

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}
void DMA1_Channel1_IRQHandler(void) 

{
	
  if(DMA_GetITStatus(DMA1_IT_TC1) != RESET) 

   { 
	    void filter(void);
	    //printf("DMA");
			filter();  
			check=1;
//     DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, DISABLE);
//		 for(x=0;x<2;x++)
//			{
//		    for(y=0;y<32;y++)
//					printf("%d,,,",ADC_ConvertedValue[y][x]);
//				printf("----------------------------------------------------\n"); 
//			}
//		 DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
			DMA_ClearITPendingBit(DMA1_IT_TC1);			
	}
}

void USART1_IRQHandler(void)
{ 
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		//USART_ClearFlag(USART1, USART_FLAG_ORE);
	  USARTbuf[USARTbuf_n]=USART_ReceiveData(USART1);
	  if (USARTbuf[USARTbuf_n]=='e')
	 {
		int i=0,j=-1,k=0,m=0;//,n=0,o=0;
		memset(USART_pid,48,sizeof(char)*30);
		for(i=0;i<50;i++)
			{
				//printf("%c",USARTbuf[i]);
				if(USARTbuf[i]!='s')
				{
					if(USARTbuf[i]=='e')
					{
//						for(n=0;n<3;n++)
//						{
//						 for(o=0;o<10;o++) printf("%c",USART_pid[n][o]);
//						}
	           for(m=0;m<3;m++)
             {
		           temp_float[m] = atof(USART_pid[m]);
							 //printf("%f",temp_float[m]);
             }
					  break;
					}
					if(processFLAG) {j++;k=0;processFLAG=0;}
					USART_pid[j][k]=USARTbuf[i];
					//printf("%c",USART_pid[j][k]);
					k++;
		    }
				else processFLAG=1;
			}
		//Delay(0x11111);
		USARTbuf_n=0;
		xPID.Kp=temp_float[0];
		xPID.Ki=temp_float[1];
		xPID.Kd=temp_float[2];
		yPID.Kp=temp_float[0];
		yPID.Ki=temp_float[1];
		yPID.Kd=temp_float[2];
		printf("kp=%f,ki=%f,kd=%f",temp_float[0],xPID.Ki,xPID.Kd);
		initEND=1;
	 }
	else USARTbuf_n++;
	
	}
}
void TIM1_IRQHandler(void)
{
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
