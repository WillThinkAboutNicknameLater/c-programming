#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define _CRT_SECURE_NO_WARNINGS
#define ASCII 256
#define MaxLengthOfTemplate 18

typedef struct TString {
	char* data;
	unsigned short int length;
} String;

void MakeStopCharacterTable(unsigned short int* stopCharacterTable, const String template);

void MakeBoyerMoore(const String string, const String template, const unsigned int textIdx, const unsigned short int* stopTable);

void DeleteExcessLineFeed(String* string); //Удаление лишнего перевода строки

void CreateString(unsigned int size, String* string);

void DestroyString(String* string);

int main() {
	String template;
	CreateString(MaxLengthOfTemplate, &template);
	if (fgets(template.data, MaxLengthOfTemplate, stdin) == NULL) {
		DestroyString(&template);
		return 0;
	}
	template.length = strlen(template.data);
	DeleteExcessLineFeed(&template);
	unsigned short int stopCharacterTable[ASCII];
	MakeStopCharacterTable(stopCharacterTable, template);
	unsigned int textIdx = 0;
	String text;
	CreateString(ASCII, &text);
	while (fgets(text.data, ASCII, stdin) != NULL) {
		text.length = strlen(text.data);
		if (template.length > text.length) {
			printf("%u ", text.length + textIdx + 1);
		} else {
			MakeBoyerMoore(text, template, textIdx, stopCharacterTable);
		}
		textIdx += text.length;
	}
	DestroyString(&template);
	DestroyString(&text);
	return 0;
}

void CreateString(unsigned int size, String* string) {
	string->data = (char*)malloc(sizeof(char) * size);
	assert(string->data != NULL);
}

void DestroyString(String* string) {
	free(string->data);
}

void MakeStopCharacterTable(unsigned short int* stopCharacterTable, const String template) {
	for (unsigned short int idx = 0; idx < ASCII; ++idx) {
		stopCharacterTable[idx] = template.length;
	}
	for (unsigned short int idx = 0; idx + 1 < template.length; ++idx) {
		stopCharacterTable[(unsigned char)template.data[idx]] = template.length - idx - 1;
	}
}

void MakeBoyerMoore(const String string, const String template, const unsigned int textIdx, const unsigned short int* stopTable) {
	unsigned short int templateIdx = template.length - 1;
	unsigned short int stringIdx = templateIdx;
	while (stringIdx < string.length) {
		printf("%u ", stringIdx + textIdx + 1);
		if (template.data[templateIdx] == string.data[stringIdx] && templateIdx != 0) {
			--templateIdx;
			--stringIdx;
		} else {
			stringIdx += template.length - templateIdx - 1 + stopTable[(unsigned char)string.data[stringIdx - templateIdx - 1 + template.length]];
			templateIdx = template.length - 1;
		}
	}
}

void DeleteExcessLineFeed(String* string) {
	if (string->data[string->length - 1] == '\n') {
		string->data[string->length - 1] = '\0';
		--string->length;
	}
}
