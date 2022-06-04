#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

typedef struct {
	unsigned int** RelationshipMatrix;
	unsigned int NumberOfVertices;
	unsigned int NumberOfEdges;
} TGraph;

TGraph* CreateGraph(unsigned int numberOfVertices, unsigned int numberOfEdges);

void DestroyGraph(TGraph** graph);

bool FillRelationshipMatrix(TGraph* graph);

void MakeDijkstraAlgorithm(const TGraph* graph, unsigned int begin, unsigned int end);

int main() {
	unsigned int numberOfVertices = 0;
	unsigned int numberOfEdges = 0;
	unsigned int beginningOfPath = 0;
	unsigned int endOfPath = 0;
	if (scanf("%u", &numberOfVertices) < 1) {
		return 0;
	}
	if (numberOfVertices > 5000) {
		printf("bad number of vertices");
		return 0;
	}
	if (scanf("%u %u", &beginningOfPath, &endOfPath) < 1) {
		return 0;
	}
	if (beginningOfPath < 1 || beginningOfPath > numberOfVertices || endOfPath < 1 || endOfPath > numberOfVertices) {
		printf("bad vertex");
		return 0;
	}
	if (scanf("%u", &numberOfEdges) < 1) {
		return 0;
	}
	if (numberOfEdges > (unsigned int)(numberOfVertices * (numberOfVertices - 1) / 2)) {
		printf("bad number of edges");
		return 0;
	}
	TGraph* graph = CreateGraph(numberOfVertices, numberOfEdges);
	if (FillRelationshipMatrix(graph)) {
		MakeDijkstraAlgorithm(graph, beginningOfPath - 1, endOfPath - 1);
	}
	DestroyGraph(&graph);
	return 0;
}

unsigned long long* GetMinDistancesToVertices(const TGraph* graph, unsigned int from) {
	unsigned int numberOfVertices = graph->NumberOfVertices;
	unsigned long long* minDistances = malloc(sizeof(unsigned long long) * numberOfVertices);
	assert(minDistances != NULL);
	bool* isVisitedVertex = malloc(sizeof(bool) * numberOfVertices);
	assert(isVisitedVertex != NULL);
	for (unsigned int idx = 0; idx < numberOfVertices; ++idx) {
		minDistances[idx] = UINT_MAX;
		isVisitedVertex[idx] = false;
	}
	minDistances[from] = 0;
	while (true) {
		int visitedVertexIdx = -1;
		unsigned long long minDistance = UINT_MAX;
		for (unsigned int idx = 0; idx < numberOfVertices; ++idx) {
			if (!isVisitedVertex[idx] && minDistances[idx] < minDistance) {
				minDistance = minDistances[idx];
				visitedVertexIdx = idx;
			}
		}
		if (visitedVertexIdx == -1) {
			break;
		}
		for (unsigned int idx = 0; idx < numberOfVertices; ++idx) {
			if (graph->RelationshipMatrix[visitedVertexIdx][idx] != 0) {
				unsigned long long temp = minDistance + graph->RelationshipMatrix[visitedVertexIdx][idx];
				if (temp < minDistances[idx]) {
					minDistances[idx] = temp;
				}
			}
		}
		isVisitedVertex[visitedVertexIdx] = true;
	}
	free(isVisitedVertex);
	return minDistances;
}

bool PathExists(const unsigned long long* minDistances, unsigned int to) {
	return minDistances[to] != UINT_MAX;
}

void PrintMinDistances(const unsigned long long* minDistances, unsigned int numberOfVertices) {
	for (unsigned int idx = 0; idx < numberOfVertices; ++idx) {
		if (minDistances[idx] == UINT_MAX) {
			printf("oo ");
		} else if (minDistances[idx] > INT_MAX) {
			printf("INT_MAX+ ");
		} else {
			printf("%llu ", minDistances[idx]);
		}
	}
	printf("\n");
}

void PrintTheShortestPath(const int* path, unsigned int numberOfVertices) {
	for (unsigned int idx = 0; idx < numberOfVertices && path[idx] != -1; ++idx) {
		printf("%d ", path[idx] + 1);
	}
	printf("\n");
}

int* GetTheShortestPath(const TGraph* graph, unsigned int begin, unsigned int end, const unsigned long long* minDistances) {
	unsigned int numberOfVertices = graph->NumberOfVertices;
	int* path = malloc(sizeof(int) * numberOfVertices);
	assert(path != NULL);
	for (unsigned int idx = 0; idx < numberOfVertices; ++idx) {
		path[idx] = -1;
	}
	unsigned int pathIndex = 0;
	path[pathIndex++] = end;
	unsigned long long lengthOfPath = minDistances[end];
	while (end != begin) {
		for (unsigned int idx = 0; idx < numberOfVertices; ++idx) {
			if (graph->RelationshipMatrix[idx][end] != 0) {
				unsigned long long temp = lengthOfPath - graph->RelationshipMatrix[idx][end];
				if (temp == minDistances[idx]) { // один путь найден
					unsigned int count = 1;
					for (unsigned int j = idx + 1; j < numberOfVertices; ++j) { // ищем другие пути (возможно неправильно понял)
						if (graph->RelationshipMatrix[j][end] != 0) {
							if (lengthOfPath - graph->RelationshipMatrix[j][end] == minDistances[j] && lengthOfPath > INT_MAX) {
								++count;
							}
						}
					}
					if (count > 1) {
						free(path);
						return NULL;
					}
					lengthOfPath = temp;
					end = idx;
					path[pathIndex++] = idx;
				}
			}
		}
	}
	return path;
}

TGraph* CreateGraph(unsigned int numberOfVertices, unsigned int numberOfEdges) {
	TGraph* graph = malloc(sizeof(TGraph));
	assert(graph != NULL);
	graph->NumberOfVertices = numberOfVertices;
	graph->NumberOfEdges = numberOfEdges;
	graph->RelationshipMatrix = malloc(numberOfVertices * sizeof(unsigned int*));
	assert(graph->RelationshipMatrix != NULL);
	for (unsigned int idx = 0; idx < numberOfVertices; ++idx) {
		graph->RelationshipMatrix[idx] = malloc(numberOfVertices * sizeof(unsigned int));
		assert(graph->RelationshipMatrix[idx] != NULL);
		for (unsigned int i = 0; i < numberOfVertices; ++i) {
			graph->RelationshipMatrix[idx][i] = 0;
		}
	}
	return graph;
}

void DestroyGraph(TGraph** graph) {
	for (unsigned int idx = 0; idx < (*graph)->NumberOfVertices; ++idx) {
		free((*graph)->RelationshipMatrix[idx]);
	}
	free((*graph)->RelationshipMatrix);
	free(*graph);
}

bool FillRelationshipMatrix(TGraph* graph) {
	for (unsigned int idx = 0; idx < graph->NumberOfEdges; ++idx) {
		unsigned int begin = 0;
		unsigned int end = 0;
		unsigned int length = 0;
		if (scanf("%u %u %u", &begin, &end, &length) < 3) {
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

void MakeDijkstraAlgorithm(const TGraph* graph, unsigned int begin, unsigned int end) {
	unsigned int numberOfVertices = graph->NumberOfVertices;
	unsigned long long* minDistances = GetMinDistancesToVertices(graph, begin);
	PrintMinDistances(minDistances, numberOfVertices);
	if (PathExists(minDistances, end)) {
		int* path = GetTheShortestPath(graph, begin, end, minDistances);
		if (path != NULL) {
			PrintTheShortestPath(path, numberOfVertices);
		} else {
			printf("overflow");
		}
		free(path);
	} else {
		printf("no path");
	}
	free(minDistances);
}
