#ifndef EXECUTION_POINTER_H_
#define EXECUTION_POINTER_H_
#include <stdbool.h>
#include "point.h"

typedef struct ExecutionPointer {
    int id;
    Point loc;
    bool dead;
    bool running_cell;
    int recieve_pipe;
    int return_pipe;
    int process_id;
} ExecutionPointer;

char* get_recieve_pipe_name(ExecutionPointer* exec_ptr);
char* get_return_pipe_name(ExecutionPointer* exec_ptr);
#endif EXECUTION_POINTER_H_