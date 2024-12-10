#include "HX711_lib.h"

volatile static uint8_t		hx711_sample_count=0;
volatile static uint64_t	hx711_received_data = 0;
volatile static uint64_t	hx711_received_data_sum = 0;
volatile static uint64_t		hx711_range_convert = 0;
volatile static float		hx711_calibration = 0.0;
volatile static float		hx711_get_load_value = 0.0;

volatile uint8_t flag_getdata = 0;

void HX711_Make_Sample_Data(void){
	if(hx711_sample_count >= 1){
		flag_getdata = 1;
	}
	else{
		hx711_received_data_sum = HX711_Receiver();	
		hx711_sample_count++;
	}
}

uint64_t HX711_Get_Sample_Data(void){
	if(flag_getdata == 1){
		flag_getdata = 0;
		hx711_sample_count = 0;
		return hx711_received_data_sum;
	}
	else{
		return 0;
	}
}

void HX711_RangeConvert(void){
	hx711_range_convert = hx711_received_data_sum; //	HX711_SAMPLES * HX711_CONVERTING_FACTOR;
	hx711_received_data_sum = 0;
	hx711_get_load_value = hx711_range_convert - hx711_calibration;
}

void HX711_Calibration(void){
	hx711_calibration = hx711_range_convert;
}

float HX711_Get_LoadValue(void){
	return hx711_get_load_value;
}

uint64_t HX711_Receiver(void){
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
	
//	if(hx711_received_data & 0x800000)	hx711_received_data |= 0xFF000000;
	GPIO_ResetBits(HX711_PORT,HX711_SCK);
//	while(hx711_received_data >= 255)	hx711_received_data >>= 8;
	
	return (hx711_received_data >> 2);
}
