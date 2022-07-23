#include <stdlib.h>
#include <string.h>
#include "grid.h"
#include "point.h"

/**
 * Return a pointer to an initialized grid.
 * The grid is initialized with Cells that have NULL data and program_name.
 * Caller must free by calling free_grid().
 */
Grid* init_grid() {
    Grid* grid = malloc(sizeof(Grid));
    for (int i = 0; i < NUM_CELLS_IN_GRID; i++) {
        grid->arr[i].data = NULL;
        grid->arr[i].function_name = NULL;
    }
    return grid;
}

Grid* copy_grid(Grid* grid) {
    Grid* grid_copy = malloc(sizeof(Grid));
    for (int i = 0; i < NUM_CELLS_IN_GRID; i++) {
        if (grid->arr[i].function_name != NULL) {
            grid_copy->arr[i].function_name = strdup(grid->arr[i].function_name);
        } else {
            grid_copy->arr[i].function_name = NULL;
        }
        if (grid->arr[i].data != NULL) {
            grid_copy->arr[i].data = strdup(grid->arr[i].data);
        } else {
            grid_copy->arr[i].data = NULL;
        }
        
    }
    return grid_copy;
}

/**
 * Return a pointer to the Cell struct at the provided point.
 * Caller should not free.
 */
Cell* get_cell_at(Grid* grid, Point point) {
    int index = point_to_index(point);
    return &grid->arr[index];
}

void free_cell(Cell* cell) {
    free(cell->data);
    free(cell->function_name);
    free(cell);
}

/**
 * Frees the grid and all it's data.
 */
void free_grid(Grid* grid) {
    // NOTE: we only free the Cell's 'data' field because
    // the program_name field is in static memory
    for (int i = 0; i < NUM_CELLS_IN_GRID; i++) {
        free(grid->arr[i].data);
        free(grid->arr[i].function_name);
    }
    free(grid);
}

static unsigned long hash_string(unsigned char *str) {
    unsigned long hash = 5381;
    if (str != NULL) {
        int c;
        while (c = *str++) {
            hash = hash * 33 + c;
        }
    }
    return hash;
}

long hash_grid(Grid* grid) {
    unsigned long hash = 5381;
    for (int i = 0; i < NUM_CELLS_IN_GRID; i++) {
        hash = hash * 33 + hash_string(grid->arr[i].data);
        hash = hash * 33 + hash_string(grid->arr[i].function_name);
    }
    return hash;
}

