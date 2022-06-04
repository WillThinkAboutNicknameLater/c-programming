#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "BoolList.h"

void PushToBoolList(TBoolList** head, bool value) {
    TBoolList* newList = malloc(sizeof(TBoolList));
    assert(newList != NULL);
    newList->Value = value;
    newList->Next = (*head);
    (*head) = newList;
}

bool PopFromBoolList(TBoolList** head) {
    assert(head != NULL);
    TBoolList* previous = *head;
    bool value = previous->Value;
    (*head) = (*head)->Next;
    free(previous);
    return value;
}

TBoolList* ReverseBoolList(TBoolList* list) {
    TBoolList* result = NULL;
    while (list != NULL) {
        TBoolList* tmp = list->Next;
        list->Next = result;
        result = list;
        list = tmp;
    }
    return result;
}

TBoolList* CopyBoolList(const TBoolList* head) {
    if (head == NULL) {
        return NULL;
    }
    TBoolList* newHead = malloc(sizeof(TBoolList));
    assert(newHead != NULL);
    newHead->Value = head->Value;
    newHead->Next = CopyBoolList(head->Next);
    return newHead;
}

void DestroyBoolList(TBoolList** head) {
    if (*head == NULL) {
        return;
    }
    TBoolList* tmp = NULL;
    while ((*head)->Next) {
        tmp = *head;
        *head = (*head)->Next;
        free(tmp);
    }
    free(*head);
}
