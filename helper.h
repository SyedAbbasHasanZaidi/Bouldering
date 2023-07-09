#ifndef HELPER_H
#define HELPER_H


#include "Wall.h"
#include "Queue.h"
#include "PQ.h"
#include "climber.h"


//*MACROS
#define visited(rock) visited[rock.row][rock.col]
#define predecessor(rock) predecessor[rock.row][rock.col]
#define WeightMatrix(rock) WeightMatrix[rock.row][rock.col]
#define weight(node) node.weight
#define row(node) node.Item.row
#define col(node) node.Item.col

//HELPER FUNCTIONS
/*
* Returns a boolean value depnding on if the rock is valid and not a dummy/default rock
*/
bool ValidRocks(struct rock);
/*
* Creates a two dimensional matrix of different types for use in algorithm such as BSF and Djikstra
* Cost matrix - Visitor matrix - Predecessor matrix
*/
int** CreateWeightMatrix(int , int);
bool** CreateBoolMatrix(int , int);  
struct rock** CreatePredMatrix(int , int);
/*
* Functions to free the different 2 dimensional matricies instantiated 
*/
void FreeBoolMatrix(int , bool**);
void FreePredMatrix(int , struct rock**);
void FreeWeightMatrix(int , int**);
/*
* Finds all the potential starts and adds them into a queue marking them 
* as visited and setting there predecessor to the default/dummy rock  
*/
void AllRocksStartQ(Wall, Queue, int, Colour, bool**, struct rock**);
/*
* Finds all the potential starts and adds them into a array     
*/
int AllRocksStartArr(Wall, int, struct rock[]);
/*
* Flips a array of rocks (interms of order) and stores them as a path       
*/
void FlipPath(struct rock[], struct path*);
/*
* Uses the predecessor and cost matrix to find the least costly path in terms of energy
*/
struct rock FindMinEnergyPath(Wall w,int reach,struct rock* RockArr,int** WeightMatrix);
/*
* Uses the predecessor and cost matrix to find the least costly path in terms of turns
*/
struct rock FindMinTurnPath(Wall w,int reach,int energyCosts[], struct rock* RockArr,int** WeightMatrix,struct rock** predecessor);
/*
* Free's all malloced items past in as arguements
*/
void FreeAll(Wall w,struct rock RockArr[],int** WeightMatrix, struct rock** predecessor, bool** visited, PQ pq);
/*
* Uses weight and predecessor matrix to find the least costly methos to each node from a strating point  
*/
Node EdgeRelaxation(Node curr,Node neighbour,int value,int**WeightMatrix,struct rock** predecessor);
/*
* Marks the node as visitied than enqueues it in the priority queue 
*/
void MarkVisited(Node neighbour,bool** visited,PQ pq);
/*
* Saftely insert a value an automatically resizes array if index exceeds capcity - 1;
*/
int SafeInsert(struct rock** arr, int* capacity, int index, struct rock val);
#endif
    