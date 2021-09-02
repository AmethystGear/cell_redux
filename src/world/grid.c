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
        grid->arr[i].function_name = NULL;
    }
    return grid;
}

Grid* copy_grid(Grid* grid) {
    Grid* grid_copy = malloc(sizeof(Grid));
    for (uint32_t i = 0; i < num_cells_in_grid(); i++) {
        grid_copy->arr[i].data = malloc(grid->arr[i].num_bytes);
        grid_copy->arr[i].num_bytes = grid->arr[i].num_bytes;
        memcpy(grid_copy->arr[i].data, grid->arr[i].data, grid->arr[i].num_bytes);
        grid_copy->arr[i].function_name = grid->arr[i].function_name;
    }
    return grid_copy;
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
    // the program_name field is in static memory
    for (uint32_t i = 0; i < num_cells_in_grid(); i++) {
        free(grid->arr[i].data);
        free(grid->arr[i].function_name);
    }
    free(grid);
}