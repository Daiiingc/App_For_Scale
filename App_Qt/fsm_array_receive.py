from enum import Enum
import array as arr
import constant
import message
import convert


# Trường chứa trạng thái giá trị của FSM
class Fsm_State_E(Enum):
    FSM_STATE_START_FRAME  = 0
    FSM_STATE_TYPE_MESSAGE = 1
    FSM_STATE_LENGTH_DATA  = 2
    FSM_STATE_LENGTH_TEXT  = 3
    FSM_STATE_END          = 4

# Trường chứa trạng thái chuyển đổi giá trị tcủa FSM
class Fsm_State_Change_Value_E(Enum):
    FSM_STATE_CHANGE_VALUE_START_FRAME  = 0
    FSM_STATE_CHANGE_VALUE_TYPE_MESSAGE = 1
    FSM_STATE_CHANGE_VALUE_LENGTH_DATA  = 2
    FSM_STATE_CHANGE_VALUE_LENGTH_TEXT  = 3
    FSM_STATE_CHANGE_VALUE_END          = 4

fsm_state = Fsm_State_E.FSM_STATE_START_FRAME.value                                        # Gán biến trạng thái vào giá trị đầu tiên của trạng thái fsm 
fsm_array_out_index = Fsm_State_Change_Value_E.FSM_STATE_CHANGE_VALUE_START_FRAME.value    # Gán biến vị trí đầu tiên của mảng fsm_arr_out 
fsm_data_ready_flag = 0                                                                    # Cờ xác định việc truyền data cho fsm đã thành công hay chưa
fsm_array_full_length = 0


def Fsm_Test_Array_Receive(data_in, array_out):
    global fsm_state
    global fsm_array_out_index
    global fsm_data_ready_flag
    global fsm_array_full_length
    
    match fsm_state:
        case Fsm_State_E.FSM_STATE_START_FRAME.value:
            if(data_in != 0x55):
                
                fsm_array_out_index = 0
                fsm_state = Fsm_State_E.FSM_STATE_START_FRAME.value
                return 0  
            
            array_out.append(data_in)
            fsm_array_out_index += 1
            if(fsm_array_out_index == Fsm_State_Change_Value_E.FSM_STATE_CHANGE_VALUE_TYPE_MESSAGE.value):
                fsm_state = Fsm_State_E.FSM_STATE_TYPE_MESSAGE.value
            
        case Fsm_State_E.FSM_STATE_TYPE_MESSAGE.value:
            array_out.append(data_in)
            fsm_array_out_index += 1
            if(fsm_array_out_index ==  Fsm_State_Change_Value_E.FSM_STATE_CHANGE_VALUE_LENGTH_DATA.value):
                fsm_state = Fsm_State_E.FSM_STATE_LENGTH_DATA.value
        
        case Fsm_State_E.FSM_STATE_LENGTH_DATA.value:
            array_out.append(data_in)
            fsm_array_out_index += 1
            if(fsm_array_out_index ==  Fsm_State_Change_Value_E.FSM_STATE_CHANGE_VALUE_LENGTH_TEXT.value):
                fsm_state = Fsm_State_E.FSM_STATE_LENGTH_TEXT.value
        
        case Fsm_State_E.FSM_STATE_LENGTH_TEXT.value:
            array_out.append(data_in)
            fsm_array_out_index += 1
            if(fsm_array_out_index == Fsm_State_Change_Value_E.FSM_STATE_CHANGE_VALUE_END.value):
                fsm_array_full_length = array_out[2] + constant.DEFAULT_LENGTH
                if(fsm_array_full_length <= (constant.DEFAULT_LENGTH + 22)):
                    fsm_state = Fsm_State_E.FSM_STATE_END.value
                else:
                    if(fsm_array_full_length > (constant.DEFAULT_LENGTH + 22)):
                        fsm_array_out_index = 0
                        fsm_data_ready_flag = 0
                        fsm_state = Fsm_State_E.FSM_STATE_START_FRAME.value
        
        case Fsm_State_E.FSM_STATE_END.value:
            array_out.append(data_in)
            fsm_array_out_index += 1
            
            if(fsm_array_out_index == fsm_array_full_length):
                fsm_receive_checksum = message.check_sum(array_out,fsm_array_full_length - 2)
                fsm_array_in_checksum = convert.Convert_From_Bytes_To_Uint16(array_out[fsm_array_out_index - 2], array_out[fsm_array_out_index - 1])

                if(fsm_receive_checksum == fsm_array_in_checksum):
                    fsm_data_ready_flag = fsm_array_full_length
                    
                else:
                    fsm_data_ready_flag = 0

                fsm_array_out_index = 0
                fsm_state = Fsm_State_E.FSM_STATE_START_FRAME.value
                return 1
    return 0
            
                 

def Fsm_Get_DataReady_Flag():
    return fsm_data_ready_flag

def Fsm_Clear_DataReady_Flag():
    global fsm_data_ready_flag
    fsm_data_ready_flag = 0



    