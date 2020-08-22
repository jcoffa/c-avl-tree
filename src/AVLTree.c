#include "AVLTree.h"
#include "Queue.h"	// A queue is needed for breadth-first search (Levelorder traversal)


/********************************
 * "PRIVATE" INTERNAL FUNCTIONS *
 ********************************/

/*
 * There are a couple functions which aren't part of the public API of this library.
 * They all operate on the AvlNode struct, which is an implementation detail of the
 * data structure which the user does not need to concern themselves with.
 */

/*
 * Creates a new node to be added to an AvlTree struct.
 * The node contains abstracted (void *) data.
 */
static AvlNode *avlNodeNew(void *data) {
	AvlNode *toReturn = malloc(sizeof(AvlNode));

	// Can't assume malloc works every time, no matter how unlikely
	if (toReturn == NULL) {
		return NULL;
	}

	toReturn->data = data;
	toReturn->bf = Balanced;
	toReturn->parent = toReturn->left = toReturn->right = NULL;

	return toReturn;
}


/*
 * Deletes every node in the tree.
 */
static void avlClearTree_node(AvlNode *root, void (*delete)(void *)) {
	if (root == NULL) {
		return;
	}

	avlClearTree_node(root->left, delete);
	avlClearTree_node(root->right, delete);
	delete(root->data);
	free(root);
}


/*
 * Returns the node containing the smallest value in the tree.
 */
static AvlNode *avlFindMin_node(AvlNode *root, int (*compare)(const void *, const void *)) {
	if (root == NULL) {
		return NULL;
	}

	AvlNode *cur = root;
	while (cur->left != NULL) {
		cur = cur->left;
	}

	return cur;
}


/*
 * Returns the node containing the largest value in the tree.
 */
static AvlNode *avlFindMax_node(AvlNode *root, int (*compare)(const void *, const void *)) {
	if (root == NULL) {
		return NULL;
	}

	AvlNode *cur = root;
	while (cur->right != NULL) {
		cur = cur->right;
	}

	return cur;
}


/*
 * Returns the height of the node.
 */
#define maxl(x,y) (x > y ? x : y)
static long avlGetHeight_node(AvlNode *root) {
	if (root == NULL) {
		return 0;
	}

	return  1 + maxl(avlGetHeight_node(root->left), avlGetHeight_node(root->right));
}


/*
 * Returns the first node in the tree that is equivalent to searchRecord when using the
 * custom comparison function compare.
 *
 * Returns NULL if no such node is found.
 */
static AvlNode *avlFind_node(AvlNode *root, int (*compare)(const void *, const void *), const void *searchRecord) {
	if (root == NULL) {
		return NULL;
	}

	int cmp = compare(root->data, searchRecord);

	if (cmp < 0) {
		return avlFind_node(root->right, compare, searchRecord);
	} else if (cmp > 0) {
		return avlFind_node(root->left, compare, searchRecord);
	} else { // cmp == 0
		return root;
	}
}


/*
 * A family of functions which turn an AVL tree into a string representation.
 * The traversal method determines what order the nodes are visited in.
 */
static char *avlToString_Preorder(AvlNode *root, char *(printData)(void*)) {return NULL;}
static char *avlToString_Inorder(AvlNode *root, char *(printData)(void*)) {return NULL;}
static char *avlToString_Postorder(AvlNode *root, char *(printData)(void*)) {return NULL;}
static char *avlToString_Levelorder(AvlNode *root, char *(printData)(void*), Queue *queue) {return NULL;}


/*
 * A family of functions which executes a function on every element in an AVL tree.
 * The traversal method determines what order the nodes are visited in.
 */
static void avlMapTree_Preorder(AvlNode *root, void (*func)(void *)) {
	if (root == NULL) return;
	func(root->data);
	avlMapTree_Preorder(root->left, func);
	avlMapTree_Preorder(root->right, func);
}

static void avlMapTree_Inorder(AvlNode *root, void (*func)(void *)) {
	if (root == NULL) return;
	avlMapTree_Inorder(root->left, func);
	func(root->data);
	avlMapTree_Inorder(root->right, func);
}

static void avlMapTree_Postorder(AvlNode *root, void (*func)(void *)) {
	if (root == NULL) return;
	avlMapTree_Postorder(root->left, func);
	avlMapTree_Postorder(root->right, func);
	func(root->data);
}

static void avlMapTree_Levelorder(AvlNode *root, void (*func)(void *), Queue *queue) {
	if (root == NULL) return;
	enqueue(queue, root);
	AvlNode *node;
	while (!queueIsEmpty(queue)) {
		node = dequeue(queue);
		func(node);
		if (node->left != NULL) enqueue(queue, node->left);
		if (node->right != NULL) enqueue(queue, node->right);
	}
}



/**************************
 * "PUBLIC" API FUNCTIONS *
 **************************/

AvlTree *avlTreeNew(char *(*printFunc)(void *), void (*deleteFunc)(void *), int (*compareFunc)(const void *, const void *)) {
	if (printFunc == NULL || deleteFunc == NULL || compareFunc == NULL) {
		return NULL;
	}

	AvlTree *toReturn = malloc(sizeof(AvlTree));

	// Can't assume malloc works every time, no matter how unlikely
	if (toReturn == NULL) {
		return NULL;
	}

	toReturn->root = NULL;
	toReturn->printData = printFunc;
	toReturn->deleteData = deleteFunc;
	toReturn->compare = compareFunc;

	return toReturn;
}


void avlClearTree(AvlTree *tree) {
	if (tree == NULL) {
		return;
	}

	avlClearTree_node(tree->root, tree->deleteData);
}


void avlChopTree(AvlTree *tree) {
	if (tree == NULL) {
		return;
	}

	avlClearTree(tree);
	free(tree);
}


void avlInsert(AvlTree *tree, void *data);


void *avlFindMin(AvlTree *tree) {
	if (tree == NULL) {
		return NULL;
	}

	AvlNode *ret = avlFindMin_node(tree->root, tree->compare);
	return ret != NULL ? ret->data : NULL;
}


void *avlFindMax(AvlTree *tree) {
	if (tree == NULL) {
		return NULL;
	}

	AvlNode *ret = avlFindMax_node(tree->root, tree->compare);
	return ret != NULL ? ret->data : NULL;
}

// Maybe we sacrifice some memory to store each node's height within the AvlNode struct,
// because this function is O(N) time complexity right now.
long getHeight(AvlTree *tree) {
	if (tree == NULL) {
		return -1;
	}

	return avlGetHeight_node(tree->root);
}


void avlDelete(AvlTree *tree, void *data);


void *avlFind(AvlTree *tree, int (*compare)(const void *, const void *), const void *searchRecord) {
	if (tree == NULL) {
		return NULL;
	}

	AvlNode *ret = avlFind_node(tree->root, compare, searchRecord);
	return ret != NULL ? ret->data : NULL;
}


/* These dummy functions are needed to initialize a Queue for the Levelorder traversal.
 * The queue is only used to enqueue and dequeue elements (theres no deletion or printing)
 * so its fine that these are only dummy functions that don't do anything.
 */
static inline void dummyQueueDelete(void *a) {return;}
static inline char *dummyQueuePrint(void *a) {return NULL;}

char *avlToString(AvlTree *tree, TraversalType traversal) {
	char *toReturn;

	if (tree == NULL) {
		toReturn = malloc(sizeof(char));
		toReturn[0] = '\0';
		return toReturn;
	}

	Queue *queue;
	switch (traversal) {
		case Preorder:
			toReturn = avlToString_Preorder(tree->root, tree->printData);
			break;

		case Inorder:
			toReturn = avlToString_Inorder(tree->root, tree->printData);
			break;

		case Postorder:
			toReturn = avlToString_Postorder(tree->root, tree->printData);
			break;

		case Levelorder:
			queue = queueNew(dummyQueueDelete, dummyQueuePrint);
			toReturn = avlToString_Levelorder(tree->root, tree->printData, queue);
			queueFree(queue);
			break;
	}

	return toReturn;
}


void avlPrintTree(AvlTree *tree, TraversalType traversal) {
	char *toPrint = avlToString(tree, traversal);
	printf("%s\n", toPrint);
	free(toPrint);
}


void avlMapTree(AvlTree *tree, TraversalType traversal, void (*func)(void *)) {
	if (tree == NULL || func == NULL) {
		return;
	}

	Queue *queue;
	switch(traversal) {
		case Preorder:
			avlMapTree_Preorder(tree->root, func);
			break;

		case Inorder:
			avlMapTree_Inorder(tree->root, func);
			break;

		case Postorder:
			avlMapTree_Postorder(tree->root, func);
			break;

		case Levelorder:
			// Use the same dummy functions as avlToString()
			queue = queueNew(dummyQueueDelete, dummyQueuePrint);
			avlMapTree_Levelorder(tree->root, func, queue);
			queueFree(queue);
			break;
	}
}

