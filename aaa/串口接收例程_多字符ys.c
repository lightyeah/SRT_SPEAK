//注意，这里用的是 USART2 ,要根据需要改变
#include <string.h>
#include <stdio.h>
//收到一帧数据后将该标志置1
//读取后清零，防止重复读取
u8 uart2Flag=0;

//两级缓冲
char uart2Cache1[100];
char uart2Cache2[100];

void USART2_IRQHandler(void)
{
	static u8 byteCNT=0;
	char byteRec;
	//溢出中断处理
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)!=RESET) 
	{
		USART_ClearFlag(USART2, USART_FLAG_ORE);
		USART_ReceiveData(USART2);
		byteCNT=0;
	} 
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{
		byteRec=USART_ReceiveData(USART2);
		if(byteRec != '\r' && byteRec!='\n')	//判断结束符
		{	
			uart2Cache1[byteCNT++]=byteRec;
			if(byteCNT>=100) byteCNT=0;
		}
		else
		{	
			uart2Cache1[byteCNT]='\0';	//加字符串结尾标志
			memcpy(uart2Cache2,uart2Cache1,byteCNT+1);	//memcpy提高效率
			uart2Flag=1;	//标志位置1
			byteCNT = 0;
		}
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}

//串口数据处理
void vUartRecTask(void* pvParameters)
{
	char buffer[100];	//本地缓冲
	char *keyword;
	
	OptionalPara OP;
	WayPointType wpt;

	USART2_IT_Config();
	for(;;)
	{
		if(uart2Flag==1)
		{
			memcpy(buffer,uart2Cache2,100);
			uart2Flag=0;
			if((keyword=strstr(buffer,"PID")) != NULL)
			{
				sscanf(keyword,PID_FORMAT_IN
						,&(OP.rollP),&(OP.rollD),&(OP.rollI)
						,&(OP.yawP),&(OP.yawD1),&(OP.yawD2),&(OP.yawI)
						,&(OP.horiP),&(OP.horiD),&(OP.horiI)
						,&(OP.heightP),&(OP.heightD),&(OP.heightI));
			}
			else if((keyword=strstr(buffer,"Neutral")) != NULL)
			{
				sscanf(keyword,NEUTRAL_FORMAT_IN	
,&(OP.hoverThrust)
,&(OP.RCneutral[0])
,&(OP.RCneutral[1])
,&(OP.RCneutral[2])
,&(OP.RCneutral[3]));
			}
			else if((keyword=strstr(buffer,"Waypoint")) != NULL)
			{
				sscanf(keyword,WAYPOINT_FORMAT_IN
						,&(wpt.properties),&(wpt.maxSpeed),&(wpt.time),&(wpt.posAcrcy)
						,&(wpt.x),&(wpt.y),&(wpt.height),&(wpt.yaw));
			}
		}
}
}

其中
const char* PID_FORMAT_IN="PID,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f";
const char* NEUTRAL_FORMAT_IN="Neutral,%f,%hd,%hd,%hd,%hd";
const char* WAYPOINT_FORMAT_IN="Waypoint,%hd,%hd,%hd,%hd,%d,%d,%d,%d";

只要一帧数据长度不超过缓冲区长度，若要添加一个字段或者减少一个字段，只需要修改字符串格式常量和sscanf函数中的参数。

