#ifndef __MOTORINIT_H
#define __MOTORINIT_H
#include"stm32f10x_lib.h"



#define Pitch_StartMotor  GPIO_Init(GPIOA, &GPIO_Pitch_InitStructure);TIM_Cmd(TIM2,ENABLE)
#define Pitch_StopMotor   Motor_Stop();
#define Yaw_Left_StartMotor GPIO_Init(GPIOA, &GPIO_Left1_InitStructure);GPIO_Init(GPIOA, &GPIO_Right1_InitStructure);TIM_Cmd(TIM3,ENABLE)
#define Yaw_Left_StopMotor Motor_Stop();
#define Yaw_Right_StartMotor GPIO_Init(GPIOB, &GPIO_Left1_InitStructure);GPIO_Init(GPIOB, &GPIO_Right1_InitStructure);TIM_Cmd(TIM4,ENABLE)
#define Yaw_Right_StopMotor Motor_Stop();




void Motor_GPIO_Init(void);
void Motor_TIM_Configuration(void);
void Motor_Configration(void);
void Motor_Stop(void);

#endif


