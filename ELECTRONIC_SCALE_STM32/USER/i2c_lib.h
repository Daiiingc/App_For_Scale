/*
  ******************************************************************************
  * @file		i2c_lib.h                                                                  *
  * @author	           Tran Ngoc Dai                                                    *
  * @date		19/04/2023                                                         *
  ******************************************************************************
*/
#ifndef __I2C_LIB_
#define __I2C_LIB_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"

#define PCF8574T_Address    0x4E				// Dia chi cua I2C la 0x4E
#define I2C_Chanel      		I2C2				// Su dung bo I2C2 cua STM32


void I2c_Init(void);					// Cau hinh bo I2C2 


#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/

