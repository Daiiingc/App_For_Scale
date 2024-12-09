import constant
import struct
import convert
from array import array

def Message_Create_FrameStruct(frame_struct,type_msg,sensor):
    frame_struct.Start_Frame = constant.START_BYTE
    frame_struct.Type_Message = type_msg
    frame_struct.Sensor = sensor

    match(frame_struct.Type_Message):
        case constant.Type_Msg_E.WEIGTH_CALIB_ASK.value:
            frame_struct.Length_Data = constant.WEIGTH_CALIB_ASK_LENGTH

        case constant.Type_Msg_E.WEIGTH_HOLD_ASK.value:
            frame_struct.Length_Data = constant.WEIGTH_HOLD_ASK_LENGTH

        case constant.Type_Msg_E.WEIGTH_UNHOLD_ASK.value:
            frame_struct.Length_Data = constant.WEIGTH_UNHOLD_ASK_LENGTH
        
        case constant.Type_Msg_E.WEIGTH_ASK.value:
            frame_struct.Length_Data = constant.WEIGTH_ASK_LENGTH
        
def Message_Create_Frame_Array(frame_struct, array_out):
    if(frame_struct.Start_Frame != constant.START_BYTE):
        return 0
    
    array_out.append(frame_struct.Start_Frame)
    array_out.append(frame_struct.Type_Message)
    array_out.append(frame_struct.Sensor)
    
    if(frame_struct.Length_Data == 2):
        #Gán giá trị của Length_Data vào trong mảng
        data_len = struct.pack('B',frame_struct.Length_Data)
        array_out.append(data_len[0])

        check_sum = Check_Sum(array_out, constant.DEFAULT_LENGTH)
        array_out.append(check_sum & 0xff)
        array_out.append((check_sum >> 8) & 0xff)

    return len(array_out)

def Message_Detect_Frame(array_in,frame_struct):
    datain_index = 0
    if(array_in[datain_index] != constant.START_BYTE):
        return 0
    
    frame_struct.Start_Frame = array_in[datain_index]
    datain_index += 1
    
    frame_struct.Type_Message = array_in[datain_index]
    datain_index += 1
    
    frame_struct.Sensor = array_in[datain_index]
    datain_index += 1
    
    frame_struct.Length_Data = array_in[datain_index]
    datain_index += 1

    match(frame_struct.Type_Message):
        case constant.Type_Msg_E.WEIGTH_CALIB_ANWSER.value:
            datain_index = Copy_Data_From_Arr_To_Struct(array_in,frame_struct,datain_index)
        case constant.Type_Msg_E.WEIGTH_HOLD_ANWSER.value:
            datain_index = Copy_Data_From_Arr_To_Struct(array_in,frame_struct,datain_index)
        case constant.Type_Msg_E.WEIGTH_UNHOLD_ANWSER.value:
            datain_index = Copy_Data_From_Arr_To_Struct(array_in,frame_struct,datain_index)
        case constant.Type_Msg_E.WEIGTH_ANWSER.value:
            datain_index = Copy_Data_From_Arr_To_Struct(array_in,frame_struct,datain_index)

    frame_struct.Check_Frame = convert.Convert_From_Bytes_To_Uint16(array_in[datain_index], array_in[datain_index + 1])
    datain_index += 2
    return datain_index


def Copy_Data_From_Arr_To_Struct(data_in,data_out, temp_index):
    del data_out.Data[:]
    data_out.Data.append(data_in[temp_index])
    temp_index += 1
    data_out.Data.append(data_in[temp_index])
    temp_index += 1
    data_out.Data.append(data_in[temp_index])
    temp_index += 1
    data_out.Data.append(data_in[temp_index])
    temp_index += 1
    return temp_index

# in mảng
def Print_Array(arr,arr_len):
    
    for i in range(arr_len):
        print(hex(arr[i]), end=' ')
    print('\n')

# in các thành phần có trong struct
def Print_Struct(struct, len_data):

    print("Start Frame:", hex(struct.Start_Frame))
    print("Type_Message:", hex(struct.Type_Message))
    print("Sensor:", hex(struct.Sensor))
    print("Length_Data:", hex(struct.Length_Data))
    print("Data:", end= ' ')
    for i in range(len_data):
        print(hex(struct.Data[i]), end=' ')
    
    print('\n')
    print("Check_Frame:", hex(struct.Check_Frame))

def Check_Sum(buf, length):
    crc = 0xFFFF
    for pos in range(length):
        crc ^= buf[pos]  # XOR byte into least sig. byte of crc
        for i in range(8, 0, -1):  # Loop over each bit
            if crc & 0x0001:  # If the LSB is set
                crc >>= 1  # Shift right and XOR 0xA001
                crc ^= 0xA001
            else:  # Else LSB is not set
                crc >>= 1  # Just shift right
    return crc


# array_receive = array('B',[0xAA, 0x08, 0x11, 0x06, 0xb6, 0xf3, 0x9d, 0x3f, 0x36, 0x76])
# struct_out = constant.Frame_Msg_T()

# length = Message_Detect_Frame(array_receive,struct_out)
# print(length)
# Print_Struct(struct_out, 4)
