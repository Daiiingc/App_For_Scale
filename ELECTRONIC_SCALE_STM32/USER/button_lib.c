#include "button_lib.h"
#include "gpio_lib.h"
#include "uart_lib.h"
#include "hx711_lib.h"

volatile button_state CALIB_BT, HOLD_BT;
volatile uint8_t button_mode_flag = UNHOLD_MODE;

void Button_ReadAll(void){
	Button_Read(INPUT_CALIB_BUTTON, (button_state*)&CALIB_BT);
	Button_Read(INPUT_HOLD_BUTTON	, (button_state*)&HOLD_BT);
	if(HOLD_BT.Flag == 1){
		Button_Invert_ModeFlag();
		HOLD_BT.Flag = 0;
	}
	if(CALIB_BT.Flag){
		HX711_Calibration();
		CALIB_BT.Flag = 0;
	}
}

static void Button_Read(const uint8_t input_signal,button_state *_button_var_){
	if(input_signal == PUSHED_STATE){
		if(_button_var_->count <= 2000){
			_button_var_->count++;
			if(_button_var_->count == 50){
					_button_var_->Flag = 1;
			}
		}
	}
	else{
		_button_var_->count = 0;
	}
}

void Button_Invert_ModeFlag(void){
	button_mode_flag = !button_mode_flag;
}

void Button_Set_HoldMode(void){
	button_mode_flag = 1;
}

void Button_Set_UnholdMode(void){
	button_mode_flag = 0;
}

uint8_t Button_Get_ModeFlag(void){
	return button_mode_flag;
}


