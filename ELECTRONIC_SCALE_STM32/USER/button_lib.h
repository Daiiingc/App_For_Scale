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

#define PUSHED_STATE 0
#define HOLD_MODE		1
#define	UNHOLD_MODE 0

typedef struct
{
	uint8_t Flag;
	uint16_t count;
} button_state;

extern volatile button_state CALIB_BT, HOLD_BT;
extern volatile uint8_t button_mode_flag;

void Button_Set_HoldMode(void);
void Button_Set_UnholdMode(void);
uint8_t Button_Get_ModeFlag(void);
void		Button_Invert_ModeFlag(void);
static void 		Button_Read(const uint8_t input_signal,button_state *_button_var_);
void 						Button_ReadAll(void);
//void CalibButton_Init(void);
//void HoldButton_Init(void);



#ifdef __cplusplus
}
#endif

#endif



/***********************************************/
