/*
*A4 A5 B12 B13 photoresistance input
*PB9 BULE PB7 GREEN PB6 RED
*
*
*
*
*
*
*/

#include "stm32f10x_lib.h"

#define PRINPUT_DEFAULT  0x00
#define PRINPUT_1        0x01
#define PRINPUT_2        0x02
#define PRINPUT_3        0x03
#define PRINPUT_4        0x04
#define PRINPUT_5        0x05
#define PRINPUT_6        0x06
#define PRINPUT_7        0x07
#define PRINPUT_8        0x08
#define PRINPUT_9        0x09
#define PRINPUT_10       0x0A
#define PRINPUT_11       0x0B
#define PRINPUT_12       0x0C
#define PRINPUT_13       0x0D
#define PRINPUT_14       0X0E
#define PRINPUT_15       0x0F




void PR_GPIO_Config(void);
int Get_PRInput(void);
void LED_GPIO_Conifg(void);
void GPIO_Config(void);