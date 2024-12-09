/*
	**********************************************
	*@file				button_lib.h						*
	*@author			Vu Quang Luong				*
	*@date				15/07/2023						*
	**********************************************
*/

#ifndef	_BUTTON_
#define	_BUTTON_

#ifdef __cplusplus
	extern "C"{
#endif

#include "stm32f10x.h"  
#include "stm32f10x_gpio.h" 


void ButtonCalib_Check(void);
void ButtonMode_Check(void);
void Button_HoldMode(void);
void Button_UnholdMode(void);
uint8_t Button_Get_ModeFlag(void);

#ifdef __cplusplus
}
#endif

#endif



/***********************************************/
