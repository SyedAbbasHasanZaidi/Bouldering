/*REFERENCES:
Main structure of queue (both .c and .h files) used from lab 7 implementation 
*/

// Interface to the Queue ADT

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdio.h>

#include "Wall.h"

typedef struct queue *Queue;

/**
 * Creates a new empty queue
 * Complexity: O(1)
 */
Queue QueueNew(void);

/**
 * Frees all resources associated with the given queue
 * Complexity: O(n)
 */
void QueueFree(Queue q);

/**
 * Adds an item to the end of the queue
 * Complexity: O(1)
 */
void QueueEnqueue(Queue q, struct rock it);

/**
 * Removes an item from the front of the queue and returns it
 * Assumes that the queue is not empty
 * Complexity: O(1)
 */
struct rock QueueDequeue(Queue q);

/**
 * Returns true if the queue is empty, and false otherwise
 * Complexity: O(1)
 */
bool QueueIsEmpty(Queue q);

#endif

