/*
	**********************************************
	*@file				HX711_lib.h						*
	*@author			Vu Quang Luong				*
	*@date				15/07/2023						*
	**********************************************
*/

#ifndef	_HX711_
#define	_HX711_

#ifdef __cplusplus
	extern "C"{
#endif

#include "stm32f10x.h"       
#include "gpio_lib.h"
#include "stdio.h"
//  >> 0 bit:	0.00000258
//  >> 2 bit:	0.000103200
//	>> 4 bit:	0.00041320
#define HX711_CONVERTING_FACTOR			0.000103200
#define HX711_SAMPLES								10

uint32_t	HX711_Receiver(void);
void 			HX711_Make_Sample_Data(void);
uint32_t	HX711_Get_Sample_Data(void);
void 			HX711_Data_Average(void);
uint32_t 	HX711_Get_Data_Average(void);
uint8_t 	HX711_Get_Sample_Count(void);
float 		HX711_Get_LoadValue(void);
void 			HX711_Calibration(void);
void 			HX711_RangeConvert(void);

#ifdef __cplusplus
}
#endif

#endif



/***********************************************/
