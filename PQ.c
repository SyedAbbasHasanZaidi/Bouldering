
/*
*REFERENCES:
Main structure of priority queue (both .c and .h files) used from lab 8 implementation 
*/

// Implementation of a priority queue of edges


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "PQ.h"

#define DEFAULT_CAPACITY 64

struct PQRep {
	Node* items;
	int   size;
	int   capacity;
};

static void fixUp(PQ pq, int i);
static void fixDown(PQ pq, int i);
static void swap(Node* items, int i, int j);

PQ PQNew(void) {
	PQ pq = malloc(sizeof(*pq));
	if (pq == NULL) {
		fprintf(stderr, "Couldn't allocate PQ!\n");
		exit(EXIT_FAILURE);
	}
	
	pq->items = malloc((DEFAULT_CAPACITY + 1) * sizeof(struct node));
	if (pq->items == NULL) {
		fprintf(stderr, "Couldn't allocate PQ!\n");
		exit(EXIT_FAILURE);
	}
	
	pq->size = 0;
	pq->capacity = DEFAULT_CAPACITY;
	return pq;
}

void PQFree(PQ pq) {
	free(pq->items);
	free(pq);
}

void PQInsert(PQ pq, struct node e) {
	// If the PQ is full, expand it (i.e., double its capacity)
	if (pq->size == pq->capacity) {
		pq->capacity *= 2;
		pq->items = realloc(pq->items, (pq->capacity + 1) * sizeof(struct node));
		if (pq->items == NULL) {
			fprintf(stderr, "Couldn't expand PQ!\n");
			exit(EXIT_FAILURE);
		}
	}
	
	// Add the new item to the end
	pq->size++;
	pq->items[pq->size] = e;
	fixUp(pq, pq->size);
}

static void fixUp(PQ pq, int i) {
	while (i > 1 && pq->items[i].weight < pq->items[i / 2].weight) {
		swap(pq->items, i, i / 2);
		i = i / 2;
	}
}

struct node PQExtract(PQ pq) {
	assert(pq->size > 0);
	
	struct node e = pq->items[1];
	pq->items[1] = pq->items[pq->size];
	pq->size--;
	fixDown(pq, 1);
	return e;
}

static void fixDown(PQ pq, int i) {
	Node* es = pq->items;
	while (true) {
		int j = i;
		int l = 2 * i;
		int r = 2 * i + 1;
		if (l <= pq->size && es[l].weight < es[j].weight) j = l;
		if (r <= pq->size && es[r].weight < es[j].weight) j = r;
		if (j == i) break;
		swap(pq->items, i, j);
		i = j;
	}
}

bool PQIsEmpty(PQ pq) {
	return (pq->size == 0);
}

// int PQLast(PQ pq){
// 	return pq->items[pq->size - 1].weight;
// }

Node NewNode(struct rock rock){
	Node NewNode;
	NewNode.Item = rock;
	NewNode.weight = 0;
	return NewNode;
}

////////////////////////////////////////////////////////////////////////

static void swap(Node* items, int i, int j) {
	Node tmp = items[i];
	items[i] = items[j];
	items[j] = tmp;
}
