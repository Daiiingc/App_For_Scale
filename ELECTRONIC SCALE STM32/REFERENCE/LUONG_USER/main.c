#include "timer_lib.h"
#include "HX711_lib.h"
#include "uart_lib.h"
#include "gpio_lib.h"

static uint32_t		get_ticks;
static float			get_loadvalue;

void Delay(uint16_t t);
void Display(void);

int main(void){
	Gpio_Init();
	USART1_Init(9600);
	SysTick_Init(1000);		// systick 1 ms
	
	USART1_Send_String((uint8_t*)"Start\n");
	get_ticks = SysTick_Millis();
	while(1){
		HX711_RangeConvert();
		if( (SysTick_Millis() - get_ticks) > 100 ){
			get_ticks = SysTick_Millis();
			HX711_Data_Average();
			Display();
		}
	}
}

void Delay(uint16_t t){
	uint16_t i, j; 
	for(i=0 ; i<t ; i++){
		for(j=0 ; j<7999; j++);
	}
}
void Display(void){
	get_loadvalue = HX711_Get_LoadValue();
	if((get_loadvalue > -1.0)&&(get_loadvalue < 1.0)){
				if(get_loadvalue == 0.0)	printf("0\n");
				else											printf("%.0f g\n",1000.0*get_loadvalue);
	}
	else	printf("%.3f kg\n",get_loadvalue);
}

