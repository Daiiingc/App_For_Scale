/*
	**********************************************
	*@file				fsm_array_receive.h		 *
	*@author			Vu Quang Luong			 *
	*@date				4/08/2023				 *
	**********************************************
*/

#ifndef FSM_ARRAY_RECEIVE_H
#define FSM_ARRAY_RECEIVE_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "stdint.h"
#include "message.h"

#define FSM_STATE 4
typedef enum{
    FSM_STATE_START_FRAME  = 0,
    FSM_STATE_TYPE_MESSAGE = 1,
    FSM_STATE_SENSOR			 = 2,
    FSM_STATE_LENGHT_DATA  = 3,
    FSM_STATE_END          = 4,
} fsm_state_e;

typedef enum{
    FSM_STATE_CHANGE_VALUE_START_FRAME  = 0,
    FSM_STATE_CHANGE_VALUE_TYPE_MESSAGE = 1,
    FSM_STATE_CHANGE_VALUE_SENSOR			  = 2,
    FSM_STATE_CHANGE_VALUE_LENGHT_DATA  = 3,
    FSM_STATE_CHANGE_VALUE_END          = 4,
} fsm_state_change_value_e;

extern uint8_t fsm_arr_out[10];

void Fsm_Test_Array_Receive(uint8_t data_in, uint8_t *array_out);
uint8_t Fsm_Get_DataReady_Flag(void);
void Fsm_Clear_DataReady_Flag(void);
uint8_t Fsm_Get_Arrayout_Index(void);

#ifdef __cplusplus
}
#endif

#endif



/***********************************************/

