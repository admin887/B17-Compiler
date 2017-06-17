/* Doubly Linked List implementation */
#include<stdio.h>
#include<stdlib.h>
#include "DoublyLinkedList.h"

 struct Node* GetHeadNode()
 {
	return head;
 }
 struct Node* GetNewNode(token *x) {
	struct Node* newNode= (struct Node*)malloc(sizeof(struct Node));
	if(newNode!=NULL)
	{
		newNode->arrayOfTokens = x;
		newNode->prev = NULL;
		newNode->next = NULL;
	}
	return newNode;
}
//Inserts a Node at head of doubly linked list
void InsertAtHead(token *x) {
	struct Node* newNode = GetNewNode(x);
	if(head == NULL) {
		head = newNode;
		return;
	}
	head->prev = newNode;
	newNode->next = head; 
	head = newNode;
}

//Inserts a Node at tail of Doubly linked list
void InsertAtTail(token *x) {
	struct Node* temp = head;
	struct Node* newNode = GetNewNode(x);
	if(head == NULL) {
		head = newNode;
		return;
	}
	while(temp->next != NULL) temp = temp->next; // Go To last Node
	temp->next = newNode;
	newNode->prev = temp;
}