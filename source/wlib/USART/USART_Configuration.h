#ifndef __USART_CONFIGURATION_H
#define __USART_CONFIGURATION_H
#include"stm32f10x_lib.h"
#include"stdio.h"

void USART_Configuration(void);
int fgetc(FILE *f);
int fputc(int ch, FILE *f);
void USART_Init_NVIC(void);

//typedef enum
//{
//	CheckS, CheckW,CheckD,Check4_0,Check4_1,Check4_2,Check4_3,CheckSum
//}STATE;
/*
short int Usart1_Instruct_Rec[6] = {0,0,0,0,0,0};
short int Instruct_Rec = 0;
short int Usart1_Rec = 0;
STATE Rec_State = CheckS;*/


#endif


