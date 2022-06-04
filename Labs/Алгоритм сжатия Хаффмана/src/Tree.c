#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Tree.h"
#include "Byte.h"
#include "Character.h"

#define SIZE_OF_BYTE 8

void CreateTree(TTree** tree, TCharacter character) {
    *tree = malloc(sizeof(TTree));
    assert((*tree) != NULL);
    (*tree)->Character = character;
    (*tree)->LeftSubtree = NULL;
    (*tree)->RightSubtree = NULL;
}

bool IsLeaf(TTree* tree) {
    return tree->LeftSubtree == NULL && tree->RightSubtree == NULL;
}

void GoAroundAndWriteTreeToFile(TTree* tree, FILE* file, TByte* byte) {
    ShiftByteToLeft(byte);
    if (IsLeaf(tree)) {
        if (byte->NumberOfRecordedBits == SIZE_OF_BYTE) {
            WriteByteToFile(*byte, file);
            ResetByte(byte);
        }
        unsigned char leafValue = tree->Character.Value;
        for (unsigned short idx = 0; idx < SIZE_OF_BYTE; ++idx) {
            ShiftByteToLeft(byte);
            byte->Value |= (leafValue >> (SIZE_OF_BYTE - 1 - idx)) & 1;
            if (byte->NumberOfRecordedBits == SIZE_OF_BYTE) {
                WriteByteToFile(*byte, file);
                ResetByte(byte);
            }
        }
    } else {
        byte->Value |= 1;
        if (byte->NumberOfRecordedBits == SIZE_OF_BYTE) {
            WriteByteToFile(*byte, file);
            ResetByte(byte);
        }
        GoAroundAndWriteTreeToFile(tree->LeftSubtree, file, byte);
        GoAroundAndWriteTreeToFile(tree->RightSubtree, file, byte);
    }
}

void WriteTreeToFile(TTree* tree, FILE* file) {
    TByte byte;
    ResetByte(&byte);
    GoAroundAndWriteTreeToFile(tree, file, &byte);
    if (byte.NumberOfRecordedBits != SIZE_OF_BYTE) {
        byte.Value <<= SIZE_OF_BYTE - byte.NumberOfRecordedBits;
        WriteByteToFile(byte, file);
    }
}

void ReadFileAndRestoreTree(FILE* file, TTree** huffmanTree, TByte* byte) {
    TCharacter emptyCharacter;
    CreateCharacter(&emptyCharacter, 0, 0);
    if (byte->NumberOfRecordedBits == 0) {
        ReadByteFromFile(file, byte);
    }
    bool bit = ResetBitAtPositionN(byte->NumberOfRecordedBits - 1, byte);
    CreateTree(huffmanTree, emptyCharacter);
    if (bit == 0) {
        unsigned char leafValue = 0;
        for (unsigned short idx = 0; idx < SIZE_OF_BYTE; ++idx) {
            if (byte->NumberOfRecordedBits == 0) {
                ReadByteFromFile(file, byte);
            }
            leafValue = (leafValue << 1) | (ResetBitAtPositionN(byte->NumberOfRecordedBits - 1, byte));
        }
        (*huffmanTree)->Character.Value = leafValue;
        (*huffmanTree)->Character.Frequency = 1;
        return;
    } else {
        ReadFileAndRestoreTree(file, &((*huffmanTree)->LeftSubtree), byte);
        ReadFileAndRestoreTree(file, &((*huffmanTree)->RightSubtree), byte);
    }
}

void RestoreTreeFromFile(FILE* file, TTree** huffmanTree) {
    TByte byte;
    ResetByte(&byte);
    ReadFileAndRestoreTree(file, huffmanTree, &byte);
    if (IsLeaf(*huffmanTree)) {
        TCharacter empty;
        CreateCharacter(&empty, 0, 0);
        TTree* tree = NULL;
        CreateTree(&tree, empty);
        tree->LeftSubtree = *huffmanTree;
        *huffmanTree = tree;
    }
}

void DestroyTree(TTree* tree) {
    if (tree != NULL) {
        DestroyTree(tree->LeftSubtree);
        DestroyTree(tree->RightSubtree);
        free(tree);
    }
}
