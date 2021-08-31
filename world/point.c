#include <stdint.h>
#include "point.h"
#include "grid.h"

Point init_point(uint32_t x, uint32_t y) {
    Point point = {x, y};
    return point;
}

Point add_points(Point a, Point b) {
    return index_to_point(index(a) + index(b));
}

uint32_t point_to_index(Point p) {
    return p.x + p.y * GRID_SIZE;
}

Point index_to_point(uint32_t index) {
    uint32_t real_index = index % num_cells_in_grid();
    uint32_t x = real_index % GRID_SIZE;
    uint32_t y = real_index / GRID_SIZE;
    Point point = {x, y};
    return point;
}
