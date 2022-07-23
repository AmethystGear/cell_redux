#ifndef FIGHTER_INTERFACE_H_
#define FIGHTER_INTERFACE_H_
#include "../world/stats.h"
/**
 * Call the function with the specified name
 */
void call_function_with_name(char* name);

int get_function_index(char* name);

/**
 * Get an array containing the names of all the fighters.
 */
char** get_fighters(int* len);

Stats get_fighter_stats(char* fighter_name);

char* get_function(int index);

int num_functions();
#endif