
#include "stm32f10x_lib.h"
#include "RCC_Configuration.h"  //œµÕ≥ ±÷”≈‰÷√
#include "USART_Configuration.h"
#include "GPIO_Config.h"
#include "PWM_Config.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_nvic.h"
#include "stm32f10x_gpio.h"
#include <stdio.h>


int main(void)
	{
		int PR_Input[3] = {0,0,0};

		RCC_Configuration();
		USART_Configuration();
    GPIO_Config();
		LED_PWM_Config();
		
    /*TEST
		//TIM_Cmd(TIM4,ENABLE);
		TIM_SetCompare2(TIM4,400);
		TIM_SetCompare3(TIM4,40);
		TIM_SetCompare4(TIM4,40);*/
		
		while(1)
		{
			PR_Input[1] = Get_PRInput();
		  /*if(PR_Input[0] != PR_Input[1])
			{
				PR_Input[1] = PR_Input[0];
			}		*/		
			printf("%d\r\n",PR_Input[1]);
				switch(PR_Input[1])
				{
					case PRINPUT_1:
						break;
					case PRINPUT_2:
						break;
					case PRINPUT_3:
						break;
					case PRINPUT_4:
						break;
					case PRINPUT_5:
						break;
					case PRINPUT_6:
						break;
					case PRINPUT_7:
						break;
					case PRINPUT_8:
						break;
					case PRINPUT_9:
						break;
					case PRINPUT_10:
						break;
					case PRINPUT_11:
						break;
					case PRINPUT_12:
						break;
					case PRINPUT_13:
						break;
					case PRINPUT_14:
						break;
					case PRINPUT_15:
						break;
					
						
					default:
					break;
				}

				
		 }
	}
	


	