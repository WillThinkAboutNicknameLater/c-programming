#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void Print_Next_Permutations(char* permutation, unsigned int permutations_count); //Вывод следующей перестановки

bool Is_Permutation(const char* permutation); //Проверка правильности данных

int main() {
	char permutation[11] = { 0 };
	unsigned int permutations_count = 0;
	if (scanf("%10s", permutation) < 1) {
		return -1;
	}
	if (scanf("%u", &permutations_count) < 1) {
		return -1;
	}
	if (Is_Permutation(permutation)) {
		Print_Next_Permutations(permutation, permutations_count);
	}
	else {
		printf("bad input");
	}
	return 0;
}

void Swap(char* a, char* b) { //Swap
	char c = *a;
	*a = *b;
	*b = c;
}

void Reverse_String(char* str) { //Переворот строки
	unsigned int str_len = strlen(str);
	for (unsigned int index = 0; index < str_len / 2; ++index) {
		Swap(&str[index], &str[str_len - index - 1]);
	}
}

int Get_Min_Element_Idx_Greater_N(const char* array, char n) { //Индекс минимального элемента в массиве
	char min_element = '9';
	int min_element_idx = -1;
	unsigned int array_size = strlen(array);
	for (unsigned int idx = 0; idx < array_size; ++idx) {
		if (array[idx] <= min_element && array[idx] > n) {
			min_element = array[idx];
			min_element_idx = idx;
		}
	}
	return min_element_idx;
}

void Make_Next_Permutation(char* permutation) { //Перестановки
	unsigned int permutation_length = strlen(permutation);
	for (int idx = permutation_length - 2; idx >= 0; --idx) {
		if (permutation[idx] < permutation[idx + 1]) {
			Swap(&permutation[idx], &permutation[Get_Min_Element_Idx_Greater_N(permutation + idx + 1, permutation[idx]) + idx + 1]);
			Reverse_String(permutation + idx + 1);
			return;
		}
	}
}

bool Is_Last_Permutation(const char* permutation) { //Проверка на последнюю перестановку
	unsigned int permutation_length = strlen(permutation);
	for (unsigned int idx = 0; idx + 1 < permutation_length; ++idx) {
		if (permutation[idx] < permutation[idx + 1]) {
			return false;
		}
	}
	return true;
}

void Print_Next_Permutations(char* permutation, unsigned int permutations_count) { //Вывод следующей перестановки
	for (unsigned int idx = 0; idx < permutations_count; ++idx) {
		if (Is_Last_Permutation(permutation)) {
			return;
		}
		Make_Next_Permutation(permutation);
		printf("%s\n", permutation);
	}
}

unsigned int Character_Count_In_String(const char* str, const char sought_element) { //Количество повторяющихся символов в строке
	unsigned int sought_element_count = 0, str_length = strlen(str);
	for (unsigned int idx = 0; idx < str_length; ++idx) {
		if (str[idx] == sought_element) {
			++sought_element_count;
		}
	}
	return sought_element_count;
}

bool Is_Permutation(const char* permutation) { //Проверка правильности данных
	unsigned int permutation_length = strlen(permutation);
	for (unsigned int idx = 0; idx < permutation_length; ++idx) { //Проверка на посторонние символы
		if (permutation[idx] < '0' || permutation[idx] > '9') {
			return false;
		}
	}
	for (unsigned int idx = 0; idx < 10; ++idx) { //Проверка на повторяющиеся символы
		if (Character_Count_In_String(permutation, idx + '0') > 1) {
			return false;
		}
	}
	return true;
}
