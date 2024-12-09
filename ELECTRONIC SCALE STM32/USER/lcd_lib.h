/*
  ******************************************************************************
  * @file		lcd_lib.h                                                                  *
  * @author	Tran Ngoc Dai                                                    *
  * @date		15/07/2023                                                         *
  ******************************************************************************
*/
#ifndef __LCD_LIB_
#define __LCD_LIB_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"

#define ROW1				0x80								// Hang 1 tren LCD
#define ROW2				0xC0								// Hang 2 tren LCD
#define DELAY_TIME	10

void LCD_Init(void);										// Khoi dong LCD thong qua I2C
void LCD_Write_Byte(char data);					// Gui du lieu xuong module I2C 
void LCD_Write_Control(char cmd);				// Ghi thong so cai dat len LCD
void LCD_Write_Data(char data);					// Ghi du lieu len LCD
void LCD_Write_String(char *str);				// Ghi chuoi ky tu len LCD
void LCD_Clear(void);										// Lenh xoa man hinh LCD
void LCD_Clear_Row(uint8_t row);						// Lenh xoa hang tren man hinh LCD 
void LCD_Goto_XY(uint8_t row, uint8_t col);			// Lenh dua con tro di den vi tri nhat dinh
void LCD_Interger_Number(int32_t number);		// Lenh ghi so nguyen len LCD
void LCD_Float_Number(float number);		// Lenh ghi so thuc len LCD
void LCD_Print_On(void);								// Lenh ghi bat LCD
void LCD_Weight_Print(float weight);


#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/

