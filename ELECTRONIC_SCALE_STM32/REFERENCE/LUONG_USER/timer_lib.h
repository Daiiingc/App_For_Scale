/*
	**********************************************
	*@file				timer_lib.h									*
	*@author			Vu Quang Luong				*
	*@date				15/07/2023						*
	**********************************************
*/

#ifndef	_TIMER_
#define	_TIMER_

#ifdef __cplusplus
	extern "C"{
#endif

#include "stm32f10x.h"     
#include "stm32f10x_tim.h"

void SysTick_Init(uint32_t freq);
void SysTick_Handler(void);
uint32_t SysTick_Millis(void);

#ifdef __cplusplus
}
#endif

#endif



/***********************************************/
