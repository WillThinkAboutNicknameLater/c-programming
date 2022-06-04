#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "BoolList.h"
#include "TreeList.h"
#include "Tree.h"
#include "Character.h"
#include "CodeTable.h"
#include "Byte.h"

#define ASCII_SIZE 256
#define SIZE_OF_BYTE 8

void ZipFile(FILE* input, FILE* output);

void UnzipFile(FILE* input, FILE* output);

int main() {
    FILE* input = fopen("in.txt", "rb");
    FILE* output = fopen("out.txt", "wb");
    unsigned char workDirection = 0;
    if (fread(&workDirection, sizeof(unsigned char), 1, input) < 1) {
        fclose(input);
        fclose(output);
        return 0;
    }
    fseek(input, 3, 0);
    if (workDirection == 'c') {
        ZipFile(input, output);
    } else if (workDirection == 'd') {
        UnzipFile(input, output);
    }
    fclose(input);
    fclose(output);
    return 0;
}

void CalculateCharacterFrequencies(FILE* file, unsigned int* tableOfFrequencies) {
    unsigned char character = 0;
    while (fread(&character, sizeof(unsigned char), 1, file) == 1) {
        ++tableOfFrequencies[(unsigned char)character];
    }
}

void BuildHuffmanTree(unsigned int* tableOfFrequencies, TTree** huffmanTree) {
    TTreeList* list = NULL;
    for (unsigned int idx = 0; idx < ASCII_SIZE; ++idx) {
        if (tableOfFrequencies[idx] != 0) {
            TCharacter character;
            CreateCharacter(&character, idx, tableOfFrequencies[idx]);
            TTree* tree = NULL;
            CreateTree(&tree, character);
            PushToTreeList(&list, tree);
        }
    }
    if (list == NULL) { 
        return;
    }
    while (list->Next != NULL) {
        SortTreeListByCharacterFrequency(&list);
        TTree* leftSon = PopFromTreeList(&list);
        TTree* rightSon = PopFromTreeList(&list);
        TTree* parent = NULL;
        TCharacter character;
        CreateCharacter(&character, 0, leftSon->Character.Frequency + rightSon->Character.Frequency);
        CreateTree(&parent, character);
        parent->LeftSubtree = leftSon;
        parent->RightSubtree = rightSon;
        PushToTreeList(&list, parent);
    }
    *huffmanTree = PopFromTreeList(&list);
}

void BuildCodeTable(TTree* huffmanTree, TCodeTable* codeTable) {
    if (huffmanTree->LeftSubtree != NULL) {
        PushToBoolList(&(codeTable->TemporaryCode), 0);
        BuildCodeTable(huffmanTree->LeftSubtree, codeTable);
    }
    if (huffmanTree->RightSubtree != NULL) {
        PushToBoolList(&(codeTable->TemporaryCode), 1);
        BuildCodeTable(huffmanTree->RightSubtree, codeTable);
    }
    if (IsLeaf(huffmanTree)) {
        if (codeTable->TemporaryCode == NULL) {
            PushToBoolList(&(codeTable->Codes[huffmanTree->Character.Value]), 0);
        } else {
            codeTable->Codes[huffmanTree->Character.Value] = ReverseBoolList(CopyBoolList(codeTable->TemporaryCode));
        }
    }
    if (codeTable->TemporaryCode != NULL) {
        PopFromBoolList(&(codeTable->TemporaryCode));
    }
}

void OverwriteFileWithNewBytes(FILE* input, FILE* output, TCodeTable codeTable) {
    unsigned char character = 0;
    TByte byte;
    ResetByte(&byte);
    while (fread(&character, sizeof(unsigned char), 1, input) == 1) {
        for (TBoolList* characterCode = codeTable.Codes[character]; characterCode != NULL; characterCode = characterCode->Next) {
            byte.Value = byte.Value | ((characterCode->Value) << (SIZE_OF_BYTE - 1 - byte.NumberOfRecordedBits));
            ++byte.NumberOfRecordedBits;
            if (byte.NumberOfRecordedBits == SIZE_OF_BYTE) {
                WriteByteToFile(byte, output);
                ResetByte(&byte);
            }
        }
    }
    if (byte.NumberOfRecordedBits != 0) {
        fseek(output, 0, 0);
        fprintf(output, "%c", byte.Value);
        fprintf(output, "%d", SIZE_OF_BYTE - byte.NumberOfRecordedBits);
    }
}

void ZipFile(FILE* input, FILE* output) {
    unsigned int tableOfFrequencies[ASCII_SIZE] = { 0 };
    CalculateCharacterFrequencies(input, tableOfFrequencies);
    TTree* huffmanTree = NULL;
    BuildHuffmanTree(tableOfFrequencies, &huffmanTree);
    if (huffmanTree == NULL) {
        return;
    }
    fprintf(output, "%c%d", 0, 0); 
    WriteTreeToFile(huffmanTree, output);
    TCodeTable codeTable;
    CreateCodeTable(&codeTable);
    BuildCodeTable(huffmanTree, &codeTable);
    fseek(input, 3, 0);
    OverwriteFileWithNewBytes(input, output, codeTable);
    DestroyTree(huffmanTree);
    DestroyCodeTable(&codeTable);
}

void RestoreFile(TTree* huffmanTree, TTree** bypassTree, TByte* byte, unsigned short extraBits, FILE* input, FILE* output) {
    while (true) {
        bool bit = ResetBitAtPositionN(byte->NumberOfRecordedBits - 1, byte);
        if (bit == 0) {
            *bypassTree = (*bypassTree)->LeftSubtree;
        } else {
            *bypassTree = (*bypassTree)->RightSubtree;
        }
        if (IsLeaf(*bypassTree)) {
            fprintf(output, "%c", (*bypassTree)->Character.Value);
            *bypassTree = huffmanTree;
        }
        if (byte->NumberOfRecordedBits - extraBits == 0) {
            if (!ReadByteFromFile(input, byte)) {
                return;
            }
        }
    }
}

void UnzipFile(FILE* input, FILE* output) {
    unsigned short extraBits = 0;
    TByte lastByte;
    ResetByte(&lastByte);
    if (!ReadByteFromFile(input, &lastByte)) {
        return;
    }
    if (fread(&extraBits, sizeof(unsigned char), 1, input) < 1) {
        return;
    }
    extraBits -= '0';
    TTree* huffmanTree = NULL;
    RestoreTreeFromFile(input, &huffmanTree);
    TTree* bypassTree = huffmanTree;
    TByte byte;
    ResetByte(&byte);
    if (ReadByteFromFile(input, &byte)) {
        RestoreFile(huffmanTree, &bypassTree, &byte, 0, input, output);
    }
    if (extraBits != 0) {
        RestoreFile(huffmanTree, &bypassTree, &lastByte, extraBits, input, output);
    }
    DestroyTree(huffmanTree);
}
