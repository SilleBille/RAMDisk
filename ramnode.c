/*
 * ramnode.c
 *
 *  Created on: Nov 15, 2016
 *      Author: dinesh
 */

#include "ramnode.h"
#include "test.h"

void addNode(ramNode *head, ramNode *node) {
	node->next = NULL;

	if (head == NULL) {
		head = node;
	} else {
		ramNode *temp = head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = node;
	}
}

ramNode* searchNode(ramNode *head, const char *name) {
	ramNode *temp = head;
	while (temp != NULL && strcmp(name, temp->name) != 0)
		temp = temp->next;
	if (temp != NULL)
		return temp;
	else
		return NULL;
}

void printNodes(ramNode *head) {
	printf("MKD: Nodes are: ");
	ramNode *temp = head;
	while(temp != NULL) {
		printf(" %s \n", temp->name);
		temp = temp->next;
	}
}

int computeSize(memBlock *mHead) {
	memBlock *t = mHead;
	int sizeOfFile = 0;
	while(t != NULL) {
		sizeOfFile += strlen(t->data);
		t = t->next;
	}
	printf("Size is: %d\n", sizeOfFile);
	return sizeOfFile;
}



