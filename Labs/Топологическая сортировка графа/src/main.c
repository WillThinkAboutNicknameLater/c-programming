#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

typedef enum {
	White,
	Gray,
	Black
} VertexColour;

typedef struct TStack {
	size_t* Data;
	size_t Size;
	size_t Top;
} TStack;

typedef struct {
	char Text[30];
	bool IsActive;
} TError;

typedef struct {
	TStack** RelationshipMatrix;
	VertexColour* VertexLabels;
	TError TError;
	size_t NumberOfVertices;
	size_t NumberOfEdges;
} TGraph;

TGraph* CreateGraph(size_t numberOfVertices, size_t numberOfEdges);

void FillRelationshipMatrix(TGraph* graph);

TStack* GetTopSort(TGraph* graph);

void PrintStack(TStack* stack);

void DestroyGraph(TGraph** graph);

void DestroyStack(TStack** stack);

int main() {
	size_t numberOfVertices = 0;
	size_t numberOfEdges = 0;
	if (scanf("%zu %zu", &numberOfVertices, &numberOfEdges) < 2) {
		printf("bad number of lines");
		return 0;
	}
	if (numberOfVertices > 1000) {
		printf("bad number of vertices");
		return 0;
	}
	if (numberOfEdges > numberOfVertices* (numberOfVertices - 1) / 2) {
		printf("bad number of edges");
		return 0;
	}
	TGraph* graph = CreateGraph(numberOfVertices, numberOfEdges);
	FillRelationshipMatrix(graph);
	TStack* stack = GetTopSort(graph);
	if (!graph->TError.IsActive) {
		PrintStack(stack);
	} else {
		printf("%s\n", graph->TError.Text);
	}
	DestroyStack(&stack);
	DestroyGraph(&graph);
	return 0;
}

TStack* CreateStack() {
	TStack* out = malloc(sizeof(TStack));
	assert(out != NULL);
	out->Size = 10;
	out->Data = malloc(out->Size * sizeof(size_t));
	assert(out->Data != NULL);
	out->Top = 0;
	return out;
}

void ResizeStack(TStack* stack) {
	stack->Size *= 2;
	stack->Data = realloc(stack->Data, stack->Size * sizeof(size_t));
	assert(stack->Data != NULL);
}

void Push(TStack* stack, size_t value) {
	if (stack->Top >= stack->Size) {
		ResizeStack(stack);
	}
	stack->Data[stack->Top] = value;
	++stack->Top;
}

size_t Pop(TStack* stack) {
	assert(stack->Top != 0);
	--stack->Top;
	return stack->Data[stack->Top];
}

void DestroyStack(TStack** stack) {
	free((*stack)->Data);
	free(*stack);
	*stack = NULL;
}

void PrintStack(TStack* stack) {
	for (int idx = stack->Top - 1; idx >= 0; --idx) {
		printf("%zu ", stack->Data[idx]);
	}
	printf("\n");
}

TGraph* CreateGraph(size_t numberOfVertices, size_t numberOfEdges) {
	TGraph* graph = malloc(sizeof(TGraph));
	assert(graph != NULL);
	graph->NumberOfVertices = numberOfVertices;
	graph->NumberOfEdges = numberOfEdges;
	graph->VertexLabels = malloc(numberOfVertices * sizeof(VertexColour));
	assert(graph->VertexLabels != NULL);
	graph->RelationshipMatrix = malloc(numberOfVertices * sizeof(TStack*));
	assert(graph->RelationshipMatrix != NULL);
	for (size_t idx = 0; idx < numberOfVertices; ++idx) {
		graph->VertexLabels[idx] = White;
		graph->RelationshipMatrix[idx] = CreateStack();
	}
	strcpy(graph->TError.Text, "None");
	graph->TError.IsActive = false;
	return graph;
}

void FillRelationshipMatrix(TGraph* graph) {
	for (size_t idx = 0; idx < graph->NumberOfEdges; ++idx) {
		size_t beginnigOfEdge = 0;
		size_t endOfEdge = 0;
		if (scanf("%zu %zu", &beginnigOfEdge, &endOfEdge) < 2) {
			strcpy(graph->TError.Text, "bad number of lines");
			graph->TError.IsActive = true;
			return;
		}
		if (beginnigOfEdge < 1 || beginnigOfEdge > graph->NumberOfVertices || endOfEdge < 1 || endOfEdge > graph->NumberOfVertices) {
			strcpy(graph->TError.Text, "bad vertex");
			graph->TError.IsActive = true;
			return;
		}
		Push((graph->RelationshipMatrix[beginnigOfEdge - 1]), endOfEdge - 1);
	}
}

void DestroyGraph(TGraph** graph) {
	for (size_t idx = 0; idx < (*graph)->NumberOfVertices; ++idx) {
		DestroyStack(&((*graph)->RelationshipMatrix[idx]));
	}
	free((*graph)->RelationshipMatrix);
	free((*graph)->VertexLabels);
	free(*graph);
}

void MakeDepthFirstSearch(TGraph* graph, size_t idx, TStack* stack) {
	graph->VertexLabels[idx] = Gray;
	while (graph->RelationshipMatrix[idx]->Top != 0) {
		size_t nextVertex = Pop(graph->RelationshipMatrix[idx]);
		if (graph->VertexLabels[nextVertex] == Gray) {
			strcpy(graph->TError.Text, "impossible to sort");
			graph->TError.IsActive = true;
			return;
		}
		if (graph->VertexLabels[nextVertex] == White) {
			MakeDepthFirstSearch(graph, nextVertex, stack);
		}
	}
	graph->VertexLabels[idx] = Black;
	Push(stack, idx + 1);
}

TStack* GetTopSort(TGraph* graph) {
	TStack* stack = CreateStack();
	assert(stack != NULL);
	for (size_t idx = 0; idx < graph->NumberOfVertices; ++idx) {
		if (!graph->TError.IsActive) {
			if (graph->VertexLabels[idx] == White) {
				MakeDepthFirstSearch(graph, idx, stack);
			}
		}
	}
	return stack;
}
