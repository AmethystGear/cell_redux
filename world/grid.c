#include <stdlib.h>
#include "grid.h"
#include "point.h"

int num_cells_in_grid() {
    return GRID_SIZE * GRID_SIZE;
}

/**
 * Return a pointer to an initialized grid.
 * The grid is initialized with Cells that have NULL data and program_name.
 * Caller must free by calling free_grid().
 */
Grid* init_grid() {
    Grid* grid = malloc(sizeof(Grid));
    for (uint32_t i = 0; i < num_cells_in_grid(); i++) {
        grid->arr[i].data = NULL;
        grid->arr[i].program_name = NULL;
    }
    return grid;
}

/**
 * Return a pointer to the Cell struct at the provided point.
 * Caller should not free.
 */
Cell* get_cell_at(Grid* grid, Point point) {
    uint32_t index = point_to_index(point);
    return &grid->arr[index];
}

/**
 * Frees the grid and all it's data.
 */
void free_grid(Grid* grid) {
    // NOTE: we only free the Cell's 'data' field because
    // the program_name field is always in stack memory
    for (uint32_t i = 0; i < num_cells_in_grid(); i++) {
        free(grid->arr[i].data);
    }
    free(grid);
}