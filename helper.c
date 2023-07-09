#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "helper.h"


bool ValidRocks(struct rock obj){
    return obj.colour != -1;
}

int** CreateWeightMatrix(int row,int col){
    int** WeightMatrix = malloc(row*sizeof(*WeightMatrix));

    if (WeightMatrix == NULL) {
		fprintf(stderr, "Couldn't allocate PQ!\n");
		exit(EXIT_FAILURE);
	}


    for(int i = 0; i < row; i++){
        WeightMatrix[i] = malloc(col*sizeof(int));
        for(int j = 0; j < col; j++){
            WeightMatrix[i][j] = INT_MAX;
        }
    }

    return WeightMatrix;
}

bool** CreateBoolMatrix(int row, int col){
    bool** BoolMatrix = malloc(row*sizeof(*BoolMatrix));
    
    if (BoolMatrix== NULL) {
		fprintf(stderr, "Couldn't allocate PQ!\n");
		exit(EXIT_FAILURE);
	}

    for(int i = 0; i < row; i++){
        BoolMatrix[i] = malloc(col*sizeof(bool));
        for(int j = 0; j < col; j++){
            BoolMatrix[i][j] = false;
        }
    }

    return BoolMatrix;
}  

struct rock** CreatePredMatrix(int row, int col){
    struct rock** PredRockMatrix = malloc(row*sizeof(*PredRockMatrix));
    
    if (PredRockMatrix == NULL) {
		fprintf(stderr, "Couldn't allocate PQ!\n");
		exit(EXIT_FAILURE);
	}

    for(int i = 0; i < row; i++){
        PredRockMatrix[i] = malloc(col*sizeof(struct rock));
    }
    return PredRockMatrix;
}

void FreeBoolMatrix(int row, bool** BoolMatrix){
    for(int i = 0; i < row; i++){
        free(BoolMatrix[i]);
    }

    free(BoolMatrix);
}

void FreePredMatrix(int row, struct rock** PredRockMatrix){
    for(int i = 0; i < row; i++){
        free(PredRockMatrix[i]);
    }

    free(PredRockMatrix);
}

void FreeWeightMatrix(int row, int** WeightMatrix){
    for(int i = 0; i < row; i++){
        free(WeightMatrix[i]);
    }

    free(WeightMatrix);
}

void AllRocksStartQ(Wall w, Queue q, int reach, Colour colour, bool** visited, struct rock** predecessor){
    
    struct rock DefaultRock;
    DefaultRock.colour = -1;

    struct rock AllRocks[WallWidth(w) * WallWidth(w)];

    int NumRocks = WallGetAllRocks(w,AllRocks);

    for(int i  = 0; i < NumRocks; i++){
        if(AllRocks[i].row <= reach && AllRocks[i].colour == colour){
            QueueEnqueue(q,AllRocks[i]);
            visited(AllRocks[i]) = true;
            predecessor(AllRocks[i]) = DefaultRock;
        }
    }
}

int AllRocksStartArr(Wall w, int reach,struct rock arr[]){
    
    struct rock AllRocks[WallWidth(w) * WallHeight(w)];

    int NumRocks = WallGetAllRocks(w,AllRocks);


    int counter = 0;

    for(int i  = 0; i < NumRocks; i++){
        if(AllRocks[i].row <= reach){
            arr[counter] = AllRocks[i];
            counter++;
        }
    }
    return counter;
}

void FlipPath(struct rock RockArr[], struct path* p){ 

    p->rocks = malloc(p->numRocks * sizeof(struct rock));

    for(int i = 0; i < p->numRocks; i++){
        p->rocks[i] = RockArr[p->numRocks - i - 1];
    }
}

struct rock FindMinEnergyPath(Wall w,int reach,struct rock* RockArr,int** WeightMatrix){
    int NumRocks = WallGetAllRocks(w, RockArr);

    struct rock RockExit;
    RockExit.colour = -1;

    for(int i = 0; i < NumRocks; i++){
        if(WallHeight(w) - RockArr[i].row <= reach){
           if(!ValidRocks(RockExit) || WeightMatrix(RockExit) > WeightMatrix(RockArr[i])){
                RockExit = RockArr[i];
           }
        }
    }
    return RockExit;    
}

struct rock FindMinTurnPath(Wall w,int reach,int energyCosts[], struct rock* RockArr,int** WeightMatrix,struct rock** predecessor){
    int NumRocks = WallGetAllRocks(w,RockArr);
    
    struct rock RockExit;
    RockExit.colour = -1;

    for(int i = 0; i < NumRocks; i++){
        if(WallHeight(w) - RockArr[i].row <= reach){
            if (!ValidRocks(RockExit) || WeightMatrix(RockExit) > WeightMatrix(RockArr[i])){
                RockExit = RockArr[i];
            }
            else if (WeightMatrix(RockExit) == WeightMatrix(RockArr[i])){

                int count1 = 0;
                int count2 = 0;
                struct rock RockTemp1 = RockExit;
                struct rock RockTemp2 = RockArr[i];

                while(ValidRocks(RockTemp1)){
                    count1 += energyCosts[RockTemp1.colour];
                    RockTemp1 = predecessor(RockTemp1);
                }
                while(ValidRocks(RockTemp2)){
                    count2 += energyCosts[RockTemp2.colour];
                    RockTemp2 = predecessor(RockTemp2);
                }
                RockExit = count1 < count2 ? RockExit : RockArr[i];
           }
        }
    }

    return RockExit;
}

void FreeAll(Wall w,struct rock RockArr[],int** WeightMatrix, struct rock** predecessor, bool** visited, PQ pq){
    free(RockArr);
    FreeWeightMatrix(WallHeight(w),WeightMatrix);
    FreePredMatrix(WallHeight(w),predecessor);
    FreeBoolMatrix(WallHeight(w),visited);
    PQFree(pq);
}

Node EdgeRelaxation(Node curr,Node neighbour,int value,int**WeightMatrix,struct rock** predecessor){
    if (WeightMatrix(curr.Item) + value < WeightMatrix(neighbour.Item)) {
        WeightMatrix(neighbour.Item) = WeightMatrix(curr.Item) + value;
        weight(neighbour) = WeightMatrix(neighbour.Item);
        predecessor(neighbour.Item) = curr.Item;
    }

    return neighbour; 
}

void MarkVisited(Node neighbour,bool** visited,PQ pq){
    if(!visited(neighbour.Item)){
        visited(neighbour.Item) = true; 
        PQInsert(pq,neighbour);
    }
}

int SafeInsert(struct rock** arr, int* capacity, int index, struct rock val){
    if(index >= *capacity){
        *arr = realloc(*arr, 2*index*sizeof(struct rock));
        *capacity = 2*index;
    }
    (*arr)[index++] = val;
    return index;
}
