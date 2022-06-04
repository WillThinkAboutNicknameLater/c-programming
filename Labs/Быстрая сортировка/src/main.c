#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void MakeQuickSort(int* array, const unsigned int arraySize);

void PrintArray(const int* array, const unsigned int arraySize);

int main() {
	unsigned int arraySize = 0;
	if (scanf("%u", &arraySize) < 1) {
		return 0;
	}
	int* array = (int*)malloc(arraySize * sizeof(int));
	for (unsigned int index = 0; index < arraySize; ++index) {
		if (scanf("%d", &array[index]) < 1) {
			free(array);
			return 0;
		}
	}
	MakeQuickSort(array, arraySize);
	PrintArray(array, arraySize);
	free(array);
	return 0;
}

void PrintArray(const int* array, const unsigned int arraySize) {
	for (unsigned int idx = 0; idx < arraySize; ++idx) {
		printf("%d ", array[idx]);
	}
	printf("\n");
}

void Swap(int* a, int* b) { //Swap
	int c = *a;
	*a = *b;
	*b = c;
}

unsigned int GetSplitIdx(int* array, const unsigned int arraySize) {
	int leftIdx = 0;
	int rightIdx = arraySize - 1;
	int pivot = array[rightIdx / 2];
	while (leftIdx <= rightIdx) {
		while (array[leftIdx] < pivot) {
			++leftIdx;
		}
		while (array[rightIdx] > pivot) {
			--rightIdx;
		}
		if (leftIdx <= rightIdx) {
			Swap(&array[leftIdx], &array[rightIdx]);
			++leftIdx;
			--rightIdx;
		}
	}
	return leftIdx;
}

void MakeQuickSort(int* array, const unsigned int arraySize) {
	unsigned int splitIdx = GetSplitIdx(array, arraySize);
	if (splitIdx > 1) {
		MakeQuickSort(array, splitIdx);
	}
	if (arraySize > splitIdx) {
		MakeQuickSort(array + splitIdx, arraySize - splitIdx);
	}
}
