/*
 * File:	Tree.c
 *
 * Copyright:	Brewer, Michael
 *
 * Description: Recieve calls from an external main function and impliment
 *		a tree to store different integer values. The each node points
 *		to left, right, and it's parent.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"

/*
 *	Declaration/Definition of Tree struct
 */

typedef struct Tree{
	int letter;
	int data;
	struct Tree *parent;
	struct Tree *left;
	struct Tree *right;
}TREE;

/*
 * File:	*createTree(int data, struct Tree *left, struct Tree *right)
 *
 * Description:	Takes in an integer as data, and left and right subtrees.
 *		Subtrees can be NULL. Allocates memory for new root and
 *		sets left and right pointers. Parent pointer to NULL.
 *		Returns new root.
 *
 */

struct Tree *createTree(int data, struct Tree *left, struct Tree *right){
	TREE *temp;

	temp	= (TREE*)malloc(sizeof(TREE));
	assert(temp != NULL);

	//Check to test if input 'left' is NULL
	if(left == NULL){
		temp->left	= NULL;
	}
	else{
		temp->left	= left;
		left->parent	= temp;
	}

	//Check to test if input 'right' is NULL
	if(right == NULL){
		temp->right	= NULL;
	}
	else{
		temp->right	= right;
		right->parent	= temp;
	}

	temp->parent	= NULL;
	temp->data	= data;
	return( temp );
}

/*
 * File:	destroyTree(struct Tree *root)
 *
 * Description:	Takes a Tree pointer as input and deletes it's entire subree.
 *		Uses recursion. No return type.
 *
 */

void destroyTree(struct Tree *root){

	//Check to test base case for recursion (root == NULL)
	if(root == NULL){
		return;
	}
	else{
		destroyTree(root->left);
		destroyTree(root->right);
		free( root );
	}
}

/*
 * File:	getData(struct Tree *root)
 *
 * Description:	Take a Tree pointer as input and returns the data associated
 *		with it.
 *
 */

int getData(struct Tree *root){
	return( root->data );
}

/*
 * File:	*getLeft(struct Tree *root)
 *
 * Description:	Takes a Tree pointer as input. Returns a pointer to 
 *		it's left subtree.
 *
 */

struct Tree *getLeft(struct Tree *root){

	//Check if 'root' is leaf
	if(root->left != NULL){
		return( root->left );
	}
	else{
		return( NULL );
	}
}

/*
 * File:	*getRight(struct Tree *root)
 *
 * Description:	Takes a Tree pointer as input. Returns a pointer to
 *		it's right subtree.
 *
 */

struct Tree *getRight(struct Tree *root){

	//Check if 'root' is leaf
	if(root->right != NULL){
		return( root->right );
	}
	else{
		return( NULL );
	}
}

/*
 * File:	*getParent(struct Tree *root)
 *
 * Description: Takes a Tree pointer as input. Returns a pointer to
 *		it's parent node.
 *
 */

struct Tree *getParent(struct Tree *root){

	//Check if 'root' is root of entire tree
	if(root->parent != NULL){
		return( root->parent );
	}
	else{
		return( NULL );
	}
}

/*
 * File:	setLeft(struct Tree *root, struct Tree *left)
 *
 * Description:	Takes two tree pointers as input. One root pointer, and
 *		a left pointer. The function inserts 'left' as the
 *		left subtree of 'root'.
 *
 */

void setLeft(struct Tree *root, struct Tree *left){
	root->left	= left;
	left->parent	= root;
}

/*
 * File:	setRight(struct Tree *root, struct Tree *right)
 *
 * Description: Takes two Tree pointers as input. One root pointer, and
 *		a right pointer. The function inserts 'right' as the
 *		right subtree of 'root'.
 *
 */

void setRight(struct Tree *root, struct Tree *right){
	root->right	= right;
	right->parent	= root;
}
