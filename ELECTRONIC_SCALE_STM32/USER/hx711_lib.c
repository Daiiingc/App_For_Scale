#include "hx711_lib.h"
#include "button_lib.h"
#include "uart_lib.h"
#include "stdio.h"

static uint32_t		hx711_read = 0;
			uint32_t		hx711_make_sample_data = 0;
			uint8_t			hx711_sample_count = 0;
			uint32_t		hx711_kalman_value = 0;
				float			hx711_range_convert = 0.0;
static float			hx711_calibration = 0.0;
static float			hx711_get_load_value = 0.0;

void HX711_Make_Sample_Data(void){
	hx711_make_sample_data = HX711_Read();
	if(hx711_make_sample_data){
		hx711_kalman_value = updateEstimate((float)(hx711_make_sample_data >> 8));
		HX711_RangeConvert();
	}
}

//void HX711_Make_Sample_Data(void){
//	hx711_make_sample_data = HX711_Read();
//	if(hx711_make_sample_data){
//		hx711_make_sample_data >>= 2;
//		printf("%u  ",hx711_make_sample_data);
//		hx711_kalman_value = (uint32_t)updateEstimate((float)(hx711_make_sample_data));
//		printf("%u\n",hx711_kalman_value);
//		hx711_range_convert = hx711_kalman_value * HX711_CONVERTING_FACTOR;
//		if(button_mode_flag == UNHOLD_MODE){
//			hx711_get_load_value = hx711_range_convert - hx711_calibration;
//		}
//	}
//}

/*
void HX711_Make_Sample_Data(void){
	if(hx711_sample_count < HX711_SAMPLES){
		hx711_make_sample_data = HX711_Read();
		if(hx711_make_sample_data){
			hx711_average_data += (hx711_make_sample_data >> 9);
			hx711_sample_count ++;
		}
	}
	else{
		hx711_average_data /= HX711_SAMPLES;

		HX711_RangeConvert();
		hx711_sample_count = 0;
	}
}
*/

void HX711_RangeConvert(void){
	hx711_range_convert = ((float)hx711_kalman_value / 1530.0) - (11629.0 / 510.0);
	if(button_mode_flag == UNHOLD_MODE){
		hx711_get_load_value = hx711_range_convert - hx711_calibration;
	}
}

float HX711_Get_LoadValue(void){
	if((hx711_get_load_value > -0.0009)&&(hx711_get_load_value < 0.0009))	return 0.0;
	else
		return hx711_get_load_value;
}

uint32_t HX711_Read(void){
	if(GPIO_ReadInputDataBit(HX711_PORT,HX711_DT) == 0){
		hx711_read = 0;
		uint8_t i;
		for(i=1 ; i<= 24 ; i++){
			GPIO_SetBits(HX711_PORT,HX711_SCK);
			hx711_read <<= 1;
			GPIO_ResetBits(HX711_PORT,HX711_SCK);
			if( GPIO_ReadInputDataBit(HX711_PORT,HX711_DT) ) hx711_read++;
		}
		/* Mode of gain: 128, channel A => need 25th SCK */
		GPIO_SetBits(HX711_PORT,HX711_SCK);
		hx711_read ^= 0x800000;
		GPIO_ResetBits(HX711_PORT,HX711_SCK);
		return hx711_read;
	}
	else{
		return 0;
	}
}

void HX711_Calibration(void){
	hx711_calibration = hx711_range_convert;
}

void HX711_Init(void){
	int i = 700000;
	while(i--){
		HX711_Make_Sample_Data();
	}
	HX711_Calibration();
}
