#include <string.h>
#include "commands.h"
#include "world_interface.h"
#include "world_globals.h"
#include "../utility/vector/cvector.h"

static Command construct_command(CommandType command_type, char* function_name, char* data, int x, int y) {
    Command command = {
        .command_type = command_type,
        .loc = init_point(x, y),
        .data = {
            .function_name = function_name == NULL ? NULL : strdup(function_name),
            .data = data == NULL ? NULL : strdup(data)
        }
    };
    return command;
}

void world_kill() {
    cvector_push_back(commands__world_globals, construct_command(KILL, NULL, NULL, 0, 0));
}

void world_write_function(char* function_name, int x, int y) {
    cvector_push_back(commands__world_globals, construct_command(WRITE, function_name, NULL, x, y));
}

void world_write_data(char* data, int x, int y) {
    cvector_push_back(commands__world_globals, construct_command(WRITE, NULL, data, x, y));
}

void world_call(char* function_name, int x, int y) {
    cvector_push_back(commands__world_globals, construct_command(CALL, function_name, NULL, x, y));
}

Cell* get_cell(int x, int y) {
    return get_cell_at(grid__world_globals, add_points(init_point(x, y), location__world_globals));
}