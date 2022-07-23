#ifndef STATS_H_
#define STATS_H_
typedef struct Stats {
    int speed; // how far can you move the execution pointer in one turn
    int range; // how far can you read/write from your position on the grid
    int power; // how many commands can you run per turn
} Stats;
#endif