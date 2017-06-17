/* Doubly Linked List implementation */
#include<stdio.h>
#include<stdlib.h>
#include "Token.h"

struct Node  {
	//int data;
	token *arrayOfTokens;
	struct Node* next;
	struct Node* prev;
};

struct Node* head; // global variable - pointer to head node.

//Creates a new Node and returns pointer to it. 
    struct Node* GetNewNode(token *x);

//Inserts a Node at head of doubly linked list
void InsertAtHead(token *x);

//Inserts a Node at tail of Doubly linked list
void InsertAtTail(token *x);

//Prints all the elements in linked list in forward traversal order
void Print();

//Prints all elements in linked list in reverse traversal order. 
void ReversePrint();
