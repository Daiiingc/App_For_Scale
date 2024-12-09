/*
	**********************************************
	*@file				message.h			     *
	*@author			Vu Quang Luong			 *
	*@date				4/08/2023				 *
	**********************************************
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "stdint.h"
#include "string.h"
#include "convert.h"
#include "stdio.h"

/* Overview about Frame
 Frame Array: Start 1 | Type_Mes 1 | Sensor 1 | LengthData 1 | Data 4 | CheckFrame 2
 Index:          0            1          2            3          4..7      8     9
 Sensor = Sensor_Type OR Sensor_Order
*/


#define START_BYTE                      0xAA
#define DEFAULT_LENGTH                  4  /* StartFrame 1 + TypeMessage 1 + Sensor 1 + LengthData 1 */
#define WEIGHT_DATA_LENGTH              4
#define CHECKSUM_LENGTH									2

#define WEIGTH_CALIB_ASK_LENGTH         2        
#define WEIGTH_CALIB_ANWSER_LENGTH      6        
#define WEIGTH_HOLD_ASK_LENGTH          2        				
#define WEIGTH_HOLD_ANWSER_LENGTH       6        
#define WEIGTH_UNHOLD_ASK_LENGTH        2        
#define WEIGTH_UNHOLD_ANWSER_LENGTH     6        
#define WEIGTH_ASK_LENGTH               2        
#define WEIGTH_ANWSER_LENGTH            6             





/* TypeMessage */
typedef enum
{ 
    WEIGTH_CALIB_ASK    = 1,            // không chứa data về weight		
    WEIGTH_CALIB_ANWSER,                // chứa data về weigth ngay sau thời điểm bị calibated!    
    WEIGTH_HOLD_ASK,                    // không chứa data về weight		
    WEIGTH_HOLD_ANWSER,                 // chứa data về: weight tại thời điểm yêu cầu
    WEIGTH_UNHOLD_ASK,                  // không chứa data về weight		
    WEIGTH_UNHOLD_ANWSER,               // chứa data về: weight sau khi yêu cầu
    WEIGTH_ASK,                         // không chứa data về weight		
    WEIGTH_ANWSER,                      // chứa data về weigth tại thời điểm yêu cầu				
} type_message_e;

/* Sensor*/
typedef enum{ 
    LOADCELL = 0x10,
} sensor_type_e;

typedef enum{ 
    SENSOR_1 = 0x01,
    SENSOR_2,
    SENSOR_3,
    SENSOR_4,
    SENSOR_5,
} sensor_order_e;

typedef enum{ 
    WEIGHT = LOADCELL | SENSOR_1,
} sensor_e;

/* Ban tin cap nhat */
typedef struct{
    uint8_t     StartFrame;
    uint8_t     TypeMessage;
    uint8_t     Sensor;             		// LengthData = Length( Data[] + CheckFrame )
    uint8_t     LengthData;
    uint8_t     Data[4];                // Data[] = WeightData
    uint16_t    CheckFrame;
} FrameMsg_t;

extern uint8_t								frame_array[10];
extern FrameMsg_t							mes_frame_struct;
extern unsigned char					mes_frame_arrayout[10];
extern uint8_t								mes_print_state_flag;

void Mes_Create_FrameStruct(FrameMsg_t *frame_struct, uint8_t type_mes, uint8_t sensor, float weight_data);
uint8_t Mes_Create_FrameArray(FrameMsg_t *frame_struct, uint8_t *array_out);
uint8_t Mes_Detect_FrameStruct(uint8_t *array_in, FrameMsg_t *frame_struct);
uint8_t Copy_Data_From_Arr_To_Struct(uint8_t *data_in, FrameMsg_t *data_out, uint8_t index);
uint16_t CheckSum(uint8_t *buf, uint8_t len);

/* Tools */
void Mes_Convert_AskToAnwser_FrameStruct(FrameMsg_t *message_struct);
void Mes_Process_ReceivedMessage(void);
void Mes_SendUsart_Message(float *weight);


#ifdef __cplusplus
}
#endif

#endif
