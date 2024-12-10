#include "stm32f10x.h"     
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h" 
#include "stm32f10x_i2c.h" 


void I2c_Init(void){
	
	// cau hinh I2C
	I2C_InitTypeDef I2C_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_Init(I2C2, &I2C_InitStructure);
	I2C_Cmd(I2C2, ENABLE);
}


