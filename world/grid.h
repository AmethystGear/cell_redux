#ifndef GRID_H_
#define GRID_H_
#include "point.h"

#define GRID_SIZE 50

typedef struct {
    // program_name should always point to stack memory if it isn't NULL, no need to free it.
    char* program_name;
    // data will always point to allocated memory if it isn't NULL, it must be freed.
    char* data;
} Cell;

typedef struct {
    Cell arr[GRID_SIZE * GRID_SIZE];
} Grid;

Grid* init_grid();
Cell* get_cell_at(Grid* grid, Point point);

#endif GRID_H_