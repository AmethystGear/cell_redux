#include "grid.h"
#include "../utility/vector/cvector.h"
#include "commands.h"
#define MAX_NUM_COMMANDS_PER_TURN 10

extern Grid* world_interface___COPIED_GRID;
extern Command world_interface___commands[MAX_NUM_COMMANDS_PER_TURN];
extern int world_interface___num_commands;

void world_kill();
void world_write_function(char* function, int x, int y);
void world_write_data(char* data, int x, int y);
void world_call_cell(int x, int y);