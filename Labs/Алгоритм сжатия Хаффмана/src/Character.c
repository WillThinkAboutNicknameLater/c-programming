#include <assert.h>
#include "Character.h"

void CreateCharacter(TCharacter* character, unsigned char value, unsigned int frequency) {
    character->Value = value;
    character->Frequency = frequency;
}
