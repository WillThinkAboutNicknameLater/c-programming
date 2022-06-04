#pragma once

#include "Tree.h"

typedef struct TTreeList {
    TTree* Tree;
    struct TTreeList* Next;
} TTreeList;

void PushToTreeList(TTreeList** head, TTree* tree);

TTree* PopFromTreeList(TTreeList** head);

void SortTreeListByCharacterFrequency(TTreeList** root);

TTreeList* ReverseTreeList(TTreeList* list);

TTreeList* CopyTreeList(const TTreeList* head);

void DestroyTreeList(TTreeList** head);
