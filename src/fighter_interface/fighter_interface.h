#ifndef FIGHTER_INTERFACE_H_
#define FIGHTER_INTERFACE_H_
/**
 * Call the function with the provided name of the provided fighter
 */
void call_fighter_function(char* fighter_name, char* function_name);

/**
 * Get an array containing the names of all the fighters.
 */
char** get_fighters(int* len);
#endif