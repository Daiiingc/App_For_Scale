#include "uart_lib.h"
#include "stdio.h"
#include "fsm_array_receive.h"
#include "message.h"
#include "kalman_filter_lib.h"

float kalman_mea_e = 0.0;
float kalman_est_e = 0.0;
float kalman_q		 = 0.0;
uint8_t kalman_count = 0;

static uint8_t recev_char;
static uint8_t usart1_rrx_it_flag=0;
static uint8_t recev_str_arr[100];
static uint8_t recev_str_len = 0;

void Kalman(void);
uint8_t* Convert_Number_To_String(int32_t num);			/* Chuyen doi So -> String	*/

void USART1_Init(uint32_t baud_rates){
	USART_InitTypeDef usart;
	/* USART1 pins are initiated in file gpio_lib.h */
	/* Mode TX USART1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);		/* clock ngoai vi USART */
	usart.USART_BaudRate = baud_rates;											/* toc do truyen: baud_rates */
	usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&usart);
	USART_Cmd(USART1,ENABLE);																/* cho phep USART1 hoat dong */
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);						/* ngat nhan USART1 					*/
	NVIC_EnableIRQ(USART1_IRQn);
}
void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1,USART_IT_RXNE) !=RESET){
		recev_char = (uint8_t) USART_ReceiveData(USART1);

		if(recev_char == '\n'){
			usart1_rrx_it_flag = 1;
			recev_str_arr[recev_str_len-1] = '\0';
			recev_str_len = 0;
		}
		else{
			recev_str_arr[recev_str_len] = recev_char;
			recev_str_len++;
		}	
		Fsm_Test_Array_Receive(recev_char, fsm_arr_out);
	}
}


void USART1_Send_Char(uint16_t _char){
	USART_SendData(USART1,_char);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}
void USART1_Send_String(uint8_t *string){
	while(*string != NULL){
		USART1_Send_Char(*(string++));
	}
}


void USART1_Send_Series(uint8_t *series, uint8_t series_length){
	uint8_t index;
	for(index = 0; index < series_length; index++){
		USART_SendData(USART1,series[index]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	}
}


// For print()
struct __FILE { 
    int dummy;
};
FILE __stdout; 
int fputc(int ch, FILE *f) { 
    USART1_Send_Char(ch); 
    return ch; 
} 

uint8_t* Convert_Number_To_String(int32_t num){
	  static uint8_t str[12];
    uint8_t i=0, j, k, temp;
    if(num < 0) {
        str[i++] = '-';
        num = -num;
    }
    do {
        str[i++] = (uint8_t) (num%10)+48;
    } while(num /= 10);
    str[i] = '\0';
    if(str[0] == '-') {
      k = 1;
			for(j = k; j < i / 2; j++) {
        temp = str[j];
        str[j] = str[i-j];
        str[i-j] = temp;
			}
    } else {
      k = 0;
			for(j = k; j< i/2; j++) {
        temp = str[j];
        str[j] = str[i-j-1];
        str[i-j-1] = temp;
			}
    }
		
		return str;
}

void USART1_Send_Number(int32_t number){
	USART1_Send_String(Convert_Number_To_String(number));
}


void USART1_Send_Received_String(void){
	USART1_Send_String(recev_str_arr);
}
uint8_t USART1_Check_Flag(void){
	if(usart1_rrx_it_flag){
		usart1_rrx_it_flag = 0;
		return 1;
	}
	return 0;
}
void USART1_Reset_Flag(void){
	usart1_rrx_it_flag = 0;
}
uint8_t* Recev_Str_Arr(void){
	return recev_str_arr;
}

void Usart1_Set_SimpleKalmanFilter(void){
	printf("Nhap he so Kalman:\n");
	while( kalman_count <= 2 ){
		if(usart1_rrx_it_flag){
			switch(kalman_count){
				case 0:
					sscanf((char*)recev_str_arr,"%f",&kalman_mea_e);
					if(kalman_mea_e != 0.0) kalman_count++;
					break;
				case 1:
					sscanf((char*)recev_str_arr,"%f",&kalman_est_e);
					if(kalman_est_e != 0.0) kalman_count++;
					break;
				case 2:
					sscanf((char*)recev_str_arr,"%f",&kalman_q);
					if(kalman_q != 0.0) kalman_count++;
					break;
			}
			usart1_rrx_it_flag = 0;
		}
	}
	SimpleKalmanFilter(kalman_mea_e, kalman_est_e, kalman_q);
	printf("Set Kalman successfully!\nmea_e: %f\nest_e: %f\nq: %f\n\n",kalman_mea_e,kalman_est_e,kalman_q);
}
