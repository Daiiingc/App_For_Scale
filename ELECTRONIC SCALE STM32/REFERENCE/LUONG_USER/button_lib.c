#include "button_lib.h"
#include "gpio_lib.h"
#include "HX711_lib.h"

uint8_t button_mode_flag = 0; // unhold

void ButtonCalib_Check(void){
	if( !GPIO_ReadInputDataBit(BUTTON_PORT,CALIB_BUTTON) ){
		HX711_Calibration();
	}
}
void ButtonMode_Check(void){
	if( !GPIO_ReadInputDataBit(BUTTON_PORT,HOLD_BUTTON) ){
		button_mode_flag = !button_mode_flag;
	}
}

void Button_HoldMode(void){
	
}

void Button_UnholdMode(void){
	
}

uint8_t Button_Get_ModeFlag(void){
	return button_mode_flag;
}


