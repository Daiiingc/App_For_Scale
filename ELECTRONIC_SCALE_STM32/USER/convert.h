#ifndef CONVERT_H
#define CONVERT_H

#include "stdint.h"
#include <stdlib.h>

typedef union 
{
	float data_float;
	uint8_t bytes[4];	
} data_format_float_bytes;

typedef union 
{
    uint16_t data_uint16;
    uint8_t bytes[2];
} data_format_uint8_16_t;

uint8_t *Convert_From_Float_To_Bytes(float data);
float Convert_From_Bytes_To_Float(uint8_t *data);
uint16_t Convert_From_Bytes_To_Uint16(uint8_t data0, uint8_t data1);

void Convert_Bytes_To_Weight(float *weight, uint8_t *bytes_data);
void Convert_Weight_To_Bytes(float weight, uint8_t *bytes_data);
uint8_t Convert_Text_To_Bytes(const char *text, uint8_t *bytes_data);

#endif // CONVERT_H







