#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

typedef struct {
	unsigned short Begin;
	unsigned short End;
	size_t Length;
} TEdge;

typedef struct {
	TEdge* Edges;
	unsigned short NumberOfVertices;
	size_t NumberOfEdges;
} TGraph;

TGraph* CreateGraph(unsigned short numberOfVertices, size_t numberOfEdges);

void DestroyGraph(TGraph** graph);

void PrintMinimalSpanningTree(TGraph* graph);

bool FillEdgesOfGraph(TGraph* graph);

int main() {
	unsigned short numberOfVertices = 0;
	size_t numberOfEdges = 0;
	if (scanf("%hu", &numberOfVertices) < 1) {
		return 0;
	}
	if (numberOfVertices > 5000) {
		printf("bad number of vertices");
		return 0;
	}
	if (scanf("%zu", &numberOfEdges) < 1) {
		return 0;
	}
	if (numberOfEdges > (size_t)(numberOfVertices * (numberOfVertices - 1) / 2)) {
		printf("bad number of edges");
		return 0;
	}
	if ((numberOfEdges == 0 && numberOfVertices == 0) || (numberOfEdges + 1 < numberOfVertices)) {
		printf("no spanning tree");
		return 0;
	}
	TGraph* graph = CreateGraph(numberOfVertices, numberOfEdges);
	if (FillEdgesOfGraph(graph)) {
		PrintMinimalSpanningTree(graph);
	}
	DestroyGraph(&graph);
	return 0;
}

TEdge CreateEdge(unsigned short begin, unsigned short end, size_t length) {
	TEdge edge;
	edge.Begin = begin;
	edge.End = end;
	edge.Length = length;
	return edge;
}

TGraph* CreateGraph(unsigned short numberOfVertices, size_t numberOfEdges) {
	TGraph* graph = malloc(sizeof(TGraph));
	assert(graph != NULL);
	graph->NumberOfVertices = numberOfVertices;
	graph->NumberOfEdges = numberOfEdges;
	graph->Edges = malloc(sizeof(TEdge) * numberOfEdges);
	assert(graph->Edges != NULL);
	return graph;
}

void DestroyGraph(TGraph** graph) {
	free((*graph)->Edges);
	free(*graph);
}

typedef struct {
	unsigned short* Rank;
	unsigned short* Parent;
	unsigned short NumberOfSets;
} TDsu;

TDsu* CreateDsu(unsigned short numberOfSets) {
	TDsu* dsu = malloc(sizeof(TDsu));
	assert(dsu != NULL);
	dsu->Rank = malloc(sizeof(unsigned short) * numberOfSets);
	assert(dsu->Rank != NULL);
	dsu->Parent = malloc(sizeof(unsigned short) * numberOfSets);
	assert(dsu->Parent != NULL);
	dsu->NumberOfSets = numberOfSets;
	return dsu;
}

void DestroyDsu(TDsu** dsu) {
	free((*dsu)->Rank);
	free((*dsu)->Parent);
	free(*dsu);
}

void InitializeDsu(TDsu* dsu) {
	for (unsigned short idx = 0; idx < dsu->NumberOfSets; ++idx) {
		dsu->Parent[idx] = idx;
		dsu->Rank[idx] = 0;
	}
}

void SwapEdges(TEdge* a, TEdge* b) {
	TEdge c = *a;
	*a = *b;
	*b = c;
}

size_t GetSplitIdx(TEdge* array, size_t arraySize) {
	int leftIdx = 0;
	int rightIdx = arraySize - 1;
	size_t pivot = array[rightIdx / 2].Length;
	while (leftIdx <= rightIdx) {
		while (array[leftIdx].Length < pivot) {
			++leftIdx;
		}
		while (array[rightIdx].Length > pivot) {
			--rightIdx;
		}
		if (leftIdx <= rightIdx) {
			SwapEdges(&array[leftIdx], &array[rightIdx]);
			++leftIdx;
			--rightIdx;
		}
	}
	return leftIdx;
}

void MakeQuickSort(TEdge* array, size_t arraySize) {
	size_t splitIdx = GetSplitIdx(array, arraySize);
	if (splitIdx > 1) {
		MakeQuickSort(array, splitIdx);
	}
	if (arraySize > splitIdx) {
		MakeQuickSort(array + splitIdx, arraySize - splitIdx);
	}
}

unsigned short FindSet(TDsu* dsu, unsigned short element) {
	if (element == dsu->Parent[element]) {
		return element;
	}
	dsu->Parent[element] = FindSet(dsu, dsu->Parent[element]);
	return dsu->Parent[element];
}

void UnionSets(TDsu* dsu, unsigned short firstElement, unsigned short secondElement) {
	firstElement = dsu->Parent[firstElement];
	secondElement = dsu->Parent[secondElement];
	if (firstElement != secondElement) {
		if (dsu->Rank[firstElement] < dsu->Rank[secondElement]) {
			unsigned short element = firstElement;
			firstElement = secondElement;
			secondElement = element;
		}
		dsu->Parent[secondElement] = firstElement;
		if (dsu->Rank[firstElement] == dsu->Rank[secondElement]) {
			++dsu->Rank[firstElement];
		}
	}
}

void PrintMinimalSpanningTree(TGraph* graph) {
	MakeQuickSort(graph->Edges, graph->NumberOfEdges);
	TDsu* dsu = CreateDsu(graph->NumberOfVertices);
	InitializeDsu(dsu);
	size_t* frame = malloc(sizeof(size_t) * graph->NumberOfEdges);
	assert(frame != NULL);
	size_t frameIdx = 0;
	for (size_t idx = 0; idx < graph->NumberOfEdges; ++idx) {
		if (FindSet(dsu, graph->Edges[idx].Begin) != FindSet(dsu, graph->Edges[idx].End)) {
			UnionSets(dsu, graph->Edges[idx].Begin, graph->Edges[idx].End);
			frame[frameIdx] = idx;
			++frameIdx;
		}
	}
	if (frameIdx + 1 != graph->NumberOfVertices) {
		printf("no spanning tree");
	} else {
		for (size_t idx = 0; idx < frameIdx; ++idx) {
			printf("%d %d\n", graph->Edges[frame[idx]].Begin + 1, graph->Edges[frame[idx]].End + 1);
		}
	}
	DestroyDsu(&dsu);
	free(frame);
}

bool FillEdgesOfGraph(TGraph* graph) { 
	for (size_t idx = 0; idx < graph->NumberOfEdges; ++idx) {
		unsigned short begin = 0;
		unsigned short end = 0;
		size_t length = 0;
		if (scanf("%hu %hu %zu", &begin, &end, &length) < 3) {
			printf("bad number of lines");
			return false;
		}
		if (begin < 1 || begin > graph->NumberOfVertices || end < 1 || end > graph->NumberOfVertices) {
			printf("bad vertex");
			return false;
		}
		if (length > INT_MAX) {
			printf("bad length");
			return false;
		}
		graph->Edges[idx] = CreateEdge(begin - 1, end - 1, length);
	}
	return true;
}
