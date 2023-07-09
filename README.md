# Bouldering
Using graphs and path finding algorithm in a real life context
Prerequisite Knowledge
Abstract Data Types
Graphs
Graph Traversal
Directed Graphs
Weighted Graphs
Shortest Path
Background
You may or may not have heard of bouldering before, but fear no more! After completing this assignment, you will know all about it and will probably even want to try it out. In summary, bouldering is rock climbing without a harness, where the aim is to climb from the bottom of the wall to the top of the wall without falling off. To accommodate for different skill levels, bouldering places offer climbing routes of varying difficulties, indicated by different coloured rocks. For simplicity, in this assignment there are only four levels of routes (which we will now refer to as boulders):

Level 0: Green
Level 1: Teal
Level 2: Pink
Level 3: Red
To simplify things for this assignment, we will also assume that climbers hold on to only one rock at a time.

When climbing traditional boulders, you must only climb on rocks of a single colour. This will be relevant to Task 1. There are also special walls called "kilter boards", which can light up rocks with different colours and allow for various unique boulders. Tasks 2 and 3 take inspiration from this, where you will be given walls with multiple coloured rocks and are allowed to climb any of them!

DISCLAIMER

This is not by any means a tutorial on how to boulder, and this assignment is not responsible for injuries or monetary loss when bouldering.

Setting Up
Change into the directory you created for the assignment and run the following command:

unzip /web/cs2521/23T1/ass/ass2/downloads/files.zip
If you're working at home, download files.zip by clicking on the above link and then unzip the downloaded file.

You should now have the following files:

Makefile
This controls compilation of the program. You only need to modify this if you create additional files for your implementation.
Wall.h
This is the interface to the Wall ADT. It provides climbers with information about the wall. You must not modify this file.
Wall.c
This is the implementation of the Wall ADT. At the moment it is just a stub file that you will need to implement yourself.
wallReader.h
This is the interface to the wall reader, a function which reads the properties of a wall from a file and returns an instance of the Wall ADT. You must not modify this file.
wallReader.c
This is the implementation of the wall reader. You must not modify this file.
climber.h
This is the interface to the boulder climbing algorithms. You must not modify this file.
climber.c
This is the implementation of the boulder climbing algorithms. At the moment it is just a stub file that you will need to implement yourself.
testWall.c
This is an interactive program that you can use to test the Wall ADT. You should not modify this file unless you know what you're doing.
testClimber.c
This is a program that can be used to test the boulder climbing algorithms implemented in climber.c. You must not modify this file.
data/
This is a directory containing wall data files. The format of these files are described below.
tests/
This is a directory containing test data files and expected outputs. The files ending with .in are test data files, while the corresponding .exp files contain expected outputs.
First, compile the original version of the files using the make command. This will produce two executables: testWall and testClimber.

Note that in this assignment, you are permitted to create as many supporting files as you like. This allows you to compartmentalise your solution into different files. For example, you could implement a Graph ADT in Graph.c and Graph.h. To ensure that these files are actually included in the compilation, you will need to edit the Makefile; the provided Makefile contains instructions on how to do this.

Wall Data Files
Wall data files contain the details of each wall, including:

The dimensions (height and width) of the wall
The location and colour of each rock on the wall
Each file is formatted as follows:

The first line contains two integers representing the height and width of the wall.
Then, for each rock there is a line containing three integers: the row and column the rock is on and its colour (0-3).
If a rock is on row 
X
 and column 
Y
, that means it is 
X
 units above the ground and 
Y
 units from the left edge of the wall. There are no rocks on the border of the wall; this means if the wall has height 
H
 and width 
W
, all rocks will be on rows between 
1
 and 
H
−
1
 and columns between 
1
 and 
W
−
1
 (inclusive).

For example, consider the following wall data file:

20 25
4 18 0
5 6 0
6 2 0
6 14 0
7 10 0
8 16 0
9 7 0
9 23 0
10 13 0
11 19 0
12 4 0
12 9 0
14 12 0
14 21 0
15 5 0
16 17 0
5 3 1
6 11 1
6 16 1
7 7 1
7 20 1
10 18 1
13 6 1
13 11 1
13 23 1
14 15 1
16 2 1
16 20 1
4 9 2
5 17 2
5 22 2
7 5 2
9 19 2
10 3 2
11 10 2
11 15 2
14 18 2
16 9 2
The first line indicates that the wall has a height of 20 and a width of 25. The remaining lines indicate that there are several green, teal and pink rocks. There are no red rocks. The wall looks like this:


The provided programs will read the data file using the readWall function in wallReader.c, so you won't need to do any file reading yourself, but you should understand the format of the file so that you can create your own walls for testing.

Task 0
In order to start climbing, you will first need to collect some information, such as the height and width of the wall, and the coordinates and colours of each rock on the wall.

Therefore, your first task is to implement the Wall ADT in Wall.c, which stores this information and provides some helpful operations to climbers.

The functions you must implement are:

Function	Description
WallNew	Creates a new blank wall with the given dimensions
WallFree	Frees all memory allocated to a wall
WallHeight	Returns the height of a wall
WallWidth	Returns the width of a wall
WallAddRock	Adds a new rock to the wall. If there was already a rock at the given coordinates, that rock should be replaced.
WallNumRocks	Returns the number of rocks on the wall
WallGetAllRocks	Stores all rocks in the given rocks array (in any order) and returns the number of rocks stored. Assumes that the rocks array is at least as large as the number of rocks on the wall.
WallGetRocksInRange	Stores all rocks that are within a straight-line distance of dist (inclusive) from the given coordinates in the given rocks array (in any order) and returns the number of rocks stored. Assumes that the rocks array is at least as large as the number of rocks on the wall.
Definition
Straight-line distance

If two rocks are located at 
(
r
1
,
c
1
)
 and 
(
r
2
,
c
2
)
, then the straight-line distance between them is 
√
(
r
1
−
r
2
)
2
+
(
c
1
−
c
2
)
2
.

WallGetColouredRocksInRange	Stores all rocks with the colour colour that are within a straight-line distance of dist (inclusive) from the given coordinates in the given rocks array (in any order) and returns the number of rocks stored. Assumes that the rocks array is at least as large as the number of rocks on the wall.
Testing
Task 0 can be tested using the testWall program. The program takes one command-line argument: the name of a wall data file. It reads in the wall data file and creates an instance of the Wall ADT using the functions you implemented. It then accepts commands to interact with this instance using the various Wall ADT functions.

Task 1
The main aim of bouldering is to try and climb to the top of the wall on a given colour.

Your task is to complete the findShortestPath function, which, given the reach of a climber and a rock colour, finds the shortest climbing path (in terms of the number of rocks) from the bottom of the wall to the top of the wall, using only rocks of the given colour.

The climber's reach determines which rocks they can climb to from their current rock, as well as which rocks the climber can start and finish on. If the climber's reach is 
r
, then:

They may start at any rock that is at most 
r
 units above the ground
Every subsequent rock they climb on must be within 
r
 units of their current rock (straight line distance)
They may finish at any rock that is within 
r
 units of the top of the wall
The function should return a path struct where the numRocks field is set to the number of rocks on the shortest path, and the rocks field is set to a dynamically allocated array containing the sequence of rocks on the shortest path.

If there are multiple shortest paths, the function may return any of them. If there is no path from the bottom of the wall to the top of the wall, the function should return a struct where numRocks is set to 0 and rocks is set to NULL.

Examples
Consider the following wall:


Suppose the climber's reach is 5.

Then there are two shortest paths for green:


The number of rocks in each path is 4. Therefore, findShortestPath should return a path struct where numRocks is set to 4 and rocks is set to a dynamically allocated array containing one of the following sequences of rocks:

(5, 6, GREEN), (9, 7, GREEN), (12, 4, GREEN), (15, 5, GREEN)
or
(5, 6, GREEN), (9, 7, GREEN), (12, 9, GREEN), (15, 5, GREEN)
There is one shortest path for teal:


It is not possible to reach the top of the wall by climbing only pink rocks. However, if the climber had a reach of 6, then it would be possible (this is left to the reader as an exercise).

There are no red rocks, so it is not possible to reach the top of the wall by climbing only red rocks.

You can assume that the climber's reach will always be less than the height of the wall, so the shortest path will always require at least one rock.

Testing
Tasks 1, 2 and 3 can be tested using the testClimber program. The program takes one command-line argument: the name of a test data file.

Test data files contain the parameters for a test case. The format of a test data file depends on which task is being tested, but it always begins with one integer, which is the number of the task being tested.

You can test Task 1 by running ./testClimber with one of the test data files in the tests/task1 directory. The format of the test data file is as follows:

The first line contains the integer 1.
The next line contains the name of a wall data file (not including the data/ directory prefix).
The next line contains an integer representing the climber's reach.
The final line contains an integer representing the colour of the rocks the climber must exclusively climb on.
Here is an example test data file for Task 1:

1
wall1.data
5
0
This means this particular test case uses the wall given in data/wall1.data, the climber has a reach of 5, and the climber must only climb on green rocks.

This is one of the expected outputs from this test case:

./testClimber tests/task1/test1-1.in
Wall:
...

Reach: 5
Colour: Green

Path:
-> Green rock at (5, 6)
-> Green rock at (9, 7)
-> Green rock at (12, 4)
-> Green rock at (15, 5)

Total rocks: 4
Since there are two shortest paths for green, the following output is also acceptable:

./testClimber tests/task1/test1-1.in
Wall:
...

Reach: 5
Colour: Green

Path:
-> Green rock at (5, 6)
-> Green rock at (9, 7)
-> Green rock at (12, 9)
-> Green rock at (15, 5)

Total rocks: 4
Task 2
For Task 2, the rules have slightly changed. Instead of being restricted to climbing only a single colour rock for a given boulder, you are now allowed to climb on any coloured rock. However, there is now an energy cost associated with climbing each colour rock.

For example, suppose the energy cost for each colour rock is:

Green: 1
Teal: 2
Pink: 3
Red: 4
If you climb from a green rock to a pink rock, this movement will have an energy cost of 3. Alternatively, if you climb from a teal rock to a pink rock, this movement will ALSO have a cost of 3. Note that the cost of a specific movement is dependent on the rock you are climbing to, not the rock you are climbing from or the distance between the rocks.

Your task is to complete the findMinEnergyPath function, which, given the reach of a climber and the energy costs of each colour rock, finds the least energy-consuming path from the bottom of the wall to the top of the wall.

The climber's reach has the same meaning as in Task 1.

The energy costs are given in an array energyCosts where the energy cost of a colour col can be found at index col. Note that colours can be used to index into the energyCosts array because each colour is simply a constant between 0 and 3 (NUM_COLOURS - 1).

There is no energy cost associated with climbing from the final rock to the top of the wall.

The function should return a path struct where the numRocks field is set to the number of rocks on the least energy-consuming path, and the rocks field is set to a dynamically allocated array containing the sequence of rocks on that path.

As in Task 1, if there are multiple best paths, the function may return any of them. If there is no path from the bottom of the wall to the top of the wall, the function should return a struct where numRocks is set to 0 and rocks is set to NULL.

Examples
Consider the following wall:


Suppose the climber's reach is 3, and the energy cost for each colour rock is:

Green: 1
Teal: 2
Pink: 3
Red: 4
Then the path that uses the least energy is:


The total energy cost of the path is 10, and the number of rocks on the path is 4. Therefore, findMinEnergyPath should return a path struct where numRocks is set to 4 and rocks is set to a dynamically allocated array containing this sequence of rocks:

(2, 4, TEAL), (4, 5, PINK), (6, 7, GREEN), (8, 8, RED)
Testing
You can test Task 2 by running ./testClimber with one of the test data files in the tests/task2 directory. The format of the test file is as follows:

The first line contains the integer 2.
The next line contains the name of a wall data file (not including the data/ directory prefix).
The next line contains an integer representing the climber's reach.
The final line contains four integers representing the energy costs of each colour rock (green, teal, pink, then red).
Here is an example test data file for Task 2:

2
wall3.data
3
1 2 3 4
This means this particular test case uses the wall given in data/wall3.data, the climber has a reach of 3, and the energy cost for each colour rock is:

Green: 1
Teal: 2
Pink: 3
Red: 4
The expected output from this test case is:

./testClimber tests/task2/test2-1.in
Wall:
...

Reach: 3
Energy costs:
- Green: 1
- Teal: 2
- Pink: 3
- Red: 4

Path:
-> Teal rock at (2, 4)
-> Pink rock at (4, 5)
-> Green rock at (6, 7)
-> Red rock at (8, 8)

Total energy: 10
Task 3
You have now come up with a new challenge. Instead of trying to climb a specific colour rock, or get to the top in the easiest manner... you now want to climb to the top as quickly as possible.

In your previous climbs, you noticed that climbing a rock always takes the same amount of time, regardless of its colour or distance from the previous rock. Unfortunately, you also discovered that humans get tired while climbing, so you occasionally need to stop to catch your breath.

To find the quickest path to the top, you must therefore take both the number of rocks climbed and the number of rests into account. Here is how resting works:

A climber begins with some amount of energy. This is also the maximum amount of energy they can have.
Upon climbing each rock, the climber's energy level decreases by the rock's energy cost.
If the climber does not have enough energy to climb any nearby (reachable) rock, the climber must rest on their current rock. Climbers can also rest on a rock by choice, even if they have energy to climb another rock.
Resting on a rock refills the climber's energy levels completely.
Climbing a rock and resting both take the same amount of time, regardless of how much energy the climber had before resting. We will call this unit of time "one turn".
For example, suppose the climber's initial energy level is 10, and the energy cost for each colour rock is:

Green: 1
Teal: 2
Pink: 3
Red: 4
If they climb a green rock, their energy level will decrease to 9. Then, if they climb two red rocks in a row, their remaining energy will be 1. If the climber now wants to climb a teal, pink or red rock, they must first rest for a turn, which will result in their energy level being refilled to 10.

Your task is to complete the findMinTurnsPath function, which, given the reach of a climber, the energy costs of each colour rock, and the initial energy level of the climber, finds the path from the bottom of the wall to the top of the wall that takes the minimum number of turns (where turn is defined above).

The function should return a path struct where the rocks field is set to a dynamically allocated array containing the sequence of rocks on the minimum-turns path, and the numRocks field is set to the number of rocks in the array. Note that if your path requires the climber to rest at a particular rock, that rock should appear twice in a row in the rocks array; the first occurrence indicates that the climber should climb to that rock, and the second occurrence indicates that the climber should rest there. See the Examples section for an example.

If there are multiple paths which result in a minimum number of turns taken, the function should return the one which results in the climber having the highest remaining energy at the end of the path. If there are multiple such paths, the function may return any of them. If there is no path from the bottom of the wall to the top of the wall, the function should return a struct where numRocks is set to 0 and rocks is set to NULL.

Examples
Consider the following wall:


Suppose the climber's reach is 3, their initial energy level is 10, and the energy cost for each colour rock is:

Green: 8
Teal: 4
Pink: 10
Red: 0
Then the path that takes the minimum number of turns is:
This path takes 5 turns:

Climb to the first green rock (remaining energy is 2)
Rest at the green rock (energy is refilled to 10)
Climb to the teal rock (remaining energy is 6)
Rest at the teal rock (energy is refilled to 10)
Climb to the green rock (remaining energy is 2)
Note that the movement from the final rock to the top of the wall is not counted.

Therefore, findMinTurnsPath should return a path struct where numRocks is set to 5 and rocks is set to a dynamically allocated array containing this sequence of rocks:

(2, 7, GREEN), (2, 7, GREEN), (5, 7, TEAL), (5, 7, TEAL), (8, 7, GREEN)
The green rock at (2, 7) and teal rock at (5, 7) are repeated because the climber rests at these rocks.

Testing
You can test Task 3 by running ./testClimber with one of the test data files in the tests/task3 directory. The format of the test file is as follows:

The first line contains the integer 3.
The next line contains the name of a wall data file (not including the data/ directory prefix).
The next line contains an integer representing the climber's reach.
The next line contains four integers representing the energy costs of each colour rock (green, teal, pink, then red).
The final line contains an integer representing the amount of energy the climber starts with, which is also the maximum amount of energy the climber can have.
Testing
We have described how to test each task under their respective sections. In summary:

Task 0 can be tested by running the ./testWall program.
Tasks 1-3 can be tested by running the ./testClimber program with one of the test data files in the tests directory. Files ending in .in are test data files, while the corresponding files ending in .exp contain the expected output of the tests. Note that the expected output files do not include the printout of the wall, since this output contains colour codes that would not display properly in a text editor.
We strongly recommend you to create your own tests, as the given tests are very simple. You can create your own tests by creating new walls in the data/ directory and then creating new test data files, following the formats described under each task.
