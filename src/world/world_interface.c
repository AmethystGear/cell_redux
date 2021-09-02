#include "commands.h"
#include "world_interface.h"

void world_kill() {
    if (world_interface___num_commands >= MAX_NUM_COMMANDS_PER_TURN) {
        return;
    }
    world_interface___commands[world_interface___num_commands].command_type = KILL;
    world_interface___num_commands++;
}

void world_write_program(char* function_name, int x, int y) {
    if (world_interface___num_commands >= MAX_NUM_COMMANDS_PER_TURN) {
        return;
    }
    world_interface___commands[world_interface___num_commands].command_type = WRITE;

    world_interface___commands[world_interface___num_commands].loc = init_point(x, y);

    world_interface___commands[world_interface___num_commands].data.function_name = strdup(function_name);
    world_interface___commands[world_interface___num_commands].data.data = NULL;
    world_interface___num_commands++;
}

void world_write_data(char* data, int x, int y) {
    if (world_interface___num_commands >= MAX_NUM_COMMANDS_PER_TURN) {
        return;
    }
    world_interface___commands[world_interface___num_commands].command_type = WRITE;
    world_interface___commands[world_interface___num_commands].data.function_name = NULL;
    world_interface___commands[world_interface___num_commands].data.data = strdup(data);
    world_interface___num_commands++;
}

void world_call(int x, int y) {
    if (world_interface___num_commands >= MAX_NUM_COMMANDS_PER_TURN) {
        return;
    }
    world_interface___commands[world_interface___num_commands].command_type = CALL;
    world_interface___commands[world_interface___num_commands].loc = init_point(x, y);
    world_interface___num_commands++;
}