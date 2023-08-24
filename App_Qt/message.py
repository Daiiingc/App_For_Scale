import constant
import struct
from constant import Type_Msg
import convert


def Message_Create_Frame(Data_in,Data_out):

    # Kiểm tra Start_Frame có đúng không
    if(Data_in.Start_Frame != constant.START_BYTE):
        return 0
    
    # Xác định kiểu bản tin để xác định độ dài bản tin
    match(Data_in.Type_Message):
        case Type_Msg.WEIGTH_CALIB_ASK.value:
            Data_in.Length_Data = constant.WEIGTH_CALIB_ASK_LENGTH        # độ dài bản tin calib_ask
        
        case Type_Msg.WEIGTH_HOLD_ASK.value:
            Data_in.Length_Data = constant.WEIGTH_HOLD_ASK_LENGTH         # độ dài bản tin hold_ask    
        
        case Type_Msg.WEIGTH_UNHOLD_ASK.value:
            Data_in.Length_Data = constant.WEIGTH_UNHOLD_ASK_LENGTH       # độ dài bản tin unhold_ask    
        
        case Type_Msg.WEIGTH_ASK.value:
            Data_in.Length_Data = constant.WEIGTH_ASK_LENGTH              # độ dài bản tin weight_ask
        
        case Type_Msg.WEIGTH_CALIB_ANWSER.value:
            Data_in.Length_Data = constant.WEIGTH_CALIB_ANWSER_LENGTH     # độ dài bản tin weight_calib_answer
            Data_in.Length_Text = constant.CALIB_ANWSER_TEXT_LENGTH       # độ dài bản tin text của weight_calib_answer

        case Type_Msg.WEIGTH_HOLD_ANWSER.value:
            Data_in.Length_Data = constant.WEIGTH_HOLD_ANWSER_LENGTH      # độ dài bản tin weight_hold_answer   
            Data_in.Length_Text = constant.HOLD_ANWSER_TEXT_LENGTH        # độ dài bản tin text của weight_hold_answer
        
        case Type_Msg.WEIGTH_UNHOLD_ANWSER.value:
            Data_in.Length_Data = constant.WEIGTH_UNHOLD_ANWSER_LENGTH    # độ dài bản tin weight_unhold_answer   
            Data_in.Length_Text = constant.UNHOLD_ANWSER_TEXT_LENGTH      # độ dài bản tin text của weight_unhold_answer  

        case Type_Msg.WEIGTH_ANWSER.value:
            Data_in.Length_Data = constant.WEIGTH_ANWSER_LENGTH           # độ dài bản tin weight_answer   
            Data_in.Length_Text = constant.WEIGHT_ANWSER_TEXT_LENGTH      # độ dài bản tin text của weight_answer
    
    # Tạo mảng đầu ra cho bản tin
    
    Data_out.append(Data_in.Start_Frame)
    Data_out.append(Data_in.Type_Message)

    if Data_in.Length_Data == 2:                                        # Bản tin ASK_WEIGHT
        # Gán giá trị của Length_Data vào trong mảng
        data_len    = struct.pack('H',Data_in.Length_Data)
        Data_out.append(data_len[0])
        Data_out.append(data_len[1])
        
        # Gán giá trị của Check_Frame vào trong mảng
        Data_in.Check_Frame = check_sum(Data_out,len(Data_out))
        crc = struct.pack('H',Data_in.Check_Frame)
        Data_out.append(crc[0])
        Data_out.append(crc[1])
    else:                                                               # Bản tin ASK_WEIGHT
         # Gán giá trị của Length_Data và Length_Text vào trong mảng
        data_len = struct.pack('BB',Data_in.Length_Data, Data_in.Length_Text)
        Data_out.append(data_len[0])
        Data_out.append(data_len[1])

        # Gán giá trị của Weight_Data và Text_Data vào trong mảng 
        for i in range(Data_in.Length_Data - 2):
            Data_out.append(Data_in.Data[i])
        
        # Gán giá trị của check frame vào trong mảng
        Data_in.Check_Frame = check_sum(Data_out,len(Data_out))
        crc = struct.pack('H',Data_in.Check_Frame)
        Data_out.append(crc[0])
        Data_out.append(crc[1])

    return len(Data_out)

def Message_Detect_Frame(Data_in,Data_out):
    datain_index = 0
    if(Data_in[datain_index] != constant.START_BYTE):
        return 0
    
    Data_out.Start_Frame = Data_in[datain_index]
    datain_index += 1
    
    Data_out.Type_Message = Data_in[datain_index]
    datain_index += 1
    
    Data_out.Length_Data = Data_in[datain_index]
    datain_index += 1
    
    Data_out.Length_Text = Data_in[datain_index]
    datain_index += 1

    match(Data_out.Type_Message):
        case Type_Msg.WEIGTH_CALIB_ANWSER.value:
            datain_index = Copy_Data_From_Arr_To_Struct(Data_in,Data_out,datain_index)
        case Type_Msg.WEIGTH_HOLD_ANWSER.value:
            datain_index = Copy_Data_From_Arr_To_Struct(Data_in,Data_out,datain_index)
        case Type_Msg.WEIGTH_UNHOLD_ANWSER.value:
            datain_index = Copy_Data_From_Arr_To_Struct(Data_in,Data_out,datain_index)
        case Type_Msg.WEIGTH_ANWSER.value:
            datain_index = Copy_Data_From_Arr_To_Struct(Data_in,Data_out,datain_index)

    Data_out.Check_Frame = convert.Convert_From_Bytes_To_Uint16(Data_in[datain_index], Data_in[datain_index + 1])
    datain_index += 2
    return datain_index-2-4


def Copy_Data_From_Arr_To_Struct(Data_in,Data_out, current_index):
    del Data_out.Data[:]
    new_index = Data_out.Length_Data + constant.DEFAULT_LENGTH - 2
    index = current_index
    for i in range(new_index - 4 ):
        Data_out.Data.append(Data_in[index])
        index += 1
    return new_index
        


def check_sum(buf, length):
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

    


 