#include "message.h"
#include "hx711_lib.h"
#include "button_lib.h"
#include "uart_lib.h"

//uint8_t frame_array[10] = { 0xAA,0,0,0,0,0,0,0,0,0 };
FrameMsg_t					mes_frame_struct;
unsigned char				mes_frame_arrayout[10];
extern uint8_t			fsm_arr_out[10];
extern float				main_get_weight;

/*-------------------------------------------------------------------------------------------------------*/
void Mes_Convert_AskToAnwser_FrameStruct(FrameMsg_t *message_struct){
	switch(message_struct->TypeMessage){
		case WEIGTH_CALIB_ASK: 
                HX711_Calibration();
								HX711_RangeConvert();
								Mes_Create_FrameStruct(message_struct, WEIGTH_CALIB_ANWSER, WEIGHT, HX711_Get_LoadValue());
                break;
		case WEIGTH_HOLD_ASK: 
								Mes_Create_FrameStruct(message_struct, WEIGTH_HOLD_ANWSER, WEIGHT, HX711_Get_LoadValue());
                Button_Set_HoldMode();
                break;
		case WEIGTH_UNHOLD_ASK: 
                Button_Set_UnholdMode();
                HX711_RangeConvert();
								Mes_Create_FrameStruct(message_struct, WEIGTH_UNHOLD_ANWSER, WEIGHT, HX711_Get_LoadValue());
                break;
		case WEIGTH_ASK: 
								Mes_Create_FrameStruct(message_struct, WEIGTH_ANWSER, WEIGHT, HX711_Get_LoadValue());
                break;
	}
	
}

/* Procedure: FSM > Detect > Convert Ask_Mes To Anwser_Mes > Create > Send */
void Mes_Process_ReceivedMessage(void){
	Mes_Detect_FrameStruct(fsm_arr_out, & mes_frame_struct);
	Mes_Convert_AskToAnwser_FrameStruct(& mes_frame_struct);
	uint8_t mes_frame_arrayout_length = Mes_Create_FrameArray(&mes_frame_struct, mes_frame_arrayout);
	USART1_Send_Series(mes_frame_arrayout, mes_frame_arrayout_length);
}
void Mes_SendUsart_Message(float *weight){
	Mes_Create_FrameStruct(& mes_frame_struct, WEIGTH_ANWSER, WEIGHT, *weight);
	uint8_t mes_frame_arrayout_length = Mes_Create_FrameArray(&mes_frame_struct, mes_frame_arrayout);
	USART1_Send_Series(mes_frame_arrayout, mes_frame_arrayout_length);
}

/*-------------------------------------------------------------------------------------------------------*/
void Mes_Create_FrameStruct(FrameMsg_t *frame_struct, uint8_t type_mes, uint8_t sensor, float weight_data){
	frame_struct->StartFrame 	= START_BYTE;
	frame_struct->TypeMessage = type_mes;
	frame_struct->Sensor 			= sensor;
	switch(frame_struct->TypeMessage){
        case WEIGTH_CALIB_ASK:
            frame_struct->LengthData = WEIGTH_CALIB_ASK_LENGTH;
            break;
        case WEIGTH_CALIB_ANWSER:
            frame_struct->LengthData = WEIGTH_CALIB_ANWSER_LENGTH;
            break;
        case WEIGTH_HOLD_ASK:
            frame_struct->LengthData = WEIGTH_HOLD_ASK_LENGTH;
            break;
        case WEIGTH_HOLD_ANWSER:
            frame_struct->LengthData = WEIGTH_HOLD_ANWSER_LENGTH;
            break;
        case WEIGTH_UNHOLD_ASK:
            frame_struct->LengthData = WEIGTH_UNHOLD_ASK_LENGTH;
            break;
        case WEIGTH_UNHOLD_ANWSER:
            frame_struct->LengthData = WEIGTH_UNHOLD_ANWSER_LENGTH;
            break;
        case WEIGTH_ASK:
            frame_struct->LengthData = WEIGTH_ASK_LENGTH;
            break;
        case WEIGTH_ANWSER:
            frame_struct->LengthData = WEIGTH_ANWSER_LENGTH;
            break;
  }
	if(frame_struct->LengthData == 6){
		uint8_t *weight_bytes = Convert_From_Float_To_Bytes(weight_data);
		uint8_t i;
		for(i=0 ; i<4 ; i++){
			frame_struct->Data[i] = weight_bytes[i];
		}
	}
	// trường CheckFrame để khi vào Mes_Create_FrameArray() mới tạo vì trong đó có mảng bỏ vào hàm CheckSum() và khi transmit mới cần //
}

uint8_t Mes_Create_FrameArray(FrameMsg_t *frame_struct, uint8_t *array_out){
		if(frame_struct->StartFrame != START_BYTE)	return 0;
	  uint8_t *arr_temp;
    arr_temp = (uint8_t *)frame_struct;      /* chuyển từ struct sang arr */
		uint8_t index;
		uint16_t checksum;
		if(frame_struct->LengthData == 2){
			checksum = CheckSum(arr_temp, DEFAULT_LENGTH);
			for(index = 0; index < DEFAULT_LENGTH; index++){
				array_out[index] = arr_temp[index];
			}
		}
		else{
			checksum = CheckSum(arr_temp, DEFAULT_LENGTH + WEIGHT_DATA_LENGTH);
			for(index = 0; index < DEFAULT_LENGTH + WEIGHT_DATA_LENGTH; index++){
				array_out[index] = arr_temp[index];
			}
		}
		
    /*Ghi giá trị checksum tính được vào cuối mảng*/
    array_out[index++] = (checksum & 0xff);
    array_out[index++] = ((checksum >> 8) & 0xff);

    /* Trả về chiều dài của khung truyền array_out */
    return index;
}

/*-------------------------------------------------------------------------------------------------------*/
uint8_t Mes_Detect_FrameStruct(uint8_t *array_in, FrameMsg_t *frame_struct){
	if(array_in[0] != START_BYTE){
		return 0;
	}
	
	uint8_t datain_index = 0;
	frame_struct->StartFrame  = array_in[datain_index];
	datain_index += 1;
	frame_struct->TypeMessage = array_in[datain_index];
	datain_index += 1;
	frame_struct->Sensor		  = array_in[datain_index];
	datain_index += 1;
  frame_struct->LengthData  = array_in[datain_index];
	datain_index += 1;

	switch (frame_struct->TypeMessage){
        case WEIGTH_CALIB_ANWSER:
                datain_index = Copy_Data_From_Arr_To_Struct(array_in, frame_struct, datain_index);
                break;
        case WEIGTH_HOLD_ANWSER:
                datain_index = Copy_Data_From_Arr_To_Struct(array_in, frame_struct, datain_index);
                break;
        case WEIGTH_UNHOLD_ANWSER:
                datain_index = Copy_Data_From_Arr_To_Struct(array_in, frame_struct, datain_index);
                break;
        case WEIGTH_ANWSER:
                datain_index = Copy_Data_From_Arr_To_Struct(array_in, frame_struct, datain_index);
                break;
	}

	frame_struct->CheckFrame  = Convert_From_Bytes_To_Uint16(array_in[datain_index], array_in[datain_index + 1]);
	datain_index += 2;
	return datain_index;
}

uint8_t Copy_Data_From_Arr_To_Struct(uint8_t *data_in, FrameMsg_t *data_out, uint8_t temp_index){
  data_out->Data[0] = data_in[temp_index++];
	data_out->Data[1] = data_in[temp_index++];
	data_out->Data[2] = data_in[temp_index++];
	data_out->Data[3] = data_in[temp_index++];
	
	return temp_index;
}

/*-------------------------------------------------------------------------------------------------------*/
uint16_t CheckSum(uint8_t *buf, uint8_t len){
    uint16_t crc = 0xFFFF, pos = 0, i = 0;
    for (pos = 0; pos < len; pos++){
        crc ^= (uint16_t)buf[pos]; // XOR byte into least sig. byte of crc
        for (i = 8; i != 0; i--){   // Loop over each bit
            if ((crc & 0x0001) != 0) // If the LSB is set
            {
                crc >>= 1; // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else // Else LSB is not set
            {
                crc >>= 1; // Just shift right
            }
        }
    }
    return crc;
}

/*-------------------------------------------------------------------------------------------------------*/
#ifdef PRINT_MESSAGE_TEST
void Print_Data_Message_Array(uint8_t *arr, uint8_t length){
    uint8_t i;
    for(i = 0; i < length; i++){
        if (arr[i] <= 0x0F){
            printf("0%x ", arr[i]);
        }
        else{
            printf("%x ", arr[i]);
        }
    }
    printf("\n");
}

void Print_Text_Array(uint8_t *arr, uint8_t length){
    uint8_t i;
    for(i = 0; i < length; i++){
        printf("%x ",arr[i]);
    }
    printf("\n");
}

void Print_Data_Message_FrameMsg_t(FrameMsg_t printed_struct){
    printf("StartFrame = 0x%x\n",printed_struct.StartFrame);
    printf("TypeMessage = 0x%x\n",printed_struct.TypeMessage);
    printf("LengthData = %d\n",printed_struct.LengthData);
    printf("LengthText = %d, ",printed_struct.LengthText);

    if((printed_struct.TypeMessage & 0x01) == 0){
        printf("WeightData[0 1 2 3] = 0x -- ");
        uint8_t index;
        for(index=0 ; index<4 ; index++){
            if(printed_struct.Data[index] <= 0x0F)  printf("0%x ",printed_struct.Data[index]);
            else                                    printf("%x ",printed_struct.Data[index]);
        }
        printf("\nTextData: ");
        for(index = 4; index < printed_struct.LengthData + DEFAULT_LENGTH - 2 ; index++){
            printf("%c",printed_struct.Data[index]);
        }
    }
    printf("\nCheckFrame = 0x%x\n",printed_struct.CheckFrame);
}

void Print_Data_Message_Fsm(uint8_t *arr, uint8_t length){
    printf("\nGiá trị phần tử trong mảng được truyền qua USART: ");
    uint8_t i;
    for (i = 0; i < length; i++){
        if (arr[i] <= 0x0F) printf("0%x ", arr[i]);
        else    printf("%x ", arr[i]);
    }
}
#endif

