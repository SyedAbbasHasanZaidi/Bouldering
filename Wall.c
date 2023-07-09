// Implementation of the Wall ADT

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Wall.h"
#include "helper.h"

//TYPEDEFS
typedef struct rock* rock;
/////////

struct wall {
    // TODO
    int height;
    int width;
    rock* rocks;
};

static int compareRocks(const void *ptr1, const void *ptr2);

/**
 * Creates a new blank wall with the given dimensions
 */
Wall WallNew(int height, int width) {
    // TODO
    Wall new = malloc(sizeof(*new));

    new->height = height;
    new->width = width;
    new->rocks = malloc(height*sizeof(rock));    

    struct rock dummy_val;

    dummy_val.colour = -1;

    for(int i = 0; i < height; i++){

        new->rocks[i] = malloc(width*sizeof(struct rock));

        for(int j = 0; j < width; j++){
            new->rocks[i][j] = dummy_val;
        }
    }
    return new;
}

/**
 * Frees all memory allocated to the wall 
 */
void WallFree(Wall w) {
    for(int i = 0; i < WallHeight(w) ; i++){
        free(w->rocks[i]);
    }

    free(w->rocks);
    free(w);
}

/**
 * Returns the height of the wall
 */
int WallHeight(Wall w) {
    return w->height;
}

/**
 * Returns the width of the wall
 */
int WallWidth(Wall w) {
    return w->width;
}

/**
 * Adds a rock to the wall
 * If there is already a rock at the given coordinates, replaces it
 */
void WallAddRock(Wall w, struct rock roc) {

    if(!(roc.row >= WallHeight(w) || roc.col >= WallWidth(w))){
        w->rocks[roc.row][roc.col] = roc;
    }
}

/**
 * Returns the number of rocks on the wall
 */
int WallNumRocks(Wall w) {
    int rock_counter = 0;

    for(int i  = 0; i < WallHeight(w) ; i++){
        for(int j  = 0; j < WallWidth(w) ; j++){
            if(ValidRocks(w->rocks[i][j])){
                rock_counter++;
            }
        }
    }

    return rock_counter;
}

/**
 * Stores all rocks on the wall in the given `rocks` array and returns
 * the number of rocks stored. Assumes that the array is at least as
 * large as the number of rocks on the wall.
 */
int WallGetAllRocks(Wall w, struct rock rocks[]) {
    int rock_counter = 0;

    for(int i  = 0; i < WallHeight(w) ; i++){
        for(int j  = 0; j < WallWidth(w) ; j++){
            if(ValidRocks(w->rocks[i][j])){
                rocks[rock_counter] = w->rocks[i][j];
                rock_counter++;
            }
        }
    }

    return rock_counter;
}

/**
 * Stores all rocks that are within a distance of `dist` from the given
 * coordinates in the given `rocks` array and returns the number of rocks
 * stored. Assumes that the array is at least as large as the number of
 * rocks on the wall.
 */
int WallGetRocksInRange(Wall w, int row, int col, int dist,
                        struct rock rocks[])
{
    int rock_counter = 0;

    int i1 = 0 > row - dist ? 0 : row-dist;
    int i2 = WallHeight(w) <= row + dist ? WallHeight(w) - 1 : row+dist;
    int j1 = 0 > col - dist ? 0 : col -dist;
    int j2 = WallWidth(w) <= col + dist ? WallWidth(w) - 1 : col + dist;

    for(int i  = i1; i <= i2 ; i++){
        for(int j  = j1; j <= j2 ; j++){
            if(sqrt(pow(i - row,2) + pow(j - col,2)) <= dist 
                && ValidRocks(w->rocks[i][j])){
                rocks[rock_counter] = w->rocks[i][j];
                rock_counter++;
            }
        }
    }

    return rock_counter;

}

/**
 * Stores all rocks with the colour `colour` that are within a distance
 * of `dist` from the given coordinates in the given `rocks` array and
 * returns the number of rocks stored. Assumes that the array is at
 * least as large as the number of rocks on the wall.
 */
int WallGetColouredRocksInRange(Wall w, int row, int col, int dist,
                                Colour colour, struct rock rocks[])
{

    int RockCounter1 = 0;

    struct rock AllRocks[WallWidth(w)*WallHeight(w)];

    int RockCounter2 = WallGetRocksInRange(w, row, col, dist, AllRocks);

    for(int i  = 0; i < RockCounter2; i++){
        if(AllRocks[i].colour == colour){
            rocks[RockCounter1] = AllRocks[i]; 
            RockCounter1++;
        }
    }

    return RockCounter1;
}

////////////////////////////////////////////////////////////////////////

/**
 * Prints the wall out in a nice format
 * NOTE: DO NOT MODIFY THIS FUNCTION! This function will work once
 *       WallGetAllRocks and all the functions above it work.
 */
void WallPrint(Wall w) {
    int height = WallHeight(w);
    int width = WallWidth(w);
    int numRocks = WallNumRocks(w);
    struct rock *rocks = malloc(numRocks * sizeof(struct rock));
    WallGetAllRocks(w, rocks);
    qsort(rocks, numRocks, sizeof(struct rock), compareRocks);

    int i = 0;
    for (int y = height; y >= 0; y--) {
        for (int x = 0; x <= width; x++) {
            if ((y == 0 || y == height) && (x == 0 || x % 5 == 0)) {
                printf("+ ");
            } else if ((x == 0 || x == width) && (y == 0 || y % 5 == 0)) {
                printf("+ ");
            } else if (y == 0 || y == height) {
                printf("- ");
            } else if (x == 0 || x == width) {
                printf("| ");
            } else if (i < numRocks && y == rocks[i].row && x == rocks[i].col) {
                char *color;
                switch (rocks[i].colour) {
                    case GREEN: color = "\x1B[32m"; break;
                    case TEAL:  color = "\x1B[96m"; break;
                    case PINK:  color = "\x1B[95m"; break;
                    case RED:   color = "\x1B[91m"; break;
                    default:    color = "\x1B[0m";  break;
                }
                printf("%so %s", color, RESET);
                i++;
            } else {
                printf("\u00B7 ");
            }
        }
        printf("\n");
    }

    free(rocks);
}

static int compareRocks(const void *ptr1, const void *ptr2) {
    struct rock *r1 = (struct rock *)ptr1;
    struct rock *r2 = (struct rock *)ptr2;
    if (r1->row != r2->row) {
        return r2->row - r1->row;
    } else {
        return r1->col - r2->col;
    }
}

