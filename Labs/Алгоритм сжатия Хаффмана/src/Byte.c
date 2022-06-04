#include <stdio.h>
#include <stdbool.h>
#include "Byte.h"

#define SIZE_OF_BYTE 8

void ResetByte(TByte* byte) {
    byte->NumberOfRecordedBits = 0;
    byte->Value = 0;
}

void WriteByteToFile(TByte byte, FILE* file) {
    fwrite(&(byte.Value), sizeof(unsigned char), 1, file);
}

void ShiftByteToLeft(TByte* byte) {
    byte->Value = byte->Value << 1;
    ++byte->NumberOfRecordedBits;
}

bool ReadByteFromFile(FILE* file, TByte* byte) {
    if (fread(&(byte->Value), sizeof(unsigned char), 1, file) < 1) {
        return false;
    }
    byte->NumberOfRecordedBits = SIZE_OF_BYTE;
    return true;
}

bool ResetBitAtPositionN(unsigned short position, TByte* byte) {
    bool lastBit = (byte->Value >> position) & 1;
    byte->Value &= ~(1 << position);
    --byte->NumberOfRecordedBits;
    return lastBit;
}
