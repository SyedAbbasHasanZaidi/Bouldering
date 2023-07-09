
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#include "helper.h"
#include "Queue.h"
#include "PQ.h"
#include "Wall.h"
#include "climber.h"

/*
* Uses Breath First Search algorithm on a graph representation 
* of the rock climbing wall to find the shortest path in terms of a colour 
*/
static struct path BFSpath(Wall,struct path,Colour,int);
/*
* Uses Djikstra algorithm on a graph representation of the rock climbing wall 
* to find least cost energy path 
*/
static struct path DijkstraMinEnergy(Wall, int, struct path ,int[]);
/*
* Uses Djikstra algorithm on a graph representation of the rock climbing wall 
* to find the least turn path 
*/
static struct path DijkstraMinTurns(Wall w, int reach, struct path p, int energyCosts[], int maxEnergy);


struct path findShortestPath(Wall w, int reach, Colour colour) {
    // TODO - Task 1
    struct path p = {0, NULL};

    p = BFSpath(w,p,colour,reach);

    return p;
}

struct path findMinEnergyPath(Wall w, int reach, int energyCosts[NUM_COLOURS]) {
    // TODO - Task 2

    struct path p = {0, NULL};

    p = DijkstraMinEnergy(w, reach, p, energyCosts);
    
    return p;
}

struct path findMinTurnsPath(Wall w, int reach, int energyCosts[NUM_COLOURS],
                             int maxEnergy) {
    // TODO - Task 3
    struct path p = {0, NULL};
    p = DijkstraMinTurns(w, reach, p, energyCosts, maxEnergy);

    return p;
}

struct path BFSpath(Wall w ,struct path p, Colour colour, int reach){
    
    //* Creates visited and predecessor matrix 
    bool** visited = CreateBoolMatrix(WallHeight(w),WallWidth(w));  
    struct rock** predecessor = CreatePredMatrix(WallHeight(w),WallWidth(w));
    //* Initalising Queue 
    Queue q = QueueNew();
    //* Enqueues all rocks start points into the queue 
    AllRocksStartQ(w,q,reach,colour,visited,predecessor);
    //* Storing all possible nodes/rocks in the graph/wall into the queue 
    while(!QueueIsEmpty(q)){
        //* popping queue
        struct rock curr = QueueDequeue(q);
        //* creating an array of rocks for max size (worst case)
        struct rock RockArr[WallWidth(w) * WallHeight(w)];
        //* finshing all rocks in range => neighbours of curr
        int NumRocks = WallGetColouredRocksInRange(w, curr.row, curr.col, reach, colour, RockArr);
        //* marking neighbours as visited, curr as predecessor of neighbour and enqeueing neighbour if not visited
        for(int i = 0; i < NumRocks; i++){
            if(!visited(RockArr[i])){
                visited(RockArr[i]) = true;
                QueueEnqueue(q,RockArr[i]);
                predecessor(RockArr[i]) = curr;
            }
        }
        //* If rocks is and exit and is the correct colour, start tracing the path that found the exxit the quickest
        if(ValidRocks(curr) && WallHeight(w) - curr.row <= reach && curr.colour == colour){
            struct rock path = curr;
            struct rock PathArr[WallWidth(w)*WallHeight(w)]; //* worst case 
            //* Stores path from exit to start (backwards) by 'unwinding' the predecessor andd storing them in PathArr 
            while (ValidRocks(path)) {
                PathArr[p.numRocks++] = path;
                path = predecessor(path);
            }
            //* Flipping path 
            FlipPath(PathArr,&p);           
            //* Once found path => stop loop 
            break;
        }
    }
    //* Free
    QueueFree(q);
    FreeBoolMatrix(WallHeight(w),visited);
    FreePredMatrix(WallHeight(w),predecessor);
    return p;
}
//? Given the posibility of multiple strating points i assigned a 'dummy' rock that acts 
//? as the starting point and adds all the strating points of the graph as neighbours 
//? (Task 2 & 3)


struct path DijkstraMinEnergy(Wall w,int reach, struct path p,int energyCosts[]){
    //* Initialising a Default rock to serve as a 'dummy' start point
    struct rock DefaultRock;
    DefaultRock.colour = -1;
    //*Initialising a Priority Queue
    PQ pq = PQNew();
    //* Initialising a cost, predecessor and visitor matricies and creating RockkArr 
    int** WeightMatrix = CreateWeightMatrix(WallHeight(w),WallWidth(w));
    struct rock** predecessor = CreatePredMatrix(WallHeight(w),WallWidth(w));
    bool** visited = CreateBoolMatrix(WallHeight(w),WallWidth(w));
    struct rock* RockArr = malloc(WallNumRocks(w) * sizeof(struct rock));
    //* Storing all possible neighbours of start nodes/rocks in the graph/wall into the RockArr 
    int NumRocks = AllRocksStartArr(w,reach,RockArr);
    //* Adding all start nodes to pq setting predecessor, cost and visitor       
    for(int i = 0; i < NumRocks; i++){
        struct node new = NewNode(RockArr[i]);
        new.weight = energyCosts[RockArr[i].colour];
        WeightMatrix(new.Item) = new.weight;
        predecessor(new.Item) = DefaultRock;
        visited(RockArr[i]) = true;
        PQInsert(pq,new);
    }
    //* Visiting all other nodes 
    while(!PQIsEmpty(pq)){
        //* popping queue
        struct node curr = PQExtract(pq);
        int NumRocks = WallGetRocksInRange(w,row(curr),col(curr),reach,RockArr);
        //* visit all neighbours 
        for (int i = 0; i < NumRocks; i++) {
            //* create node from neighbour rock 
            Node neighbour = NewNode(RockArr[i]);
            //* need to check the itself as a neighbour (due to WallRockGetRange Implementation)
            if(row(neighbour) == row(curr) && col(neighbour) == col(curr)) continue; // dont consider diplicate rocks 
            //* Edge relax current neighbours weight 
            neighbour = EdgeRelaxation(curr,neighbour,energyCosts[RockArr[i].colour],WeightMatrix,predecessor);
            //* mark as visited and add it to pq, if not already visited 
            MarkVisited(neighbour,visited,pq);
        }
    }
    //* find least Minimum energy path 
    struct rock RockExit = FindMinEnergyPath(w,reach,RockArr,WeightMatrix);
    //* 'Unwind' predecessor to and store rocks (backwards) into RockArr
    while (ValidRocks(RockExit)) {
        RockArr[p.numRocks++] = RockExit;
        RockExit = predecessor(RockExit);
    }
    //* flip path 
    FlipPath(RockArr,&p);
    //* Free all contents stored in arguements 
    FreeAll(w,RockArr,WeightMatrix,predecessor,visited,pq);
    return p; 

}

struct path DijkstraMinTurns(Wall w, int reach, struct path p, int energyCosts[], int maxEnergy){
    //* Initialising a Default rock to serve as a 'dummy' start point
    int CurrentEnergy = maxEnergy;
    struct rock DefaultRock;
    DefaultRock.colour = -1;
    //* Initialising a Priority Queue
    PQ pq = PQNew();
    //* Initialising a Cost, predecessor and boolean matricies 
    int** WeightMatrix = CreateWeightMatrix(WallHeight(w),WallWidth(w));
    struct rock** predecessor = CreatePredMatrix(WallHeight(w),WallWidth(w));
    bool** visited = CreateBoolMatrix(WallHeight(w),WallWidth(w));
    //* Dynamically Allocating RockArr
    struct rock* RockArr = malloc(WallNumRocks(w) * sizeof(struct rock));
    //* Storing all possible neighbours of start nodes/rocks in the graph/wall into the RockArr
    int NumRocks = AllRocksStartArr(w,reach,RockArr);
    //* Adding all start nodes to pq setting predecessor, cost and visitor 
    for(int i = 0; i < NumRocks; i++){
        //* popping queue
        struct node new = NewNode(RockArr[i]);
        new.weight = 1; // We can reach any of the starting rocks wihtout resting 
        WeightMatrix(new.Item) = new.weight;
        predecessor(new.Item) = DefaultRock;
        visited(RockArr[i]) = true;
        PQInsert(pq,new);
    }
    //* Visiting all other nodes 
    while(!PQIsEmpty(pq)){
        //* popping queue
        struct node curr = PQExtract(pq);
        //* Sub
        CurrentEnergy -= energyCosts[curr.Item.colour];

        if (CurrentEnergy <= 0) CurrentEnergy = maxEnergy - energyCosts[curr.Item.colour];

        NumRocks = WallGetRocksInRange(w,row(curr),col(curr),reach,RockArr);
        //* Visiting all neighbours 
        for (int i = 0; i < NumRocks; i++) {
            Node neighbour = NewNode(RockArr[i]);
            if(row(neighbour) == row(curr) && col(neighbour) == col(curr)) continue;
            int ChangeValue = (CurrentEnergy > energyCosts[RockArr[i].colour]) ? 1 : 2; 
            neighbour = EdgeRelaxation(curr,neighbour,ChangeValue,WeightMatrix,predecessor);
            MarkVisited(neighbour,visited,pq);
        }
    }

    //* find least turn consuming path and setting capacity for 'path array' 
    struct rock RockExit = FindMinTurnPath(w,reach,energyCosts,RockArr,WeightMatrix,predecessor);
    int capacity = WallNumRocks(w);

    //* 'Unwind' predecessor to and store rocks (backwards) into RockArr
    while (ValidRocks(predecessor(RockExit))) {
        p.numRocks = SafeInsert(&RockArr,&capacity,p.numRocks,RockExit);

        if(WeightMatrix(RockExit) - WeightMatrix(predecessor(RockExit)) == 2){
            p.numRocks = SafeInsert(&RockArr,&capacity,p.numRocks,predecessor(RockExit));
        }

        RockExit = predecessor(RockExit);
    }
    //* adds starting rock 
    p.numRocks = SafeInsert(&RockArr,&capacity,p.numRocks,RockExit);
    //* Flipping Path
    FlipPath(RockArr,&p);
    //* Freeing malloced memory
    FreeAll(w,RockArr,WeightMatrix,predecessor,visited,pq);
    return p;
}
