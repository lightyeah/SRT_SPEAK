#ifndef __SWITCHINIT_H
#define __SWITCHINIT_H
#include"stm32f10x_lib.h"

void Switch_GPIO_Init(void);
void Switch_NVIC_Config(void);
void Switch_EXTI_Config(void);
void Switch_Configration(void);
short int Is_Load_Complete();

#endif
