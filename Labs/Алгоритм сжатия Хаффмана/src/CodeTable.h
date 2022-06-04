#pragma once

#include "BoolList.h"

typedef struct {
    TBoolList* TemporaryCode;
    TBoolList** Codes;
} TCodeTable;

void CreateCodeTable(TCodeTable* codeTable);

void DestroyCodeTable(TCodeTable* codeTable);
