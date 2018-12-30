#include<klee/klee.h>
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
struct node{
  struct  node* leftChild;
  struct  node* rightChild;
  int value;
};

struct node* SearchNode(struct node* root, int value) {
	if (root->value < value) {
		return SearchNode(root->rightChild, value);
	}
	else if (root->value > value){
		return SearchNode(root->leftChild, value);
	}
	return root;
}

void  DelNode(struct node* root,  int value) {
	struct node* parent = root;
	struct node* current = root;
	while (current->value != value) {
		if (current ->value <= value) {
			parent = current;
			current  = current->rightChild;
		}
		else {
			parent = current;
			current = current->leftChild;
		}
	}
	if (current->leftChild == NULL || current->rightChild == NULL) {
		struct node* temp = current->leftChild;
		if (temp == NULL) {
			temp = current->rightChild;
		}
		if (current->value < parent->value) {
			parent->leftChild = temp;
		}
		else {
			parent->rightChild = temp;
		}
	}
	else {
		struct node* leftMax = current->leftChild;
		struct node* leftMaxPar = current;
		while(leftMax->rightChild != NULL) {
			leftMaxPar = leftMax;
			leftMax = leftMax->rightChild;
		}
		leftMaxPar->rightChild = NULL;
		current->value = leftMax->value;
	}
}

struct node* Insert(struct node* root, int value) {
	if(root== NULL) {
		struct node* rt = (struct node*)malloc(sizeof(struct node));
		rt->value = value;
		rt->leftChild = NULL;
		rt->rightChild = NULL;
		return rt;
	}
	if (value >= root->value) {
		if (root->rightChild == NULL) {
			struct node* right = (struct node*)malloc(sizeof(struct node));
			right->value = value;
			right->rightChild = NULL;
			right->leftChild = NULL;
			root->rightChild = right;
			return right;
		}
		else {
			return Insert(root->rightChild, value);
		}
	}
	else {
		if (root->leftChild == NULL) {
			struct node* left = (struct node*)malloc(sizeof(struct node));
			left->value = value;
			left->rightChild = NULL;
			left->leftChild = NULL;
			root->leftChild = left;
			return left;
		}
		else {
			return Insert(root->leftChild, value);
		}
	}
}

int length(struct node* root, int count) {
	if(root!=NULL) {
		count += 1;
		count = length(root->leftChild, count);
		count = length(root->rightChild, count);
	}
	return count;
}

void display(struct node* root) {
	if (root != NULL) {
		display(root->leftChild);
		printf("%d,",root->value);
		display(root->rightChild);
	}
}

int main() {
	int testcase;
	klee_make_symbolic(&testcase, sizeof(testcase), "testcaseindex");
	if (testcase == 1) {
	// fault-1
	int a;
	klee_make_symbolic(&a, sizeof(a), "root value1");
	struct node* rt1 = Insert(NULL, a);
	DelNode(rt1, a);
	if (rt1 != NULL) {
		klee_assert(0);
	}
	}
	if (testcase == 2) {
	// fault-2
	int b;
	klee_make_symbolic(&b, sizeof(b), "root value2");
	struct node* rt2 = Insert(NULL, b);
	Insert(rt2, b-10);
	DelNode(rt2, b);
	if (length(rt2, 0) != 1) {
		klee_assert(0);
	}
	
	}
	
	if (testcase == 3) {
	// fault-3
	int c;
	klee_make_symbolic(&c, sizeof(c), "root value3");
        struct node* rt3 = Insert(NULL, c);
	Insert(rt3, c-10);
	Insert(rt3, c-20);
	Insert(rt3, c-30);
	Insert(rt3, c-15);
	DelNode(rt3, c-20);
	if (length(rt3, 0) != 4) {
		klee_assert(0);
	}
	}
	
	if (testcase == 4) {
        // fault-4
	int d;
        klee_make_symbolic(&d, sizeof(d), "root value3");
        DelNode(NULL, d);

	}
	
	if(testcase == 5) {
	// fault-5
	int e;
        klee_make_symbolic(&e, sizeof(e), "root value3");
        struct node* rt5 = Insert(NULL, e);
        Insert(rt5, e);
	SearchNode(rt5, 10000);
	}
	return 0;
}
