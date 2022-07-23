#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "commands.h"
#include "execution_pointer.h"
#include "grid.h"
#include "point.h"
#include "stats.h"
#include "../utility/utils.h"

bool run_command(ExecutionPointer* exec_ptr, Command* command, Grid* grid, Stats* stats, char* fighter_name, char** funcdiff, Point* locdiff) {
    if (command->command_type == KILL) {
        fprintf(stderr, "%s was killed by another player\n", fighter_name);
        return false;
    }
    Point true_loc = add_points(exec_ptr->loc, command->loc);
    Cell* cell = get_cell_at(grid, true_loc);
    
    char* function_name = command->data.function_name;
    if (command->command_type == CALL) {
        if (abs(command->loc.x) + abs(command->loc.y) > stats->speed) {
            fprintf(stderr, "%s tried to move faster than their maximum speed\n", fighter_name);
            return false;
        }
        exec_ptr->loc = true_loc;
        if (command->data.function_name != NULL) {
            cell->function_name = function_name;
        }
    } else if (command->command_type == WRITE) {
        if (abs(command->loc.x) + abs(command->loc.y) > stats->range) {
            fprintf(stderr, "%s tried to write to a location outside their range\n", fighter_name);
            return false;
        }
        if (command->data.function_name != NULL) {
            cell->function_name = function_name;
        }
        if (command->data.function_name != NULL) {
            cell->data = command->data.data;
        }
    }
    *funcdiff = function_name;
    locdiff->x = true_loc.x;
    locdiff->y = true_loc.y;
    return true;
}