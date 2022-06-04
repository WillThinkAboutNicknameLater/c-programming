#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef enum {
	OpenBracket,
	CloseBracket,
	Number,
	Plus,
	Minus,
	Multiplication,
	Division,
	Modification,
	Power,
	BadToken
} TokenType;

typedef union {
	long long int Number;
	char Operator;
} TokenValue;

typedef struct TToken {
	TokenValue Value;
	TokenType Type;
} Token;

typedef struct TList {
	Token Token;
	struct TList* Next;
	struct TList* Previous;
} List;

typedef struct TDeque {
	List* Head;
	List* Tail;
} Deque;

void CreateDeque(Deque** deque);

void DestroyDeque(Deque** deque);

void EnterExpression(Deque* expression);

bool IsCorrectData(const Deque* expression);

void CalculateExpression(Deque* expression, long long int* result, bool* isCorrectResult);

int main() {
	Deque* expression;
	CreateDeque(&expression);
	EnterExpression(expression);
	if (IsCorrectData(expression)) {
		long long int result = 0;
		bool isCorrectResult = true;
		CalculateExpression(expression, &result, &isCorrectResult);
		if (isCorrectResult) {
			printf("%lld\n", result);
		} else {
			printf("Division by zero\n");
		}
	} else {
		printf("syntax error\n");
	}
	DestroyDeque(&expression);
	return 0;
}

void CreateDeque(Deque** deque) {
	*deque = malloc(sizeof(Deque));
	assert((*deque) != NULL);
	(*deque)->Head = NULL;
	(*deque)->Tail = NULL;
}

bool IsEmpty(const Deque* deque) {
	return (deque->Head == NULL && deque->Tail == NULL);
}

void PushFront(Deque* deque, const Token value) {
	List* prev = malloc(sizeof(List));
	assert(prev != NULL);
	prev->Token = value;
	prev->Next = deque->Head;
	prev->Previous = NULL;
	if (deque->Head) {
		deque->Head->Previous = prev;
	}
	deque->Head = prev;
	if (deque->Tail == NULL) {
		deque->Tail = prev;
	}
}

void PushBack(Deque* deque, const Token value) {
	List* next = malloc(sizeof(List));
	assert(next != NULL);
	next->Token = value;
	next->Previous = deque->Tail;
	next->Next = NULL;
	if (deque->Tail) {
		deque->Tail->Next = next;
	}
	deque->Tail = next;
	if (deque->Head == NULL) {
		deque->Head = next;
	}
}

Token GetTop(const Deque* deque) {
	assert("Deque is empty!" && !IsEmpty(deque));
	return deque->Head->Token;
}

Token GetBottom(const Deque* deque) {
	assert("Deque is empty!" && !IsEmpty(deque));
	return deque->Tail->Token;
}

Token PopFront(Deque* deque) {
	assert("Deque is empty!" && !IsEmpty(deque));
	List* prev = deque->Head;
	Token token = prev->Token;
	deque->Head = prev->Next;
	if (deque->Head) {
		deque->Head->Previous = NULL;
	}
	if (prev == deque->Tail) {
		deque->Tail = NULL;
	}
	free(prev);
	return token;
}

Token PopBack(Deque* deque) {
	assert("Deque is empty!" && !IsEmpty(deque));
	List* next = deque->Tail;
	Token token = next->Token;
	deque->Tail = next->Previous;
	if (deque->Tail) {
		deque->Tail->Next = NULL;
	}
	if (next == deque->Head) {
		deque->Head = NULL;
	}
	free(next);
	return token;
}

void DestroyDeque(Deque** deque) {
	while (!IsEmpty((*deque))) {
		PopFront(*deque);
	}
	free(*deque);
}

TokenType GetType(const char value) {
	if (value == '(') {
		return OpenBracket;
	}
	if (value == ')') {
		return CloseBracket;
	}
	if (value == '+') {
		return Plus;
	}
	if (value == '-') {
		return Minus;
	}
	if (value == '*') {
		return Multiplication;
	}
	if (value == '/') {
		return Division;
	}
	if (value == '%') {
		return Modification;
	}
	if (value == '^') {
		return Power;
	}
	if (value >= '0' && value <= '9') {
		return Number;
	}
	return BadToken;
}

bool IsOperator(const TokenType token) {
	return (token == Plus || token == Minus || token == Multiplication || token == Division || token == Modification || token == Power);
}

unsigned short int GetPriority(const TokenType tokenType) {
	assert("Bad token type!" && tokenType != BadToken);
	switch (tokenType) {
		case OpenBracket: {
			return 1;
		}
		case CloseBracket: {
			return 2;
		}
		case Plus:
			case Minus: {
				return 3;
			}
		case Multiplication:
			case Division:
				case Modification: {
					return 4;
				}
		case Power: {
			return 5;
		}
		default: {
			return 0;
		}
	}
}

long long int MakeOperation(const long long int a, const long long int b, const TokenType tokenType) {
	assert("Bad token type!" && tokenType != BadToken && tokenType != OpenBracket && tokenType != CloseBracket && tokenType != Number);
	switch (tokenType) {
		case Plus: {
			return (a + b);
		}
		case Minus: {
			return (a - b);
		}
		case Multiplication: {
			return (a * b);
		}
		case Division: {
			return (a / b);
		}
		case Modification: {
			return (a % b);
		}
		default: {
			return (pow(a, b));
		}
	}
}

Deque* MakePostfixNotation(Deque* expression) {
	Deque* deque;
	CreateDeque(&deque);
	Deque* postfixNotation;
	CreateDeque(&postfixNotation);
	while (!IsEmpty(expression)) {
		Token token = PopBack(expression);
		unsigned short int tokenPriority = GetPriority(token.Type);
		switch (token.Type) {
			case Number: {
				PushFront(postfixNotation, token);
				break;
			}
			case OpenBracket: {
				PushBack(deque, token);
				break;
			}
			case CloseBracket: {
				while (GetBottom(deque).Type != OpenBracket) {
					PushFront(postfixNotation, PopBack(deque));
				}
				PopBack(deque);
				break;
			}
			default: {
				while (!IsEmpty(deque) && GetPriority(GetBottom(deque).Type) >= tokenPriority) {
					PushFront(postfixNotation, PopBack(deque));
				}
				PushBack(deque, token);
				break;
			}
		}
	}
	while (!IsEmpty(deque)) {
		PushFront(postfixNotation, PopBack(deque));
	}
	DestroyDeque(&deque);
	return postfixNotation;
}

void CalculateExpression(Deque* expression, long long int* result, bool* isCorrectResult) {
	Deque* deque;
	CreateDeque(&deque);
	Deque* postfixNotation = MakePostfixNotation(expression);
	while (!IsEmpty(postfixNotation)) {
		Token token = PopBack(postfixNotation);
		if (token.Type == Number) {
			PushBack(deque, token);
		} else {
			long long int a = PopBack(deque).Value.Number;
			long long int b = PopBack(deque).Value.Number;
			if ((token.Type == Division || token.Type == Modification) && a == 0) {
				*isCorrectResult = false;
				DestroyDeque(&deque);
				DestroyDeque(&postfixNotation);
				return;
			}
			token.Value.Number = MakeOperation(b, a, token.Type);
			token.Type = Number;
			PushBack(deque, token);
		}
	}
	*result = PopBack(deque).Value.Number;
	DestroyDeque(&deque);
	DestroyDeque(&postfixNotation);
}

bool IsCorrectData(const Deque* expression) {
	const List* deque = expression->Tail;
	int amountOfBrackets = 0;
	int amountOfNumbers = 0;
	int amountOfOperators = 0;
	while (deque != NULL) {
		if (deque->Token.Type == BadToken) {
			return false;
		}
		if (deque->Token.Type == OpenBracket) {
			++amountOfBrackets;
		}
		if (deque->Token.Type == CloseBracket) {
			--amountOfBrackets;
			if (amountOfBrackets < 0) {
				return false;
			}
			if (deque->Next->Token.Type == OpenBracket) {
				return false;
			}
		}
		if (deque->Token.Type == Number) {
			++amountOfNumbers;
		}
		if (IsOperator(deque->Token.Type)) {
			++amountOfOperators;
		}
		deque = deque->Previous;
	}
	if (amountOfBrackets != 0) {
		return false;
	}
	if (amountOfNumbers - amountOfOperators != 1) {
		return false;
	}
	return true;
}

Token FromCharToToken(const char character) {
	Token token;
	token.Type = GetType(character);
	if (token.Type == Number) {
		token.Value.Number = character - '0';
	} else {
		token.Value.Operator = character;
	}
	return token;
}

void EnterExpression(Deque* expression) {
	char character = getchar();
	while (character != '\n') {
		Token token = FromCharToToken(character);
		if (!IsEmpty(expression) && token.Type == Number && GetTop(expression).Type == Number) {
			expression->Head->Token.Value.Number = expression->Head->Token.Value.Number * 10 + token.Value.Number;
		} else {
			PushFront(expression, token);
		}
		character = getchar();
		token = FromCharToToken(character);
	}
}
