#include <stdlib.h>
#include "commands.h"
#include "execution_pointer.h"
#include "grid.h"
#include "point.h"

void run_command(ExecutionPointer* exec_ptr, Command* command, Grid* grid) {
    if (command->command_type == KILL) {
        exec_ptr->dead = true;
    } else{
        Point true_loc = add_points(exec_ptr->loc, command->loc);
        if (command->command_type == CALL) {
            exec_ptr->loc = true_loc;
        } else if (command->command_type == WRITE) {
            Cell* cell = get_cell_at(grid, true_loc);
            if (command->data.function_name != NULL) {
                cell->function_name = command->data.function_name;
            }
            if (command->data.function_name != NULL) {
                cell->data = command->data.data;
            }
        }
    }
}