/*
*REFERENCES:
Main structure of priority queue (both .c and .h files) used from lab 8 implementation 
*/

// Priority queue of edges
// Edges with smaller weight have higher priority

#ifndef PQ_H
#define PQ_H

#include <stdbool.h>

#include "Wall.h"

struct node{
    struct rock Item;
    int weight;
};

typedef struct node Node;
typedef struct PQRep *PQ;

/**
 * Creates a new priority queue
 * Complexity: O(1)
 */
PQ   PQNew(void);

/**
 * Frees all memory associated with the given priority queue
 * Complexity: O(1)
 */
void PQFree(PQ pq);

/**
 * Adds an edge to the priority queue
 * Amortised complexity: O(log n)
 */
void PQInsert(PQ pq, struct node e);

/**
 * Removes and returns the edge with the smallest weight from the
 * priority queue. If multiple edges have the same smallest weight, one
 * of them will be removed.
 * Complexity: O(log n)
 */
Node PQExtract(PQ pq);

/**
 * Returns true if the given priority queue is empty, or false otherwise
 */
bool PQIsEmpty(PQ pq);

// /**
//  * Prints the given priority queue to stdout for debugging purposes
//  */
struct node NewNode(struct rock rock);

// int PQLast(PQ pq);
#endif
