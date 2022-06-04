#pragma once

typedef struct {
    unsigned char Value;
    unsigned int Frequency;
} TCharacter;

void CreateCharacter(TCharacter* character, unsigned char value, unsigned int frequency);
