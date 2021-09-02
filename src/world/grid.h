#ifndef GRID_H_
#define GRID_H_
#include "point.h"

#define GRID_SIZE 50

typedef struct {
    char* function_name;
    char* data;
    size_t num_bytes;
} Cell;

typedef struct {
    Cell arr[GRID_SIZE * GRID_SIZE];
} Grid;

Grid* init_grid();
Grid* copy_grid(Grid* grid);
Cell* get_cell_at(Grid* grid, Point point);
#endif GRID_H_