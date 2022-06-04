#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct TTree {
	int Value;
	unsigned int Height;
	struct TTree* LeftSubtree;
	struct TTree* RightSubtree;
} Tree;

Tree* AddLeaf(Tree* tree, const int value);

unsigned int GetTreeHeight(const Tree* tree);

void RemoveTree(Tree* tree);

int main() {
	unsigned int numberOfVertices = 0;
	if (scanf("%u", &numberOfVertices) < 1) {
		return 0;
	}
	Tree* tree = NULL;
	for (unsigned int idx = 0; idx < numberOfVertices; ++idx) {
		int value = 0;
		if (scanf("%d", &value) < 1) {
			return 0;
		}
		tree = AddLeaf(tree, value);
	}
	printf("%u\n", GetTreeHeight(tree));
	RemoveTree(tree);
	return 0;
}

Tree* CreateTree(const int value) {
	Tree* tree = (Tree*)malloc(sizeof(Tree));
	assert(tree != NULL);
	tree->Value = value;
	tree->Height = 1;
	tree->LeftSubtree = NULL;
	tree->RightSubtree = NULL;
	return tree;
}

bool IsEmpty(const Tree* tree) {
	return (tree == NULL);
}

void RemoveTree(Tree* tree) {
	if (!IsEmpty(tree)) {
		RemoveTree(tree->LeftSubtree);
		RemoveTree(tree->RightSubtree);
		free(tree);
	}
}

unsigned int GetTreeHeight(const Tree* tree) {
	if (IsEmpty(tree)) {
		return 0;
	}
	return tree->Height;
}

int GetMax(const int a, const int b) {
	if (a > b) {
		return a;
	}
	return b;
}

void SetTreeHeight(Tree* tree) {
	unsigned int leftHeight = GetTreeHeight(tree->LeftSubtree);
	unsigned int rightHeight = GetTreeHeight(tree->RightSubtree);
	tree->Height = GetMax(leftHeight, rightHeight) + 1;
}

int GetBalanceFactor(const Tree* tree) {
	return GetTreeHeight(tree->RightSubtree) - GetTreeHeight(tree->LeftSubtree);
}

Tree* LeftRotation(Tree* leftTree) {
	Tree* rightTree = leftTree->RightSubtree;
	leftTree->RightSubtree = rightTree->LeftSubtree;
	rightTree->LeftSubtree = leftTree;
	SetTreeHeight(leftTree);
	SetTreeHeight(rightTree);
	return rightTree;
}

Tree* RightRotation(Tree* rightTree) {
	Tree* leftTree = rightTree->LeftSubtree;
	rightTree->LeftSubtree = leftTree->RightSubtree;
	leftTree->RightSubtree = rightTree;
	SetTreeHeight(rightTree);
	SetTreeHeight(leftTree);
	return leftTree;
}

Tree* BalanceTree(Tree* tree) {
	SetTreeHeight(tree);
	if (GetBalanceFactor(tree) == 2) {
		if (GetBalanceFactor(tree->RightSubtree) < 0) {
			tree->RightSubtree = RightRotation(tree->RightSubtree);
		}
		return LeftRotation(tree);
	}
	if (GetBalanceFactor(tree) == -2) {
		if (GetBalanceFactor(tree->LeftSubtree) > 0) {
			tree->LeftSubtree = LeftRotation(tree->LeftSubtree);
		}
		return RightRotation(tree);
	}
	return tree;
}

Tree* AddLeaf(Tree* tree, const int value) {
	if (IsEmpty(tree)) {
		return CreateTree(value);
	} else {
		if (value < tree->Value) {
			tree->LeftSubtree = AddLeaf(tree->LeftSubtree, value);
		} else {
			tree->RightSubtree = AddLeaf(tree->RightSubtree, value);
		}
	}
	return BalanceTree(tree);
}
