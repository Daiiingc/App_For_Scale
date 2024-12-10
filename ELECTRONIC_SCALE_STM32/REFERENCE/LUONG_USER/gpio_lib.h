/*
	**********************************************
	*@file				gpio_lib.h						*
	*@author			Vu Quang Luong				*
	*@date				15/07/2023						*
	**********************************************
*/

#ifndef	_GPIO_
#define	_GPIO_

#ifdef __cplusplus
	extern "C"{
#endif

#include "stm32f10x.h"     
#include "stm32f10x_rcc.h"  
#include "stm32f10x_gpio.h" 

#define HX711_DT			GPIO_Pin_9				// Data:			DT
#define HX711_SCK			GPIO_Pin_8				// Clock:			SCK
#define HX711_PORT		GPIOB							// Port:	

#define USART1_TX			GPIO_Pin_9				// Transmit:	TX
#define USART1_RX			GPIO_Pin_10				// Receive:		RX
#define USART1_PORT		GPIOA							// Port:			A

#define CALIB_BUTTON	GPIO_Pin_0				// 
#define HOLD_BUTTON		GPIO_Pin_1				// 
#define BUTTON_PORT		GPIOA							// Port:			A


void Gpio_Init(void);

#ifdef __cplusplus
}
#endif

#endif



/***********************************************/
