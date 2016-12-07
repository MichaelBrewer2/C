/*
 *	File:		huffman.c
 *
 *	Copyright:	2015, Michael Brewer
 *
 *	Description:	Simple implimentation of huffman compression algorithm.
 *			Reads a .txt file and compresses it into a .zip file.
 *			Prints out the compression of each letter to show it working.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "tree.h"
#include "pack.h"

/*	Subject to change for different files */
#define MAX_TREE_HT 100

/*	Struct Tree definition	*/
typedef struct Tree{
	int letter;
	int data;
	struct Tree *parent;
	struct Tree *left;
	struct Tree *right;
}TREE;

/*	Global variables definition	*/
TREE *leaves[257];
TREE *heap[257];
int heap_count;
int *code;
int top;

/*	Functions definition	*/
void balanceTree(int index);
TREE *removeFirst( );
void reverseBalance(int index);
void insert(struct Tree *new_tree);
void printCode(struct Tree *root, int *code, int top);
void printBits(int top);
int isLeaf(struct Tree *root);

/*
 *	Function:	int main(int argc, char *argv[ ])
 *
 *	Purpose:	main driver for program.
 *
 */

int main(int argc, char *argv[ ]){
	FILE *fp;
	char input;
	int ch, i;
	TREE *temp_left, *temp_right, *new_tree;

	/*	Points to file to be read	*/
	fp	= fopen(argv[1], "r");
	assert(fp != NULL);
	
	/*	
	 *	Allocates memory for int array that prints binary
	 *	codes.
	 */

	code	= malloc(sizeof(int)*MAX_TREE_HT);
	assert(code != NULL);

	leaves[256]		= createTree(0, NULL, NULL);
	leaves[256]->letter	= 256;

	/*	Loops through and inputs characters one by one into leaves[ ]	*/
	while(fscanf(fp, "%c", &input) != EOF){
		ch	= (int)input;
		
		/*	If spot is NULL, new tree is created	*/
		if(leaves[ch] == NULL){
			leaves[ch]		= createTree(1, NULL, NULL);
			leaves[ch]->letter	= ch;
		}
		else{
			leaves[ch]->data++;
		}
	}

	heap_count	= 0;

	/*	Inputs leaves[ ] into heap[ ]	*/
	for(i = 0; i < 257; i++){
		
		/*	Checks that leaves[i] is not NULL	*/
		if(leaves[i] != NULL){
			heap[heap_count]	= leaves[i];
			balanceTree(heap_count);
			heap_count++;
		}
	}

	/*	
	 *	Removes first two spots in heap[ ]. Using those two as children creates new
	 *	tree and inserts it back into heap[ ]. Rebalance.
 	 */

	while(heap_count > 1){
		temp_left		= removeFirst( );
		temp_right		= removeFirst( );
		new_tree		= createTree((temp_left->data + temp_right->data), temp_left, temp_right);
		new_tree->letter	= (int)'$';
		insert( new_tree );
	}
	
	top	= 0;	
	printCode(heap[0], code, top);

	pack(argv[1], argv[2], leaves);

	fclose( fp );
return(EXIT_SUCCESS);
}

/*
 *	Function:	void balanceTree(int index)
 *
 *	Purpose:	Takes and index and checks it agains parent. Swaps them if parent
 *			is larger. Function calls itself.
 *
 */

void balanceTree(int index){
	TREE *temp;

	/*	Check index against parent. Or index == 0	*/
	if((heap[index]->data > heap[(index - 1)/2]->data) || ((index - 1)/2 == 0)){
		return;
	}
	else{
		temp			= heap[(index - 1)/2];
		heap[(index - 1)/2]	= heap[index];
		heap[index]		= temp;
		balanceTree((index - 1)/2);
	}
}

/*
 *	Function:	TREE *removeFirst( )
 *
 *	Purpose:	Removes first spot in heap[ ] into temp replaces it with last spot.
 *			Rebalances and returns temp.
 *
 */

TREE *removeFirst( ){
	TREE *temp;

	temp	= heap[0];
	heap[0]	= heap[heap_count - 1];
	heap_count--;
	reverseBalance( 0 );

	return( temp );	
}

/*
 *	Function:	void reverseBalance(int index)
 *
 *	Purpose:	Takes and index and checks against its smallest child. Defaults to
 *			right child if childtren are equal. Function calls itself.
 *
 */

void reverseBalance(int index){
	TREE *temp;

	/*	Check index of left child smaller than heap_count	*/
	if((2*index + 1) >= heap_count){
		return;
	}

	/*	Check left child smaller than right child	*/
	if(heap[2*index + 1]->data < heap[2*index + 2]->data){
		temp			= heap[2*index + 1];
		heap[2*index + 1]	= heap[index];
		heap[index]		= temp;
		reverseBalance(2*index + 1);
	}
	else{
		temp			= heap[2*index + 2];
		heap[2*index + 2]	= heap[index];
		heap[index]		= temp;
		reverseBalance(2*index + 2);
	}
}

/*
 *	Function:	void insert(struct Tree *new_tree)
 *
 *	Purpose:	Takes a tree pointer and inserts it into last spot in heap[ ].
 *			Increments heap_count and rebalances tree.
 *
 */

void insert(struct Tree *new_tree){
	heap[heap_count]	= new_tree;
	balanceTree(heap_count);
	heap_count++;
}

/*
 *	Function:	void printCode(struct Tree *root, int *code, int top)
 *
 *	Purpose:	Takes the root of a tree and traverses down each branch to each child.
 *			Left path = 0 right path = 1. Prints out the binary code associated
 *			with each leaf.
 *
 */

void printCode(struct Tree *root, int *code, int top){
	/*	Check if left child equal to NULL	*/
	if(root->left != NULL){
		code[top]	= 0;
		printCode(root->left, code, top + 1);
	}
	
	/*	Check if right child equal to NULL	*/
	if(root->right != NULL){
		code[top]	= 1;
		printCode(root->right, code, top + 1);
	}
	
	/*	Check if leaf node	*/
	if(isLeaf(root)){
		/*	Check if character of node is printable	*/
		if(isprint(root->letter)){
			printf("%c:	%d --> ", root->letter, root->data);
		}
		else{
			printf("%03o:	%d --> ", root->letter, root->data);
		}
		printBits(top);
	}
}

/*
 *	Function:	int isLeaf(struct Tree *root)
 *
 *	Purpose:	Takes a Tree pointer and checks to see if it is a leaf node.
 *			Returns int.
 */
int isLeaf(struct Tree *root){
	return(!(root->left) && !(root->right));
}

/*
 *	Function: 	void printBits(int top)
 *
 *	Purpose:	Takes the larges number of bits as input. Prints the bit string.
 *
 */

void printBits(int top){
	int i;
	
	/*	Loops through *code and prints each element	*/
	for(i = 0; i < top; i++){
		printf("%d", code[i]);
	}
	printf("\n");
}
