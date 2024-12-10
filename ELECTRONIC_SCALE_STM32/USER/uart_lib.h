/*
	********************************************************************************************
	*@file				usart1.h																													*
	*@author			Vu Quang Luong																								*
	*@date				15/07/2023																										*
	********************************************************************************************
*/

#ifndef __USART__
#define __USART__

#ifdef __cplusplus
	extern "C"{
#endif

#include "stm32f10x.h"     
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stdio.h"

extern float kalman_mea_e;
extern float kalman_est_e;
extern float kalman_q		;
void Usart1_Set_SimpleKalmanFilter(void);
	
uint8_t* Recev_Str_Arr(void);
void Reset_Flag(void);

void USART1_Init(uint32_t baud_rates);																/* Khoi tao USART1				*/
void USART1_Send_Char(uint16_t _char);																/* Truyen mot ky tu				*/
void USART1_Send_String(uint8_t *string);															/* Truyen mot string			*/
void USART1_Send_Series(uint8_t *series, uint8_t series_length);			/* Truyen mot chuoi				*/
void USART1_Send_Number(int32_t number);															/* Truyen mot so					*/
uint8_t Compare_String(int8_t *string);																/* So sanh chuoi					*/
uint8_t USART1_Check_Flag(void);																			/* Check co								*/
void USART1_Send_Received_String(void);																/* In chuoi nhan duoc			*/


#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ***********************************************/
/*********************************************************************************************/

