#include "HX711_lib.h"

static uint32_t		hx711_received_data = 0;
static uint8_t		hx711_sample_count=0;
static uint32_t 	hx711_make_sample_data[HX711_SAMPLES];				// 10 samples
static uint8_t 		hx711_average_flag = 0;
static uint32_t		hx711_average = 0;
static double			hx711_range_convert = 0.0;
static double			hx711_calibration = 0.0;
static float			hx711_get_load_value = 0.0;

void HX711_Make_Sample_Data(void){
	if(hx711_average_flag == 0){
		if(hx711_sample_count < HX711_SAMPLES){
			hx711_make_sample_data[hx711_sample_count] = HX711_Receiver();
			hx711_sample_count++;
		}
		else{
			hx711_average_flag = 1;
			hx711_sample_count = 0;
		}
	}
}


void HX711_Data_Average(void){
	if(hx711_average_flag){
		uint8_t i=0;
		uint64_t hx711_data_sum = 0;
		for(i=0 ; i<HX711_SAMPLES ; i++){
			hx711_data_sum += (uint64_t)hx711_make_sample_data[i];
			hx711_make_sample_data[i] = 0;
		}
		hx711_average = ((uint32_t) (hx711_data_sum / HX711_SAMPLES));
		hx711_average_flag = 0;
	}
}

void HX711_RangeConvert(void){
	hx711_range_convert = ((double)hx711_average)* HX711_CONVERTING_FACTOR;
	hx711_get_load_value =  (float) (hx711_range_convert - hx711_calibration);
}

uint32_t HX711_Get_Data_Average(void){
	return hx711_average;
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
	
	return (hx711_received_data);
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


