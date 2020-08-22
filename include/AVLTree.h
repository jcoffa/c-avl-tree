#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* This data structure is a little tricky to implement!
 * You're gonna need these:
 * 	https://en.wikipedia.org/wiki/AVL_tree
 * 	https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
 * 	https://rosettacode.org/wiki/AVL_tree/C
 * 	https://docs.google.com/document/d/1GHwPjEvO5jE22kmBUDCBDyqwyR_pM540ekS5ibvYrIc/edit?usp=sharing
 */


/**********************************
 * CONSTANTS, MACROS, ENUMS, etc. *
 **********************************/

/*
 * Used in certain functions to specify the traversal order that should be used.
 */
typedef enum {
	Preorder, Inorder, Postorder, Levelorder
} TraversalType;


/***********
 * STRUCTS *
 ***********/

typedef enum {
	LeftTooHeavy = -2,
	LeftHeavy = -1,
	Balanced = 0,
	RightHeavy = 1,
	RightTooHeavy = 2
} BalanceFactor;

/**************
 * STRUCTURES *
 **************/

/*
 * A node stored by the tree.
 * Contains the stored data and all information necessary for maintaining an AVL Tree.
 */
typedef struct treeNode {
    void *data;
	BalanceFactor bf;
	struct treeNode *parent;
    struct treeNode *left;
    struct treeNode *right;
} AvlNode;


/*
 * Metadata root of the tree. 
 * Contains no actual data but contains the tree's root node
 * as well as the function pointers for working with the abstracted tree data.
 */
typedef struct treeHead {
    AvlNode *root;
    char *(*printData)(void* toPrint);
    void (*deleteData)(void* toDelete);
    int (*compare)(const void* first, const void* second);
} AvlTree;


/*************
 * FUNCTIONS *
 *************/

/*
 * Function to initialize the AVL Tree metadata head to the appropriate function pointers.
 * Allocates memory to the struct, unless any of the function pointers are NULL. In this
 * case, NULL is returned instead and no memory is allocated.
 *
 * The AvlTree struct provides an interface to a generic collection of data. The three
 * function pointers allow the struct to print, delete, and compare its data.
 * The (void *) arguments are to be casted into their proper data type (i.e. whatever
 * data type the tree will be holding) and do as follows:
 *
 *  char *printFunc(void *toPrint) : return a string representation of `toPrint`
 *  void deleteFunc(void *toDelete) : free all memory associated with `toDelete`
 *  int comapreFunc(const void *a, const void *b) : compare `a` and `b`, utilizing the same
 *                                                  logic as `strcmp`
 *
 * Examples of these functions are provided for string (char *) data in the README.
 */
AvlTree *avlTreeNew(char *(*printFunc)(void *), void (*deleteFunc)(void *), int (*compareFunc)(const void *, const void *));


/*
 * Removes every node from an AvlTree struct without deleting the struct itself.
 */
void avlClearTree(AvlTree *tree);


/*
 * Frees all memory associated with an AvlTree struct, including the struct itself.
 */
void avlChopTree(AvlTree *tree);


/*
 * Creates an AvlNode to store the provided data, and then inserts the node into
 * the provided AvlTree struct.
 * The tree is rebalanced if necessary after insertion.
 */
void avlInsert(AvlTree *tree, void *data);


/*
 * Returns the data with the smallest value in the tree, or NULL if the tree is empty.
 */
void *avlFindMin(AvlTree *tree);


/*
 * Returns the data with the largest value in the tree, or NULL if the tree is empty.
 */
void *avlFindMax(AvlTree *tree);


/*
 * Returns the height of the AvlTree, or -1 if the tree is NULL.
 */
long avlHeight(AvlTree *tree);


/*
 * Finds the first AvlNode with the provided data and removes it from the tree
 * before freeing all memory associated with the found node.
 * The comparison uses the `comapreFunc` function pointer inside the AvlTree struct
 * to determine which direction to traverse the tree and if the right node has been found.
 */
void avlDelete(AvlTree *tree, void *data);


/*
 * Returns the data in the first element in the tree that matches the searchRecord data
 * according to the comparison function.
 *
 * NULL is returned on a failure (i.e. no node that satisfied the criteria was found)
 * or if the tree was NULL.
 */
void *avlFind(AvlTree *tree, int (*compare)(const void *, const void *), const void *searchRecord);


/*
 * Returns a string representation of the AvlTree using the tree's
 * `printData` function pointer to create the string.
 *
 * The string must be freed by the calling function after use.
 */
char *avlToString(AvlTree *tree);


/*
 * A convenient alias for printing the string returned by `avlToString(tree, traversal)`
 * and then freeing the string that was created after printing it.
 * A newline is printed after the tree-string is done printing.
 */
void avlPrintTree(AvlTree *tree);


/*
 * Execute a function `func` on each element in the tree using
 * the desired traversal method.
 */
void avlMapTree(AvlTree *tree, TraversalType traversal, void (*func)(void *));

#endif

