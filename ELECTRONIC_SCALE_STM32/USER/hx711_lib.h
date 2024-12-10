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
#include "kalman_filter_lib.h"

/*
kg1 -> ADC1
kg2 -> ADC2
HPT	: ADC1.x + y = kg1
		: ADC2.x + y = kg2
		suy ra x, y
		suy ra: kg = x.ADC + y
*/
#define HX711_SAMPLES		20

uint32_t		HX711_Read(void);
void				HX711_Init(void);
void				HX711_Make_Sample_Data(void);
float				HX711_Get_LoadValue(void);
void				HX711_Calibration(void);
void				HX711_RangeConvert(void);

#ifdef __cplusplus
}
#endif

#endif



/***********************************************/
