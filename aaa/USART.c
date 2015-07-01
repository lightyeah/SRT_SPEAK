//使用printf函数时注意要在option for target中勾选USE MicroLib选项，并添加stdio.h
#include "stm32f10x.h"
#include <stdio.h>
#include "USART.h"
#define RUN_POS GPIO_SetBits(GPIOB, GPIO_Pin_6)  	  //正向前进
#define RUN_REVERSE GPIO_ResetBits(GPIOB, GPIO_Pin_6)//反向前进

//#define LED1_OFF GPIO_SetBits(GPIOB, GPIO_Pin_4)  	  //LED低电平亮，高电平灭
//#define LED1_ON GPIO_ResetBits(GPIOB, GPIO_Pin_4)

//#define LED2_OFF GPIO_SetBits(GPIOB, GPIO_Pin_5)  
//#define LED2_ON GPIO_ResetBits(GPIOB, GPIO_Pin_5)



void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				   //PA10为串口输入RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				   //PA9为串口输出TX 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		   //口线翻转速度为2MHz	 最大波特率只需115.2k，那么用2M的GPIO的引脚速度就够了，既省电也噪声小
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;	 //初始化串口设置
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USART1,&USART_ClockInitStructure);

	USART_InitStructure.USART_BaudRate = 9600;				   //设置串口参数
	USART_InitStructure.USART_WordLength =USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);

	USART_Cmd(USART1,ENABLE);
}

void USART_IT_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); /*接收中断使能*/

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;   /*3.4的库不是使用USART1_IRQChannel，看stm32f10x.h吧*/	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
}

typedef enum
{
	CheckS,CheckW,Checka,Checkb,Checkc,Checkd,CheckSum
}STATE;

//u16 Start[1]='SW';
//s8 PositionData[5]={0,0,0,0,0};


void USART1_IRQHandler(void) 	 //串口中断接收
{ 
    s8 u1Rec; 
	  static STATE State = CheckS;//初始状态待检查
	  static s8 PositionTemp[5]={0,0,0,0,0};//装载数据值a(左右转) b(前后退) c(机械手高度) d(机械手旋转角度) Sum(求和校验)
    if(USART_GetFlagStatus(USART1,USART_IT_RXNE)==SET) 
    {               
        u1Rec = USART_ReceiveData(USART1); 
        switch(State)
				{
          case CheckS:
						if(u1Rec==(s8)'S')
							State=CheckW;//准备跳转至数据读取
						else
							State=CheckS;
						break;
					case CheckW:
						if(u1Rec==(s8)'W')
							State=Checka;
						else if(u1Rec==(s8)'S')
							State=CheckW;
						else 
							State=CheckS;
						break;
					case Checka:
						PositionTemp[0]=u1Rec;
					  State=Checkb;
					  break;
					case Checkb:
						PositionTemp[1]=u1Rec;
					  State=Checkc;
					  break;
					case Checkc:
						PositionTemp[2]=u1Rec;
					  State=Checkd;
					  break;
				  case Checkd:
						PositionTemp[3]=u1Rec;
					  State=CheckSum;
					  break;
					case CheckSum:
						PositionTemp[4]=u1Rec;
					  if(PositionTemp[4]==(s8)(PositionTemp[0]+PositionTemp[1]+PositionTemp[2]+PositionTemp[3]))
						{
							Direction = PositionTemp[0];
							Speed     = PositionTemp[1];
							Height    = PositionTemp[2];
							Angle     = PositionTemp[3];
						}
						State=CheckS;
						break;
					default:
						State=CheckS;
					  break;
				}
				USART_ClearITPendingBit(USART1, USART_IT_RXNE);		
    }				 
 //       USART_SendData(USART1,i);				  
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

void Armheight(s8 height)//控制机械臂高度
{
	TIM_SetCompare1(TIM3,height*12+1250);//控制机械手高度，参数可根据实际情况改为4000-30*Height等，30的取值是为避免弯得过低。
  	TIM_SetCompare2(TIM3,2500+5*height);//控制高度的两个舵机转角相同
}

void PositionOfClaw(u32 position)//控制机械手初始位置
{
	TIM_SetCompare3(TIM3,position);//控制机械手旋转,TIM通道可更改
}

void Catch(u8 paraangle)//机械手夹持参数设定
{
	TIM_SetCompare4(TIM3,3000+paraangle*Angle);//表示机械手的夹持，目前通讯协议中未定义，用定值代替。
}

void RunAndTurn(u32 basicspeed,u32 speedset,u8 cspeedset)//速度及转速设置函数
{
	  	if(Speed>0)//正向前进
		{
		  TIM_SetCompare3(TIM2,basicspeed+Speed*speedset+Direction*cspeedset);//控制电机A的转速
		  TIM_SetCompare4(TIM2,0);
		  TIM_SetCompare1(TIM4,basicspeed+Speed*speedset-Direction*cspeedset);//控制电机B的转速
		  TIM_SetCompare2(TIM4,0);
		}
		else if(Speed<0)//反向前进
		{
		  TIM_SetCompare4(TIM2,basicspeed-Speed*speedset+Direction*cspeedset);//控制电机A的转速
		  TIM_SetCompare3(TIM2,0);
		  TIM_SetCompare2(TIM4,basicspeed-Speed*speedset-Direction*cspeedset);//控制电机B的转速
		  TIM_SetCompare1(TIM4,0);	
		}
	    else 
		{
		  if(Direction==0)
		  {
			TIM_SetCompare4(TIM2,0);
			TIM_SetCompare3(TIM2,0);
			TIM_SetCompare2(TIM4,0);
			TIM_SetCompare1(TIM4,0);
		  }
		  else if(Direction>0)
	    {
		  TIM_SetCompare4(TIM2,basicspeed+Direction*cspeedset);
          TIM_SetCompare3(TIM2,0);
			TIM_SetCompare2(TIM4,0);
			TIM_SetCompare1(TIM4,basicspeed+Direction*cspeedset); }
		else
			 {
		  TIM_SetCompare3(TIM2,basicspeed-Direction*cspeedset);
          TIM_SetCompare4(TIM2,0);
			TIM_SetCompare1(TIM4,0);
			TIM_SetCompare2(TIM4,basicspeed-Direction*cspeedset); }			
		}
}




/*******************************************************************************
* Function Name  : int fputc(int ch, FILE *f)
* Description    : Retargets the C library printf function to the USART.printf重定向
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fputc(int ch, FILE *f)
{
  /* Write a character to the USART */
  USART_SendData(USART1, (u8) ch);

  /* Loop until the end of transmission */
  while(!(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET))
  {
  }

  return ch;
}

/*******************************************************************************
* Function Name  : int fgetc(FILE *f)
* Description    : Retargets the C library printf function to the USART.fgetc重定向
* Input          : None
* Output         : None
* Return         : 读取到的字符
*******************************************************************************/
int fgetc(FILE *f)
{
  /* Loop until received a char */
  while(!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET))
  {
  }
  
    /* Read a character from the USART and RETURN */
  return (USART_ReceiveData(USART1));
}
