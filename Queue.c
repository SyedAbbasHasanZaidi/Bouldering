/*
*REFERENCES:
Main structure of queue (both .c and .h files) used from lab 7 implementation 
*/


// Implementation of the Queue ADT using a linked list

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

typedef struct node *Node;
struct node {
	struct rock item;
	Node next;
};

struct queue {
	Node head;
	Node tail;
	int  size;
};

static Node newNode(struct rock it);

/**
 * Creates a new empty queue
 */
Queue QueueNew(void) {
	Queue q = malloc(sizeof(*q));
	if (q == NULL) {
		fprintf(stderr, "couldn't allocate Queue\n");
		exit(EXIT_FAILURE);
	}
	
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	return q;
}

/**
 * Frees all resources associated with the given queue
 */
void QueueFree(Queue q) {
	Node curr = q->head;
	while (curr != NULL) {
		Node temp = curr;
		curr = curr->next;
		free(temp);
	}
	free(q);
}

/**
 * Adds an item to the end of the queue
 */
void QueueEnqueue(Queue q, struct rock it) {
	Node n = newNode(it);
	if (q->size == 0) {
		q->head = n;
	} else {
		q->tail->next = n;
	}
	q->tail = n;
	q->size++;
}

static Node newNode(struct rock it) {
	Node n = malloc(sizeof(*n));
	if (n == NULL) {
		fprintf(stderr, "couldn't allocate Node\n");
		exit(EXIT_FAILURE);
	}
	
	n->item = it;
	n->next = NULL;
	return n;
}

/**
 * Removes an item from the front of the queue and returns it
 * Assumes that the queue is not empty
 */
struct rock QueueDequeue(Queue q) {
	assert(q->size > 0);
	
	Node newHead = q->head->next;
	struct rock it = q->head->item;
	free(q->head);
	q->head = newHead;
	if (newHead == NULL) {
		q->tail = NULL;
	}
	q->size--;
	return it;
}

/**
 * Returns true if the queue is empty, and false otherwise
 */
bool QueueIsEmpty(Queue q) {
	return q->size == 0;
}

