#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

typedef struct {
	unsigned int** RelationshipMatrix;
	unsigned short NumberOfVertices;
	size_t NumberOfEdges;
} TGraph;

TGraph* CreateGraph(unsigned short numberOfVertices, size_t numberOfEdges);

void DestroyGraph(TGraph** graph);

void PrintMinimalSpanningTree(TGraph* graph);

bool FillRelationshipMatrix(TGraph* graph);

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
	if (FillRelationshipMatrix(graph)) {
		PrintMinimalSpanningTree(graph);
	}
	DestroyGraph(&graph);
	return 0;
}

TGraph* CreateGraph(unsigned short numberOfVertices, size_t numberOfEdges) {
	TGraph* graph = malloc(sizeof(TGraph));
	assert(graph != NULL);
	graph->NumberOfVertices = numberOfVertices;
	graph->NumberOfEdges = numberOfEdges;
	graph->RelationshipMatrix = malloc(numberOfVertices * sizeof(unsigned int*));
	assert(graph->RelationshipMatrix != NULL);
	for (unsigned short idx = 0; idx < numberOfVertices; ++idx) {
		graph->RelationshipMatrix[idx] = malloc(numberOfVertices * sizeof(unsigned int));
		assert(graph->RelationshipMatrix[idx] != NULL);
		for (unsigned short i = 0; i < numberOfVertices; ++i) {
			graph->RelationshipMatrix[idx][i] = 0;
		}
	}
	return graph;
}

void DestroyGraph(TGraph** graph) {
	for (unsigned short idx = 0; idx < (*graph)->NumberOfVertices; ++idx) {
		free((*graph)->RelationshipMatrix[idx]);
	}
	free((*graph)->RelationshipMatrix);
	free(*graph);
}

void PrintMinimalSpanningTree(TGraph* graph) {
	size_t* minDistances = malloc(sizeof(size_t) * graph->NumberOfVertices);
	assert(minDistances != NULL);
	bool* isVisitedVertex = malloc(sizeof(bool) * graph->NumberOfVertices);
	assert(isVisitedVertex != NULL);
	short* parent = malloc(sizeof(short) * graph->NumberOfVertices);
	assert(parent != NULL);
	for (unsigned short idx = 0; idx < graph->NumberOfVertices; ++idx) {
		minDistances[idx] = UINT_MAX;
		isVisitedVertex[idx] = false;
		parent[idx] = -1;
	}
	minDistances[0] = 0;
	unsigned short numberOfEdjes = 0;
	for (unsigned short idx = 0; idx < graph->NumberOfVertices - 1; ++idx) {
		size_t minDistance = UINT_MAX;
		unsigned short visitedVertexIdx = 0;
		for (unsigned short vertex = 0; vertex < graph->NumberOfVertices; ++vertex) {
			if (!isVisitedVertex[vertex] && minDistances[vertex] < minDistance) {
				minDistance = minDistances[vertex];
				visitedVertexIdx = vertex;
			}
		}
		isVisitedVertex[visitedVertexIdx] = true;
		for (unsigned short vertex = 0; vertex < graph->NumberOfVertices; ++vertex) {
			if (graph->RelationshipMatrix[visitedVertexIdx][vertex] != 0 && !isVisitedVertex[vertex] && graph->RelationshipMatrix[visitedVertexIdx][vertex] < minDistances[vertex]) {
				if (parent[vertex] == -1) {
					++numberOfEdjes;
				}
				parent[vertex] = visitedVertexIdx;
				minDistances[vertex] = graph->RelationshipMatrix[visitedVertexIdx][vertex];
			}
		}

	}
	if (numberOfEdjes + 1 != graph->NumberOfVertices) {
		printf("no spanning tree");
	} else {
		for (unsigned short idx = 1; idx < graph->NumberOfVertices; ++idx) {
			printf("%d %d\n", parent[idx] + 1, idx + 1);
		}
	}
	free(parent);
	free(isVisitedVertex);
	free(minDistances);
}

bool FillRelationshipMatrix(TGraph* graph) {
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
		graph->RelationshipMatrix[begin - 1][end - 1] = length;
		graph->RelationshipMatrix[end - 1][begin - 1] = length;
	}
	return true;
}
