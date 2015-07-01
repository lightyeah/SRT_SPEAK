//ʹ��printf����ʱע��Ҫ��option for target�й�ѡUSE MicroLibѡ������stdio.h
#include "stm32f10x.h"
#include <stdio.h>
#include "USART.h"
#define RUN_POS GPIO_SetBits(GPIOB, GPIO_Pin_6)  	  //����ǰ��
#define RUN_REVERSE GPIO_ResetBits(GPIOB, GPIO_Pin_6)//����ǰ��

//#define LED1_OFF GPIO_SetBits(GPIOB, GPIO_Pin_4)  	  //LED�͵�ƽ�����ߵ�ƽ��
//#define LED1_ON GPIO_ResetBits(GPIOB, GPIO_Pin_4)

//#define LED2_OFF GPIO_SetBits(GPIOB, GPIO_Pin_5)  
//#define LED2_ON GPIO_ResetBits(GPIOB, GPIO_Pin_5)



void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				   //PA10Ϊ��������RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				   //PA9Ϊ�������TX 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		   //���߷�ת�ٶ�Ϊ2MHz	 �������ֻ��115.2k����ô��2M��GPIO�������ٶȾ͹��ˣ���ʡ��Ҳ����С
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;	 //��ʼ����������
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USART1,&USART_ClockInitStructure);

	USART_InitStructure.USART_BaudRate = 9600;				   //���ô��ڲ���
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

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); /*�����ж�ʹ��*/

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;   /*3.4�Ŀⲻ��ʹ��USART1_IRQChannel����stm32f10x.h��*/	
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


void USART1_IRQHandler(void) 	 //�����жϽ���
{ 
    s8 u1Rec; 
	  static STATE State = CheckS;//��ʼ״̬�����
	  static s8 PositionTemp[5]={0,0,0,0,0};//װ������ֵa(����ת) b(ǰ����) c(��е�ָ߶�) d(��е����ת�Ƕ�) Sum(���У��)
    if(USART_GetFlagStatus(USART1,USART_IT_RXNE)==SET) 
    {               
        u1Rec = USART_ReceiveData(USART1); 
        switch(State)
				{
          case CheckS:
						if(u1Rec==(s8)'S')
							State=CheckW;//׼����ת�����ݶ�ȡ
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
			//���-������������Ҫ�����SR�����λ,�ٶ�DR�Ĵ��� �������������жϵ�����
	  if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	  {
	  	USART_ClearFlag(USART1,USART_FLAG_ORE);     //�����λ
	  	USART_ReceiveData(USART1);                        //��DR
	  }
}

void Armheight(s8 height)//���ƻ�е�۸߶�
{
	TIM_SetCompare1(TIM3,height*12+1250);//���ƻ�е�ָ߶ȣ������ɸ���ʵ�������Ϊ4000-30*Height�ȣ�30��ȡֵ��Ϊ������ù��͡�
  	TIM_SetCompare2(TIM3,2500+5*height);//���Ƹ߶ȵ��������ת����ͬ
}

void PositionOfClaw(u32 position)//���ƻ�е�ֳ�ʼλ��
{
	TIM_SetCompare3(TIM3,position);//���ƻ�е����ת,TIMͨ���ɸ���
}

void Catch(u8 paraangle)//��е�ּгֲ����趨
{
	TIM_SetCompare4(TIM3,3000+paraangle*Angle);//��ʾ��е�ֵļг֣�ĿǰͨѶЭ����δ���壬�ö�ֵ���档
}

void RunAndTurn(u32 basicspeed,u32 speedset,u8 cspeedset)//�ٶȼ�ת�����ú���
{
	  	if(Speed>0)//����ǰ��
		{
		  TIM_SetCompare3(TIM2,basicspeed+Speed*speedset+Direction*cspeedset);//���Ƶ��A��ת��
		  TIM_SetCompare4(TIM2,0);
		  TIM_SetCompare1(TIM4,basicspeed+Speed*speedset-Direction*cspeedset);//���Ƶ��B��ת��
		  TIM_SetCompare2(TIM4,0);
		}
		else if(Speed<0)//����ǰ��
		{
		  TIM_SetCompare4(TIM2,basicspeed-Speed*speedset+Direction*cspeedset);//���Ƶ��A��ת��
		  TIM_SetCompare3(TIM2,0);
		  TIM_SetCompare2(TIM4,basicspeed-Speed*speedset-Direction*cspeedset);//���Ƶ��B��ת��
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
* Description    : Retargets the C library printf function to the USART.printf�ض���
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
* Description    : Retargets the C library printf function to the USART.fgetc�ض���
* Input          : None
* Output         : None
* Return         : ��ȡ�����ַ�
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
