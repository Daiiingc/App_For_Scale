#include "timer_lib.h"
#include "button_lib.h"

volatile uint32_t timer4_ticks=0;
volatile uint32_t timer4_ticks_makesampledata = 0;
volatile uint32_t timer4_ticks_usartsend = 0;

/*	@Brief: parameters
	timer_mode: chon che do hoat dong
			0x0000 - count up,
			0x0010 - count down,
			0x0020 - CenterAligned1 
			0x0040 - CenterAligned2 
			0x0060 - CenterAligned3 
	timer_period: gia tri automatic reload
	timer_prescaler: ty le chia tan so vao timer
	clock_division: bo chia xung clock cho timer
			0x0000 - TIM_CKD_DIV1                       
			0x0100 - TIM_CKD_DIV2                       
			0x0200 - TIM_CKD_DIV4                       
	repetition_counter: so lan dem lap cho timer, co gia tri 8 bit
	new_state: co the la ENABLE hoac DISABLE timer
	
	@ Cach cai dat timer_period: timer_period = T.F/(k+1) - 1, trong do:
			T: thoi gian kich hoat reload
			F: tan so clock APB1, max 72 MHz
			k: he so chia tan cho vao timer4 (timer_prescaler), k: 0 -> 2^16 - 1
		=> timer_period: 0 -> 2^16 - 1
*/
void Timer4_Base_Init(uint16_t timer_mode, uint16_t timer_period, uint16_t timer_prescaler, uint16_t clock_division, uint8_t repetition_counter, FunctionalState new_state){
	TIM_TimeBaseInitTypeDef tim_init_tydef;
	NVIC_InitTypeDef nvic_init_tydef;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	tim_init_tydef.TIM_CounterMode = timer_mode;
	tim_init_tydef.TIM_Period = timer_period;												/* Gia tri nap lai					*/
	tim_init_tydef.TIM_Prescaler = timer_prescaler;									/* Ty le chia tan	PSC + 1		*/
	tim_init_tydef.TIM_ClockDivision = clock_division;
	tim_init_tydef.TIM_RepetitionCounter = repetition_counter;			/* So lan dem lap cua Timer	*/
	TIM_TimeBaseInit(TIM4, &tim_init_tydef);
	TIM_Cmd(TIM4,new_state);

	/* NVIC	*/
	TIM_ITConfig(TIM4,TIM_IT_Update, ENABLE);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	nvic_init_tydef.NVIC_IRQChannel = TIM4_IRQn;
	nvic_init_tydef.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_init_tydef.NVIC_IRQChannelSubPriority = 0;
	nvic_init_tydef.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init_tydef);
}

void TIM4_IRQHandler(void){
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET){
		timer4_ticks++;
		timer4_ticks_makesampledata ++;
		timer4_ticks_usartsend ++;
		Button_ReadAll();
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}

uint32_t Timer4_Millis(void){
	return timer4_ticks;
}

void Timer4_Delay_Ms(uint32_t t){
	uint32_t start = timer4_ticks;
  uint32_t end = start +  t;
	if (start < end){ 
  	while((timer4_ticks >= start) && (timer4_ticks < end));
  }
	else{ 
    while((timer4_ticks >= start) || (timer4_ticks < end));
  }
}
