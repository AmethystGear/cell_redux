#ifndef WORLD_GLOBALS_H_
#define WORLD_GLOBALS_H_
#include "../utility/vector/cvector.h"
#include "commands.h"
#include "stats.h"

extern int num_commands__world_globals;
extern Stats stats__world_globals;
extern cvector_vector_type(Command) commands__world_globals;
extern Grid* grid__world_globals;
extern Point location__world_globals;
#endif