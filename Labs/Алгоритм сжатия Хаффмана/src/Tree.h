#pragma once

#include "Character.h"
#include "Byte.h"

typedef struct TTree {
    TCharacter Character;
    struct TTree* LeftSubtree;
    struct TTree* RightSubtree;
} TTree;

void CreateTree(TTree** tree, TCharacter character);

bool IsLeaf(TTree* tree);

void GoAroundAndWriteTreeToFile(TTree* tree, FILE* file, TByte* byte);

void WriteTreeToFile(TTree* tree, FILE* file);

void ReadFileAndRestoreTree(FILE* file, TTree** huffmanTree, TByte* byte);

void RestoreTreeFromFile(FILE* file, TTree** huffmanTree);

void DestroyTree(TTree* tree);
