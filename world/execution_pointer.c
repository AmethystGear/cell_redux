#include "execution_pointer.h"

/**
 * Get the pipe name given the prefix and the id of the exec_ptr.
 * Caller must free.
 */
/*
char* get_pipe_name(char* prefix, int id) {
    char* id_str = int_to_string(id);
    char* concat[] = {prefix, id_str};
    char* pipe_name = 
    free(id_str);
    return pipe_name;
}
*/
/**
 * Returns the name of the recieve pipe. Caller must free.
 */
char* get_recieve_pipe_name(ExecutionPointer* exec_ptr) {
    return get_pipe_name("recieve", exec_ptr->id);
}

/**
 * Returns the name of the recieve pipe. Caller must free.
 */
char* get_return_pipe_name(ExecutionPointer* exec_ptr) {
    return get_pipe_name("return", exec_ptr->id);
}