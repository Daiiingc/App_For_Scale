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

#define SENDUSART_TIME 500

extern volatile uint32_t timer4_ticks_usartsend;
extern volatile uint32_t timer4_ticks_makesampledata;

void Timer4_Base_Init(uint16_t timer_mode, uint16_t timer_period, uint16_t timer_prescaler, uint16_t clock_division, uint8_t repetition_counter, FunctionalState new_state);
void TIM4_IRQHandler(void);
void Timer4_Delay_Ms(uint32_t t);
uint32_t Timer4_Millis(void);





#ifdef __cplusplus
}
#endif

#endif



/***********************************************/
