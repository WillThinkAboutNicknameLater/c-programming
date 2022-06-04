#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void Swap(char* a, char* b) { //Swap
	char c = *a;
	*a = *b;
	*b = c;
}

void Reverse_String(char* str) { //Переворот строки
	size_t str_len = strlen(str);
	for (size_t index = 0; index < str_len / 2; ++index) {
		Swap(&str[index], &str[str_len - index - 1]);
	}
}

void Convert_String_To_Upper_Case(char* str) { //Перевод строки в верхний регистр
	for (size_t index = 0; str[index] != '\0'; ++index) {
		str[index] = toupper(str[index]);
	}
}

bool Is_Point_Present(const char* str) { //Проверка на наличие точки
	return strchr(str, '.') != NULL;
}

bool Is_Good_Character(char c) {
	if ((c >= 'A' && c <= 'Z') || (c == '.') || (c >= '0' && c <= '9')) {
		return true;
	}
	return false;
}

void Remove_Insignificant_Zero(char* str) { //Удаление незначащих нулей
	if (Is_Point_Present(str)) {
		size_t index = strlen(str) - 1;
		while (str[index] == '0') {
			str[index] = '\0';
			--index;
		}
	}
}

int From_Character_To_Value(char c) {
	if (c - '0' < 10) {
		return (c - '0');
	} else {
		return (c - 'A' + 10);
	}
}

char From_Value_To_Character(int value) {
	if (value < 10) {
		return (value + '0');
	} else {
		return (value + 'A' - 10);
	}
}

long double Parse_Long_Double(const char* num, int original_ns) {
	char* point_ptr = strchr(num, '.');
	int integer_part_size = point_ptr == NULL ? (int)strlen(num) : point_ptr - num;  //Количество символов числа до точки
	long double num_in10 = 0; //Возвращаемое число, переведенное в 10 (yt ,tqnt yfcxtn 10) с.с.
	for (size_t index = 0; num[index] != '\0'; ++index) {
		if (num[index] != '.') {
			num_in10 += From_Character_To_Value(num[index]) * pow(original_ns, integer_part_size - 1); //Возводим цифру в степень
			--integer_part_size;
		}
	}
	return num_in10;
}

void Create_Long_Double_For_Numeral(char* numeral, long double num, int final_ns) {
	long long int n_part = (long long int)num; //Целая часть числа
	long double f_part = num - n_part; //Дробная часть числа
	int next_digit_idx = 0, fractional_part_size = 13;
	bool point = Is_Point_Present(numeral);
	memset(numeral, 0, strlen(numeral)); //Обнуление строки
	if (n_part == 0) {
		numeral[0] = '0';
		next_digit_idx++;
	}
	while (n_part != 0) { //Перевод целой части
		numeral[next_digit_idx] = From_Value_To_Character(n_part % final_ns);
		n_part /= final_ns;
		next_digit_idx++;
	}
	Reverse_String(numeral);
	int len_npart = strlen(numeral) + 1;
	if (point) {
		numeral[next_digit_idx] = '.';
	}
	next_digit_idx++;
	while (f_part != 0 && next_digit_idx != fractional_part_size + len_npart) { //Перевод дробной части
		numeral[next_digit_idx] = From_Value_To_Character(f_part * final_ns);
		f_part = (f_part * final_ns) - (int)(f_part * final_ns);
		next_digit_idx++;
	}
	Remove_Insignificant_Zero(numeral);
}

bool Data_Validation(char* num, int o_ns, int f_ns) {
	size_t num_of_points = 0;
	for (size_t index = 0; num[index] != '\0'; index++) {
		if (!Is_Good_Character(num[index])) {
			return false;
		}
		if (From_Character_To_Value(num[index]) >= o_ns) {
			return false;
		}
		if (num[index] == '.') {
			num_of_points++;
		}
	}
	if (num[strlen(num) - 1] == '.' || num[0] == '.') {
		return false;
	}
	if (o_ns < 2 || o_ns > 16 || f_ns < 2 || f_ns > 16 || num_of_points > 1) {
		return false;
	}
	return true;
}

int main() {
	int original_ns, final_ns;
	char number[256] = { 0 };
	if (scanf("%d %d", &original_ns, &final_ns) < 2) {
		return -1;
	}
	if (scanf("%13s", number) < 1) {
		return -1;
	}
	Convert_String_To_Upper_Case(number);
	if (Data_Validation(number, original_ns, final_ns)) {
		Create_Long_Double_For_Numeral(number, Parse_Long_Double(number, original_ns), final_ns);
		printf("%s", number);
	} else {
		printf("bad input\n");
	}
	return 0;
}
