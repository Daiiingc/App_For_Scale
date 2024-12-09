#include "stm32f10x.h"     
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h" 
#include "stdio.h"
#include "i2c_lib.h"
#include "lcd_lib.h"
#include "timer_lib.h"

static float old_weight_value = 1.0;

void LCD_Write_Byte(char data){
    /* Send START condition */
    I2C_GenerateSTART(I2C_Chanel, ENABLE);
    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2C_Chanel, I2C_EVENT_MASTER_MODE_SELECT));
    /* Send PCF8574A address for write */
    I2C_Send7bitAddress(I2C_Chanel, PCF8574T_Address, I2C_Direction_Transmitter);
	/* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C_Chanel, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    /* Send the byte to be written */
    I2C_SendData(I2C_Chanel, data);
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C_Chanel, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    /* Send STOP condition */
    I2C_GenerateSTOP(I2C_Chanel, ENABLE);		
}

void LCD_Write_Data(char data){
	char data_u, data_l;
	uint8_t data_t[4]={0},i=0;
	data_u = data&0xf0;
	data_l = (data<<4)&0xf0;
	data_t[0] = data_u|0x0d;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0d;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	for(i = 0;i<4;i++)
	{
	     LCD_Write_Byte(data_t[i]);
	}

}

void LCD_Write_Control(char cmd){
  char data_u, data_l;
	uint8_t data_t[4]={0},i=0;
	data_u = cmd&0xf0;
	data_l = (cmd<<4)&0xf0;
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	for(i = 0;i<4;i++)
	{
	     LCD_Write_Byte(data_t[i]);
	}

}

void LCD_Init (void){
	/* Set 4-bits interface */
	LCD_Write_Control(0x33);		 
	Timer4_Delay_Ms(DELAY_TIME);
	LCD_Write_Control(0x32);
	Timer4_Delay_Ms(DELAY_TIME);
	/* Start to set LCD function */
	LCD_Write_Control(0x28);
	Timer4_Delay_Ms(DELAY_TIME);	
	/* clear LCD */
	LCD_Write_Control(0x01);
	Timer4_Delay_Ms(DELAY_TIME);
	/* wait 60ms */
	
	/* set entry mode */
	LCD_Write_Control(0x06);	
	Timer4_Delay_Ms(DELAY_TIME);
	/* set display to on */	
	LCD_Write_Control(0x0C);	
	Timer4_Delay_Ms(DELAY_TIME);
	/* move cursor to home and set data address to 0 */
	LCD_Write_Control(0x02);	
	Timer4_Delay_Ms(DELAY_TIME);
	LCD_Write_Control(0x80);
}

void LCD_Write_String (char *str){
	while(*str) LCD_Write_Data(*str++);
}

void LCD_Goto_XY (uint8_t row, uint8_t col){
	uint8_t pos_Addr;
	if(row == 1) {
		pos_Addr = 0x80 + row - 1 + col;
	}
	else{
		pos_Addr = 0x80 | (0x40 + col);
	}
	LCD_Write_Control(pos_Addr);
}

void LCD_Interger_Number(int32_t number){
//		char str[5];
//    sprintf(str, "%d", number);
//    LCD_Write_String(str);
	uint8_t count_element=0;// count_element: bien dem so luong phan tu cua chuoi so
	int32_t varNumber=number;
	uint8_t string_number[10];
	if(number<0){
		varNumber=-varNumber;
	}
	while(varNumber){
		string_number[count_element]=(uint8_t)(varNumber%10)+48;
		count_element++;
		varNumber/=10;
	}
	if(number<0){
		string_number[count_element]='-';
		count_element++;
	}
		string_number[count_element]='\0';
	while(count_element--){
		LCD_Write_Data(string_number[count_element]);
	}
}	

void LCD_Float_Number(float number){
//		char str[5];
//    sprintf(str, "%.3f", number);
//    LCD_Write_String(str);
	uint8_t count_element=0;// count_element: bien dem so luong phan tu cua chuoi so
	int32_t varNumber=number*10000;
	uint8_t string_number[10];
	if(number<0){
		varNumber=-varNumber;
	}
	if(varNumber%10>=5){
			varNumber=(varNumber/10)+1;
	}else{
			varNumber=varNumber/10;
	}
	while(varNumber){
		string_number[count_element]=(uint8_t)(varNumber%10)+48;
		if(count_element==3){
			string_number[count_element]='.';
			count_element++;
			string_number[count_element]=(uint8_t)(varNumber%10)+48;
		}
		count_element++;
		varNumber/=10;
	}
	if(number<0){
		string_number[count_element]='-';
		count_element++;
	}
	string_number[count_element]='\0';
	while(count_element--){
		LCD_Write_Data(string_number[count_element]);
	}
}

void LCD_Clear(void){
	LCD_Write_Control(0x01);
	Timer4_Delay_Ms(5);
}
void LCD_Clear_Row(uint8_t row){
	uint8_t i=0;
	if(row==1){
		LCD_Write_Control(ROW1);
		for(i=0; i<16; i++){
			LCD_Write_Data(' ');
		}
	}
	if(row==2){
		LCD_Write_Control(ROW2);
		for(i=0; i<16; i++){
			LCD_Write_Data(' ');
		}
	}
}

void LCD_Print_On(void){
	LCD_Clear();
	LCD_Goto_XY(1,0);
	LCD_Write_String("Initializing...");
	Timer4_Delay_Ms(500);
	LCD_Goto_XY(2,0);
	LCD_Write_String("Calibrating...");
	Timer4_Delay_Ms(1000);
	LCD_Clear();
}

void LCD_Weight_Print(float weight){
	if(weight != old_weight_value){	//(weight - old_weight_value) >= 0.001 && (weight - old_weight_value) <= -0.001
		old_weight_value = weight;
		
		LCD_Clear_Row(2);
		LCD_Goto_XY(1,0);
		LCD_Write_String("WEIGHT:");
		if(weight==0){
			LCD_Goto_XY(2,7);
			LCD_Write_Data('0');
		}
		else if(weight >= 1){
			LCD_Goto_XY(2,4);
			LCD_Float_Number(weight);
			LCD_Goto_XY(2,10);
			LCD_Write_String("kg");
		}
		else if(weight < 1 && weight > 0){
			LCD_Goto_XY(2,5);		
			LCD_Interger_Number((weight)*1000);
			LCD_Goto_XY(2,9);	
			LCD_Write_String("g");
		}
		else if(weight >- 1 && weight < 0){
			LCD_Goto_XY(2,4);
			LCD_Interger_Number((weight)*1000.0);
			LCD_Goto_XY(2,9);
			LCD_Write_String("g");
		}
		else{
			LCD_Goto_XY(2,3);
			LCD_Float_Number(weight);
			LCD_Goto_XY(2,10);
			LCD_Write_String("kg");
		}
	}
}



