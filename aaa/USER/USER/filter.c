#include "stm32f10x.h"
#define N 32
extern int After_filter[2];
extern vu16 ADC_ConvertedValue[32][2];

void filter(void) 

{ 
	vu32 sum = 0; int count,i; 
	for(i=0;i<2;i++) 
	{
		for ( count=0;count<N;count++) 
		{
			sum += ADC_ConvertedValue[count][i]; 
		}
	After_filter[i]=sum>>5; 
	sum=0;
	}		
}
