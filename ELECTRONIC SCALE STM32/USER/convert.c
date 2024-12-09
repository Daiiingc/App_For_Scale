#include "convert.h"

uint8_t *Convert_From_Float_To_Bytes(float data)
{
    static data_format_float_bytes input_convert;
    input_convert.data_float = data;
    return input_convert.bytes;
}

float Convert_From_Bytes_To_Float(uint8_t *data)
{
    static data_format_float_bytes input_convert;
    input_convert.bytes[0] = data[0];
    input_convert.bytes[1] = data[1];
    input_convert.bytes[2] = data[2];
    input_convert.bytes[3] = data[3];
    return input_convert.data_float;
}

uint16_t Convert_From_Bytes_To_Uint16(uint8_t data0, uint8_t data1){
    data_format_uint8_16_t input_convert;
    input_convert.bytes[0] = data0;
    input_convert.bytes[1] = data1;

    return input_convert.data_uint16;
}

void Convert_Weight_To_Bytes(float weight, uint8_t *bytes_data){
	uint8_t *buffer = Convert_From_Float_To_Bytes(weight);
	uint8_t i;
	for(i=0 ; i<4 ; i++){
		bytes_data[i] = buffer[i];
	}
}

void Convert_Bytes_To_Weight(float *weight, uint8_t *bytes_data){
	(*weight) = Convert_From_Bytes_To_Float(bytes_data);
}

uint8_t Convert_Text_To_Bytes(const char *text, uint8_t *bytes_data){
	uint8_t text_length = 0;
    while( (text[text_length]) ){
		bytes_data[text_length] = text[text_length];
        text_length++ ;
	}
//	bytes_data[text_length] = text[text_length]; // Nếu copy cả '\0' thì bỏ chú thích đi
    return text_length;             // trả về chiều dài ký tự in được của text[]
}
