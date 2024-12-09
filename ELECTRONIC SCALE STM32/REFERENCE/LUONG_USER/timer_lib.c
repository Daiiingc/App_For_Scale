#include "timer_lib.h"
#include "HX711_lib.h"
#include "button_lib.h"

static uint32_t systicks = 0;

void SysTick_Init(uint32_t freq){
	SysTick_Config(SystemCoreClock/freq);
}
void SysTick_Handler(void){
	systicks++;
	HX711_Make_Sample_Data();
	ButtonCalib_Check();
	ButtonMode_Check();
}
uint32_t SysTick_Millis(void){
  return systicks;
}

