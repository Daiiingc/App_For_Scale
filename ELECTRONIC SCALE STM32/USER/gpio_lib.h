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

#define LCD_DATA			GPIO_Pin_11				// Data:			SDA
#define LCD_SCK				GPIO_Pin_10				// Clock:			SCL
#define LCD_PORT			GPIOB							// Port:			B

#define CALIB_BUTTON	GPIO_Pin_0				// Calibrate
#define HOLD_BUTTON		GPIO_Pin_1				// Hold / Unhold
#define BUTTON_PORT		GPIOA							// Port:			A
#define INPUT_CALIB_BUTTON			(uint8_t) GPIO_ReadInputDataBit(BUTTON_PORT,CALIB_BUTTON)
#define	INPUT_HOLD_BUTTON				(uint8_t) GPIO_ReadInputDataBit(BUTTON_PORT,HOLD_BUTTON)

#define LED_PC13				GPIO_Pin_13				// Clock:			SCL
#define LED_PC13_PORT		GPIOC						// Port:			C

void Gpio_Init(void);

#ifdef __cplusplus
}
#endif

#endif



/***********************************************/
