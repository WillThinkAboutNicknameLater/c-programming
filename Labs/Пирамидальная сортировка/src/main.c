#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void Swap(int* a, int* b) { //Swap
	int c = *a;
	*a = *b;
	*b = c;
}

int Get_Max_Child_Idx(const int* array, const unsigned int heap_size, const unsigned int idx) { //Индекс максимального элемента из двух
	if (idx + 1 < heap_size) { //Проверка на вхождение элемента в кучу
		if (array[idx] < array[idx + 1]) {
			return idx + 1;
		}
	}
	return idx;
}

void Sieve_From_Index(int* array, const unsigned int heap_size, unsigned int idx) { //Просеивание
	while (idx * 2 + 1 < heap_size) {
		unsigned int max_idx = Get_Max_Child_Idx(array, heap_size, idx * 2 + 1); //Определяем индекс максимального элемента
		if (array[idx] < array[max_idx]) {
			Swap(&array[idx], &array[max_idx]); //Меняем местами исходный элемент с максимальным
			idx = max_idx;
		} else {
			return;
		}
	}
}

void Heap_Sort(int* array, const unsigned int array_size) { //Сортировка
	for (unsigned int heap_size = array_size; heap_size > 1; --heap_size) {
		Swap(&array[0], &array[heap_size - 1]); //Меняем местами первый и последний элемент кучи
		Sieve_From_Index(array, heap_size - 1, 0); //Просеиваем 0-вой элемент кучи
	}
}

void Make_Heap(int* array, const unsigned int array_size) { //Построение кучи исходного массива
	for (int idx = array_size / 2 - 1; idx >= 0; --idx) {
		Sieve_From_Index(array, array_size, idx);
	}
}

int main() {
	unsigned int array_size = 0;
	if (scanf("%u", &array_size) < 1) {
		return -1;
	}
	int* array = malloc(array_size * sizeof(int));
	for (unsigned int index = 0; index < array_size; ++index) {
		if (scanf("%d", &array[index]) < 1) {
			return -1;
		}
	}
	Make_Heap(array, array_size);
	Heap_Sort(array, array_size);
	for (unsigned int index = 0; index < array_size; ++index) {
		printf("%d ", array[index]);
	}
	free(array);
	return 0;
}
