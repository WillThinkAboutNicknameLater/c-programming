#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "CodeTable.h"

#define ASCII_SIZE 256

void CreateCodeTable(TCodeTable* codeTable) {
    codeTable->TemporaryCode = NULL;
    codeTable->Codes = malloc(sizeof(bool*) * ASCII_SIZE);
    assert(codeTable->Codes != NULL);
    for (unsigned int idx = 0; idx < ASCII_SIZE; ++idx) {
        codeTable->Codes[idx] = NULL;
    }
}

void DestroyCodeTable(TCodeTable* codeTable) {
    DestroyBoolList(&(codeTable->TemporaryCode));
    for (unsigned int idx = 0; idx < ASCII_SIZE; ++idx) {
        DestroyBoolList(&(codeTable->Codes[idx]));
    }
    free(codeTable->Codes);
}
