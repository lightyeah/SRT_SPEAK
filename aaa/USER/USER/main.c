/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 库版本  ：ST3.5.0
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
 THU SKYWORKS
**********************************************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "adc.h"
#include "mypid.h"
#include "pwm_output.h"
#include "stdlib.h"
#define debug 0
vu16 ADC_ConvertedValue[32][2];
int After_filter[2];
extern int check,initEND;
PID xPID,yPID;

// 软件延时
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
} 

int myABS(int a)
{
if (a<0) a=(-1)*a;
return(a);
}

int calcPID(PID *pid, int input);

/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */

int main(void)
{ 
	//char test[2][3]={{'1','.','2'},{'0','0','0'}};
	int xPWM,yPWM;
	int i=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	xPID.Kp = 24.0;
  xPID.Ki = 0;
  xPID.Kd = 140.0;
  xPID.integrationError = 0;
  xPID.lastError = 0;
  xPID.targetValue = 2030;

  yPID.Kp = 24.0;
  yPID.Ki = 0;
	yPID.Kd = 140.0;
  yPID.integrationError = 0;
  yPID.lastError = 0;
  yPID.targetValue = 1260;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	
	/* USART1 config */
	USART1_Config();
	if(debug)
	{
			while(!initEND)
			{
				printf("no\n"); 
				Delay(0x111111);
			}
	
			Delay(0x1111111);
	
	    printf("kp=%f,ki=%f,kd=%f",xPID.Kp,xPID.Ki,xPID.Kd);
	}
	
	/* enable adc1 and config adc1 to dma mode */
	ADC1_Init();
	
  TIM3_PWM_Init();
	
	//printf("\n%f\n",atof(test[0]));
//	while(1)
//	{
//		printf("x_pos=%d,y_pos=%d\r", After_filter[0],After_filter[1]); 
//	}
	while(1)
	{	
		if(!check) continue; 
		
		check=0;
		xPWM=calcPID(&xPID,After_filter[0]);
		yPWM=calcPID(&yPID,After_filter[1]);
		if (xPWM<=0)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	    TIM_SetCompare1(TIM3,myABS(xPWM));
		}
		if (xPWM>0)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_13);
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			TIM_SetCompare1(TIM3,xPWM);
		}
		if (yPWM<=0)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
			GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	    TIM_SetCompare2(TIM3,myABS(yPWM));
		}
		if (yPWM>0)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_15);
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	    TIM_SetCompare2(TIM3,yPWM);
		}
		
//		printf("\r\nxpwm=%d,ypwm=%d\r\n",xPWM,yPWM);
//		if (i==0) 
//		{
//			GPIO_SetBits(GPIOB,GPIO_Pin_11);
//			i=1;
//		}
//		else
//		{
//			GPIO_ResetBits(GPIOB,GPIO_Pin_11);
//			i=0;
//		}
	
		
//printf("%04X,0x%04X \r   ", After_filter[0],After_filter[1]); 
//printf("\r\n The current x_plus,y_plus value =%f,%f\r\n", x_plus,y_plus);
//printf(" The current x_total,y_total value =%f,%f\r\n", x_total,y_total); 	
//printf("%d",check);		
//	printf(" %d,%d\r\n", abs_(x_plus),abs_(y_plus)); 
//	for(i = 0; i < 8; ++i)
//{
//    printf("%d \n", ADC_ConvertedValue[i][0]);
//}
//for(i = 0; i < 8; ++i)
//{
//    printf("%d \n", ADC_ConvertedValue[i][1]);
//}	
//printf("\r\n The current AD_x,AD_y value = 0x%04X,0x%04X \r\n", After_filter[0],After_filter[1]); 
//printf("\r\n The current AD_x,AD_y value =%f,%f\r\n", x_plus,y_plus); 
//Delay(0xffff00);
//GPIO_SetBits(GPIOB,GPIO_Pin_14);
//GPIO_ResetBits(GPIOB,GPIO_Pin_15);
//Delay(0x40189);    
	}
	
}

//	while (1)
//	{
//		ADC_ConvertedValueLocal_X =(float) ADC_ConvertedValue[0]/4096*3.3; 
//		ADC_ConvertedValueLocal_Y=(float) ADC_ConvertedValue[1]/4096*3.3;// 读取转换的AD值
//	
//printf("\r\n The current AD_x,AD_y value = 0x%04X,0x%04X \r\n", ADC_ConvertedValue[0],ADC_ConvertedValue[1]); 
//printf("\r\n The current ADv_x,ADv_y value = %f V,%f V \r\n",ADC_ConvertedValueLocal_X,ADC_ConvertedValueLocal_Y); 

//Delay(0x40189);                              // 延时 1ms
//}
/*****END OF FILE************/
