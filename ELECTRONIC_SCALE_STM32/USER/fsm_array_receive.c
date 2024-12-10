#include "fsm_array_receive.h"
#include "convert.h"

static fsm_state_e fsm_state 			= FSM_STATE_START_FRAME;
static uint8_t fsm_arrayout_index = FSM_STATE_CHANGE_VALUE_START_FRAME;
uint8_t fsm_arr_out[10];
static uint16_t fsm_array_full_length;
static uint8_t fsm_data_ready_flag = 0;
static uint16_t fsm_receive_checksum;			/* Checksum cho arr_out */
static uint16_t fsm_arrayin_checksum;			/* Checksum cua array truyen den, duoc luu trong array_out tai 2 vi tri cuoi cung */

void Fsm_Test_Array_Receive(uint8_t data_in, uint8_t *array_out){
    switch(fsm_state){
        case FSM_STATE_START_FRAME:
								if(data_in != 0xAA){
									fsm_arrayout_index = FSM_STATE_CHANGE_VALUE_START_FRAME;
									fsm_state 				 = FSM_STATE_START_FRAME;
									break;
								}
                array_out[fsm_arrayout_index] = data_in;
                fsm_arrayout_index++;
                if(fsm_arrayout_index == FSM_STATE_CHANGE_VALUE_TYPE_MESSAGE)  fsm_state = FSM_STATE_TYPE_MESSAGE;
                break;
        case FSM_STATE_TYPE_MESSAGE:
                array_out[fsm_arrayout_index] = data_in;
                fsm_arrayout_index++;
                if(fsm_arrayout_index == FSM_STATE_CHANGE_VALUE_SENSOR)   fsm_state = FSM_STATE_SENSOR;
                break;
        case FSM_STATE_SENSOR:
                array_out[fsm_arrayout_index] = data_in;
                fsm_arrayout_index++;
                if(fsm_arrayout_index == FSM_STATE_CHANGE_VALUE_LENGHT_DATA)   fsm_state = FSM_STATE_LENGHT_DATA;
                break;        
        case FSM_STATE_LENGHT_DATA:
                array_out[fsm_arrayout_index] = data_in;
                fsm_arrayout_index++;
                if(fsm_arrayout_index == FSM_STATE_CHANGE_VALUE_END){
                    fsm_array_full_length = array_out[3] + DEFAULT_LENGTH;
                    if(fsm_array_full_length <= (DEFAULT_LENGTH + 6) ){
                          fsm_state = FSM_STATE_END;
                    }
                    else{                       
											fsm_arrayout_index = 0;
											fsm_data_ready_flag = 0;
											fsm_state = FSM_STATE_START_FRAME;
                        
                    }
                }
                break;
        case FSM_STATE_END:
                array_out[fsm_arrayout_index] = data_in;
                fsm_arrayout_index++;

                if(fsm_arrayout_index == fsm_array_full_length){
                    fsm_receive_checksum = CheckSum(array_out,fsm_array_full_length - 2);
                    fsm_arrayin_checksum = Convert_From_Bytes_To_Uint16(array_out[fsm_arrayout_index - 2], array_out[fsm_arrayout_index - 1]);
                    if(fsm_receive_checksum == fsm_arrayin_checksum){
											fsm_data_ready_flag = fsm_array_full_length;
                    }
										else{
											fsm_data_ready_flag = 0;
										}
										fsm_arrayout_index  = 0;
										fsm_state           = FSM_STATE_START_FRAME;
                }
								break;
    }
}

uint8_t Fsm_Get_DataReady_Flag(void){
	return fsm_data_ready_flag;
}

void Fsm_Clear_DataReady_Flag(void){
	fsm_data_ready_flag = 0;
}
