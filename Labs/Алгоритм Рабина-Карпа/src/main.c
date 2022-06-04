#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define _CRT_SECURE_NO_WARNINGS
#define RABIN_KARP_NUMBER 3
#define MAX_LENGTH_OF_TEMPLATE 18
#define MAX_LENGTH_OF_TEXT 50

typedef struct {
	const char* Data;
	unsigned int DataLength;
	int ViewStart;
	unsigned int ViewLength;
	unsigned int ViewHash;
} StringView;

void DeleteExcessLineFeed(char* string); //Удаление лишнего перевода строки

void MakeRabinKarp(const StringView template, StringView text, const unsigned long long int index, const unsigned long long int* degreeTable);

void CreateView(StringView* view, const char* data, unsigned int dataLength, unsigned int viewLength, const unsigned long long int* degreeTable);

unsigned int GetHash(StringView view);

unsigned int GetStringHash(const StringView string, const unsigned long long int* degreeTable, unsigned short int size);

void MoveForward(StringView* view, const unsigned long long int* degreeTable);

void MakeDegreeTable(unsigned long long int* array, const unsigned short int arraySize);

void DestroyDegreeTable(unsigned long long int* degreeTable);

int main() {
	char template[MAX_LENGTH_OF_TEMPLATE];
	if (fgets(template, MAX_LENGTH_OF_TEMPLATE, stdin) == NULL) {
		return 0;
	}
	DeleteExcessLineFeed(template);
	unsigned int templateLength = strlen(template);
	unsigned long long int* degreeTable = (unsigned long long int*)malloc(sizeof(unsigned long long int) * templateLength);
	MakeDegreeTable(degreeTable, templateLength);
	StringView templateView;
	CreateView(&templateView, template, templateLength, templateLength, degreeTable);
	printf("%u ", GetHash(templateView));
	unsigned long long int index = 0;
	while (true) {
		char text[MAX_LENGTH_OF_TEXT];
		if (fgets(text, MAX_LENGTH_OF_TEXT, stdin) == NULL) {
			break;
		}
		unsigned int textLength = strlen(text);
		StringView textView;
		CreateView(&textView, text, textLength, templateLength, degreeTable);
		if (textLength >= templateLength) {
			MakeRabinKarp(templateView, textView, index, degreeTable);
		}
		index += textLength;
	}
	DestroyDegreeTable(degreeTable);
	return 0;
}

void CreateView(StringView* view, const char* data, unsigned int dataLength, unsigned int viewLength, const unsigned long long int* degreeTable) {
	view->Data = data;
	view->DataLength = dataLength;
	view->ViewStart = 0;
	view->ViewLength = viewLength;
	view->ViewHash = GetStringHash(*view, degreeTable, viewLength);
}

unsigned int GetHash(StringView view) {
	return view.ViewHash;
}

void MoveForward(StringView* view, const unsigned long long int* degreeTable) {
	view->ViewHash = (view->ViewHash / RABIN_KARP_NUMBER) + ((unsigned char)view->Data[view->ViewStart + view->ViewLength] % RABIN_KARP_NUMBER) * degreeTable[view->ViewLength - 1];
	view->ViewStart += 1;
}

void MakeRabinKarp(const StringView template, StringView text, const unsigned long long int index, const unsigned long long int* degreeTable) {
	unsigned int templateViewHash = GetHash(template);
	while (text.ViewStart + text.ViewLength <= text.DataLength) {
		if (GetHash(text) == templateViewHash) {
			for (unsigned int idx = 0; idx < template.ViewLength; ++idx) {
				printf("%llu ", index + 1 + text.ViewStart + idx);
				if (text.Data[text.ViewStart + idx] != template.Data[idx]) {
					break;
				}
			}
		}
		MoveForward(&text, degreeTable);
	}
}

void MakeDegreeTable(unsigned long long int* array, unsigned short int arraySize) {
	array[0] = 1;
	for (int idx = 1; idx < arraySize; ++idx) {
		array[idx] = array[idx - 1] * RABIN_KARP_NUMBER;
	}
}

unsigned int GetStringHash(const StringView string, const unsigned long long int* degreeTable, unsigned short int size) {
	unsigned int hash = 0;
	for (unsigned short int idx = 0; idx < size; ++idx) {
		hash += ((unsigned char)string.Data[idx] % RABIN_KARP_NUMBER) * degreeTable[idx];
	}
	return hash;
}

void DestroyDegreeTable(unsigned long long int* degreeTable) {
	free(degreeTable);
}

void DeleteExcessLineFeed(char* string) {
	unsigned short int stringLength = strlen(string);
	if (string[stringLength - 1] == '\n') {
		string[stringLength - 1] = '\0';
	}
}
