#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <stdbool.h>
#include "grid.h"
#include "point.h"
#include "execution_pointer.h"
#include "stats.h"

typedef enum CommandType {
    KILL,
    CALL,
    WRITE,
    READ
} CommandType;

typedef struct Command {
    CommandType command_type;
    Point loc;
    Cell data;
} Command;

bool run_command(ExecutionPointer* exec_ptr, Command* command, Grid* grid, Stats* stats, char* fighter_name, char** funcdiff, Point* locdiff);
#endif