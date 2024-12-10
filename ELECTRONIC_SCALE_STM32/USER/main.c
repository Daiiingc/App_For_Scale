#include "timer_lib.h"
#include "hx711_lib.h"
#include "uart_lib.h"
#include "gpio_lib.h"
#include "button_lib.h"
#include "lcd_lib.h"
#include "i2c_lib.h"
#include "fsm_array_receive.h"


float	main_get_weight;
void Main_System_Init(void);

int main(void){
	Main_System_Init();
	printf("yes\n");
	while(1){
		if(Fsm_Get_DataReady_Flag()){
			printf("yes\n");
			Mes_Process_ReceivedMessage();
			Fsm_Clear_DataReady_Flag();
		}
		HX711_Make_Sample_Data();
		main_get_weight = HX711_Get_LoadValue();
		printf("%.3f\n",main_get_weight);
		if(timer4_ticks_makesampledata >= 150){
			LCD_Weight_Print(main_get_weight);
			timer4_ticks_makesampledata = 0;
		}
		if(timer4_ticks_usartsend >= SENDUSART_TIME){
			GPIO_ResetBits(LED_PC13_PORT, LED_PC13);
			Mes_SendUsart_Message(& main_get_weight);
			GPIO_SetBits(LED_PC13_PORT, LED_PC13);
			timer4_ticks_usartsend = 0;
		}
	}
}

void Main_System_Init(void){
	Gpio_Init();
	SimpleKalmanFilter(2,1,0.5);
	
	Timer4_Base_Init(0x0000,999,71,0,0,ENABLE);
	HX711_Init();
	USART1_Init(9600);
	printf("uart success\n");
	I2c_Init();
	printf("I2C success\n");
	//LCD_Init();
	printf("LCD success\n");
	LCD_Print_On();
	
}
