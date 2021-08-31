#ifndef POINT_H_
#define POINT_H_

#include <stdint.h>

typedef struct Point {
    uint32_t x;
    uint32_t y;
} Point;

Point init_point(uint32_t x, uint32_t y);
Point add_points(Point a, Point b);
uint32_t point_to_index(Point p);
Point index_to_point(uint32_t index);

#endif POINT_H_