#ifndef FIGHTER_INTERFACE_H_
#define FIGHTER_INTERFACE_H_
/**
 * Call the function with the specified name
 */
void call_function_with_name(char* name);

/**
 * Get an array containing the names of all the fighters.
 */
char** get_fighters(int* len);
#endif