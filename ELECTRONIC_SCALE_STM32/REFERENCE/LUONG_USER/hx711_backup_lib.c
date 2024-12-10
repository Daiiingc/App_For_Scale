#include "HX711_lib.h"
#include "KalmanFilterExample.h"

static uint8_t		hx711_sample_count=0;
static uint32_t		hx711_received_data = 0;
static float			hx711_receivedData_sum = 0.0;
static float			hx711_rangeConvert = 0.0;
static float			hx711_calibration = 0.0;
static float			hx711_get_loadValue = 0.0;

void HX711_ReceivedData_Average(void){
	if(hx711_sample_count > HX711_SAMPLES){
		HX711_RangeConvert();
		hx711_sample_count = 0;
	}
	hx711_receivedData_sum += (float)HX711_Receiver();	
//	hx711_receivedData_sum += KalmanFilterExample((double)HX711_Receiver(),&kalman_data);
	hx711_sample_count++;
}
void HX711_RangeConvert(void){
	hx711_rangeConvert = hx711_receivedData_sum / HX711_SAMPLES * HX711_CONVERTING_FACTOR;
	hx711_receivedData_sum = 0.0;
	hx711_get_loadValue = hx711_rangeConvert - hx711_calibration;
}
void HX711_Calibration(void){
	hx711_calibration = hx711_rangeConvert;
}
float HX711_Get_LoadValue(void){
	return hx711_get_loadValue;
}
float HX711_Get_AverageValue(void){
	return hx711_receivedData_sum / HX711_SAMPLES;
}

uint32_t HX711_Receiver(void){
	if( !GPIO_ReadInputDataBit(HX711_PORT,HX711_DT) ){
		hx711_received_data = 0; 
		uint8_t i;
		for(i=1 ; i<= 24 ; i++){
			GPIO_SetBits(HX711_PORT,HX711_SCK);
			hx711_received_data <<= 1;				
			GPIO_ResetBits(HX711_PORT,HX711_SCK);
			if( GPIO_ReadInputDataBit(HX711_PORT,HX711_DT) ) hx711_received_data++;
		}
	}
	/* Mode of gain: 128, channel A => need 25th SCK */
	GPIO_SetBits(HX711_PORT,HX711_SCK);
	hx711_received_data ^= 0x800000;
//	if(hx711_received_data & 0x800000)	hx711_received_data |= 0xFF000000;
	GPIO_ResetBits(HX711_PORT,HX711_SCK);
//	while(hx711_received_data >= 255)	hx711_received_data >>= 8;
	
	return hx711_received_data;
}

/* Kalman Structure Initialization */
//static kalman_filter_data_s kalman_data = 
//{
//	/* Transition matrix: 2x2 */
//	/* float Phi_matrix[4]; */
//	{1.0, 0.25e-3, 0.0, 1.0},
//	/* Q covariance plant noise matrix: 2x2 */
//	/* float Q_matrix[4]; */
//	{0.0, 0.0, 0.0, 1082.323},		
//	/* Sensitivity matrix: 1X2 */
//	/* float H_matrix[2]; */
//	{1.0, 0.0},		
//	/* Observation noise: R covariance matrix 1x1 */
//	/* float R_matrix; */
//	0.04,
//	/* P plus current covariance matrix 2x2: estimate error */
//	/* float P_plus[4]; */
//	{0.04, 160.0, 160.0, 641082.323},
//	/* x plus current state vector 2x1: value, speed */
//	/* float x_plus[2]; */
//	{0.0, 0.0},
//};

//uint32_t HX711_Receiver(void){
//	if( !GPIO_ReadInputDataBit(HX711_PORT,HX711_DT) ){
//		hx711_received_data = 0; 
//		uint8_t i;
//		for(i=1 ; i<= 24 ; i++){
//			GPIO_SetBits(HX711_PORT,HX711_SCK);			// Rising edge of SCK
//			hx711_received_data <<= 1;							// Shift LSB of hx711_received_data to the left
//			GPIO_ResetBits(HX711_PORT,HX711_SCK);
//			hx711_received_data |= (uint32_t)GPIO_ReadInputDataBit(HX711_PORT,HX711_DT);			// write to LSB of hx711_received_data
//		}
//	}
//	/* Mode of gain: 128, channel A => need 25th SCK */
//	GPIO_SetBits(HX711_PORT,HX711_SCK);
//	if(hx711_received_data & 0x800000){
//		hx711_received_data |= 0xFF000000;
//	}
////	hx711_received_data ^= 0x800000;
//	GPIO_ResetBits(HX711_PORT,HX711_SCK);
//	
//	return hx711_received_data;
//}


//uint32_t HX711_TempReceiver(void){
//	while(GPIO_ReadInputDataBit(HX711_PORT,HX711_DT)){
//		// unoccupied
//	}
//	hx711_received_data = 0;
//	uint8_t i;
//	for(i=0 ; i<24 ; i++){
//		GPIO_SetBits(HX711_PORT,HX711_SCK);
//		hx711_received_data <<= 1;
//		GPIO_ResetBits(HX711_PORT,HX711_SCK);
//		if(GPIO_ReadInputDataBit(HX711_PORT,HX711_DT))	hx711_received_data++;
//	}
//	/* Mode of gain: 128, channel A => need 25th SCK */
//	GPIO_SetBits(HX711_PORT,HX711_SCK);
// 	hx711_received_data ^= 0x800000;
//	GPIO_ResetBits(HX711_PORT,HX711_SCK);
//	
//	return hx711_received_data;
//}

