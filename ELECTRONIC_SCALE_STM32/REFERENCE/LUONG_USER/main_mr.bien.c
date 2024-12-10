#include "timer_lib.h"
#include "HX711_lib.h"
#include "uart_lib.h"
#include "gpio_lib.h"

static uint32_t get_ticks;
uint8_t count = 0;
uint64_t sum = 0;

void Delay(uint16_t t);

int main(void){
	Gpio_Init();
	USART1_Init(9600);
	SysTick_Init(1000);		// systick 1 ms
	
	USART1_Send_String((uint8_t*)"Start\n");
	HX711_Calibration();
	get_ticks = SysTick_Millis();
	while(1){
		if( (SysTick_Millis() - get_ticks) > 100){
			get_ticks = SysTick_Millis();
			if(count >= 10){
				 printf("%lld\n", sum/10);
				 sum = 0;
				 count = 0;
			}
			else{
				sum += Get_Sample_Data();
				count++;
			}
		}

	}
}

void Delay(uint16_t t){
	uint16_t i, j; 
	for(i=0 ; i<t ; i++){
		for(j=0 ; j<7999; j++);
	}
}

//		if( (SysTick_Millis() - get_ticks2) >5000 ){
//			get_ticks2 = SysTick_Millis();
//			HX711_Calibration();
//		}

//static uint32_t get_ticks2;
//get_ticks2 = SysTick_Millis();
