#pragma once

#include <stdio.h>

typedef struct {
    unsigned char Value;
    unsigned short NumberOfRecordedBits;
} TByte;

void ResetByte(TByte* byte);

void WriteByteToFile(TByte byte, FILE* file);

void ShiftByteToLeft(TByte* byte);

bool ReadByteFromFile(FILE* file, TByte* byte);

bool ResetBitAtPositionN(unsigned short position, TByte* byte);
