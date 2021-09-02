#ifndef EXECUTION_POINTER_H_
#define EXECUTION_POINTER_H_
#include <stdbool.h>
#include "point.h"

typedef struct ExecutionPointer {
    int id;
    Point loc;
    bool dead;
} ExecutionPointer;
#endif EXECUTION_POINTER_H_