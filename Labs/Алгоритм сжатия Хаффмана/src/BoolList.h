#pragma once

typedef struct TBoolList {
    bool Value;
    struct TBoolList* Next;
} TBoolList;

void PushToBoolList(TBoolList** head, bool value);

bool PopFromBoolList(TBoolList** head);

TBoolList* ReverseBoolList(TBoolList* list);

TBoolList* CopyBoolList(const TBoolList* head);

void DestroyBoolList(TBoolList** head);
