#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "TreeList.h"
#include "Tree.h"

void PushToTreeList(TTreeList** head, TTree* tree) {
    TTreeList* tmp = malloc(sizeof(TTreeList));
    tmp->Tree = tree;
    tmp->Next = (*head);
    (*head) = tmp;
}

TTree* PopFromTreeList(TTreeList** head) {
    assert(head != NULL);
    TTreeList* prev = *head;
    TTree* tree = prev->Tree;
    (*head) = (*head)->Next;
    free(prev);
    return tree;
}

void SortTreeListByCharacterFrequency(TTreeList** root) {
    TTreeList* newRoot = NULL;
    while (*root != NULL) {
        TTreeList* node = *root;
        *root = (*root)->Next;
        if (newRoot == NULL || node->Tree->Character.Frequency < newRoot->Tree->Character.Frequency) {
            node->Next = newRoot;
            newRoot = node;
        } else {
            TTreeList* current = newRoot;
            while (current->Next != NULL && !(node->Tree->Character.Frequency < current->Next->Tree->Character.Frequency)) {
                current = current->Next;
            }
            node->Next = current->Next;
            current->Next = node;
        }
    }
    *root = newRoot;
}
