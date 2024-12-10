#include "HX711_lib.h"
#include "button_lib.h"

static uint32_t		hx711_received_data = 0;
static uint32_t 	hx711_make_sample_data = 0;
static uint8_t		hx711_sample_count=0;
static uint8_t		hx711_count=0;
static uint8_t 		hx711_ready_data_flag = 0;
static uint32_t		hx711_data_average = 0;
static uint32_t 	hx711_data_sum = 0;
static double			hx711_range_convert = 0.0;
static double			hx711_calibration = 0.0;
static float			hx711_get_load_value = 0.0;


void HX711_Make_Sample_Data(void){
	if(hx711_count >= 1)	hx711_ready_data_flag = 1;
	else{
		hx711_data_sum = HX711_Receiver();
		hx711_count++;
	}
}

uint32_t HX711_Get_Sample_Data(void){
	if(hx711_ready_data_flag){
		hx711_ready_data_flag = 0;
		hx711_count = 0; //
		return hx711_make_sample_data;
	}
	else{
		return 0;
	}
}

void HX711_Data_Average(void){
	if(hx711_sample_count >= HX711_SAMPLES){
		hx711_data_average = hx711_data_sum / HX711_SAMPLES;
		hx711_data_sum = 0;
		hx711_sample_count = 0;
	}
	else{
//		uint32_t temp = HX711_Get_Sample_Data();
//		if(temp){
			hx711_sample_count++;
			hx711_data_sum += HX711_Get_Sample_Data();
//		}
	}
}

void HX711_RangeConvert(void){
	hx711_range_convert = ((double)hx711_data_average) * HX711_CONVERTING_FACTOR;
	if(Button_Get_ModeFlag() == 0){
		hx711_get_load_value =  (float) (hx711_range_convert - hx711_calibration);
	}
}

uint32_t HX711_Get_Data_Average(void){
	return hx711_data_average;
}

uint32_t HX711_Receiver(void){
	if(!GPIO_ReadInputDataBit(HX711_PORT,HX711_DT) ){
		hx711_received_data = 0; 
		uint8_t i;
		for(i=1 ; i<= 24 ; i++){
			GPIO_SetBits(HX711_PORT,HX711_SCK);
			hx711_received_data <<= 1;				
			GPIO_ResetBits(HX711_PORT,HX711_SCK);
			if( GPIO_ReadInputDataBit(HX711_PORT,HX711_DT) ) hx711_received_data++;
		}
	}
	/* Mode of gain: 128, channel A => need 25th SCK */
	GPIO_SetBits(HX711_PORT,HX711_SCK);
	hx711_received_data ^= 0x800000;
	GPIO_ResetBits(HX711_PORT,HX711_SCK);
	
	return (hx711_received_data >> 2);
}

void HX711_Calibration(void){
	hx711_calibration = hx711_range_convert;
}

float HX711_Get_LoadValue(void){
	if((hx711_get_load_value > -0.0005)&&(hx711_get_load_value < 0.0005))	return 0.0;
	else{
		return hx711_get_load_value;
	}
}


