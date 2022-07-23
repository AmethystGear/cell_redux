#ifndef GRID_H_
#define GRID_H_
#include "point.h"

#define GRID_SIZE 32
#define NUM_CELLS_IN_GRID (GRID_SIZE * GRID_SIZE)

typedef struct Cell {
    char* function_name;
    char* data;
} Cell;

typedef struct Grid {
    Cell arr[GRID_SIZE * GRID_SIZE];
} Grid;

Grid* init_grid();
Grid* copy_grid(Grid* grid);
Cell* get_cell_at(Grid* grid, Point point);
void free_cell(Cell* cell);
void free_grid(Grid* grid);
long hash_grid(Grid* grid);
#endif