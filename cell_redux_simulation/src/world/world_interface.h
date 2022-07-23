#ifndef WORLD_INTERFACE_H_
#define WORLD_INTERFACE_H_
#include "grid.h"
#include "commands.h"

void world_kill();
void world_write_function(char* function, int x, int y);
void world_write_data(char* data, int x, int y);
void world_call(char* function_name, int x, int y);
#endif