#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#include "utility/json/cJSON.h"

#define MAX_RECIEVE_BUF 512
#define MAX_ITERATIONS 10000
#define PROGRAM_TIMEOUT 5


void main(int argc, char *argv[]) {

}

/*
void init_rng() {
    srand(time(NULL));
}

char** get_pipe_names(ExecutionPointer* exec_ptr, int* pipe_names_len) {
    char* pipe_names[] = {
        get_recieve_pipe_name(exec_ptr),
        get_return_pipe_name(exec_ptr)
    };
    *pipe_names_len = ARRAY_LEN(pipe_names);
    return pipe_names;
}

void free_pipe_names(char** pipe_names, int pipe_names_len) {
    for (int i = 0; i < pipe_names_len; i++) {
        free(pipe_names[i]);
    }
}

void create_pipes(ExecutionPointer* exec_ptr) {
    int len;
    char** pipe_names = get_pipe_names(exec_ptr, &len);
    for (int i = 0; i < len; i++) {
        mkfifo(pipe_names[i], 0666);
    }
    exec_ptr->recieve_pipe = open(pipe_names[0], O_RDONLY | O_NONBLOCK);
    exec_ptr->return_pipe = open(pipe_names[1], O_WRONLY);
    free_pipe_names(pipe_names, len);
}

void remove_pipes(ExecutionPointer* exec_ptr) {
    int len;
    char** pipe_names = get_pipe_names(exec_ptr, &len);
    for (int i = 0; i < len; i++) {
        unlink(pipe_names[i]);
    }
    free_pipe_names(pipe_names, len);

    close(exec_ptr->recieve_pipe);
    close(exec_ptr->return_pipe);

    exec_ptr->recieve_pipe = -1;
    exec_ptr->return_pipe = -1;
}

void run_program_in_background(ExecutionPointer* exec_ptr, char* program_name) {
    int len;
    char** pipe_names = get_pipe_names(exec_ptr, &len);
    
    char* command_args[] = {"python3", program_name, pipe_names[0], pipe_names[1], "&"};
    char* command = 

    FILE *fp = popen(command, "r");
    assert(fp != NULL);

    char buf[32];
    fgets(buf, ARRAY_LEN(buf), fp);

    char* pid_str = buf + 3;
    char* ptr;
    long pid = strtol(pid_str, &ptr, 10);
    exec_ptr->process_id = pid;

    free(command);
    free_pipe_names(pipe_names, len);
}

void kill(ExecutionPointer* exec_ptr, Error* err_reason, char* reason) {
    printf("killing program: %d\n", exec_ptr->id);
    if (reason != NULL) {
        printf("%s\n", reason);
    }
    if (err_reason != NULL) {
        log_error(*err_reason);
    }
    exec_ptr->dead = true;
    remove_pipes(exec_ptr);
}

void main(int num_programs, char *program_names[]) {
    init_rng();
    Grid* grid = init_grid();

    ExecutionPointer exec_ptrs[num_programs];
    for (int program_id = 0; program_id < num_programs; program_id++) {
        char* program_name = program_names[program_id];

        Point random_point = {rand() % GRID_SIZE, rand() % GRID_SIZE};
        Cell* cell = get_cell_at(grid, random_point);
        cell->program_name = program_name;

        exec_ptrs[program_id].id = program_id;
        exec_ptrs[program_id].loc = random_point;
        exec_ptrs[program_id].dead = false;
        exec_ptrs[program_id].running_cell = false;
        exec_ptrs[program_id].recieve_pipe = -1;
        exec_ptrs[program_id].return_pipe = -1;
        exec_ptrs[program_id].process_id = -1;
    }

    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        for (ExecutionPointer* exec_ptr = exec_ptrs; exec_ptr < exec_ptrs + num_programs; exec_ptr++) {
            if (exec_ptr->dead) {
                continue;
            }

            char* program_name = get_cell_at(grid, exec_ptr->loc)->program_name;
            if (program_name == NULL) {
                // nothing to run, move to the next cell
                exec_ptr->loc = add_points(exec_ptr->loc, init_point(1, 0));
                exec_ptr->running_cell = false;
            } else {
                // just arrived at a new cell, remove old pipes and make new ones, and run the program
                if (!exec_ptr->running_cell) {
                    if (exec_ptr->recieve_pipe != -1 && exec_ptr->return_pipe != -1) {
                        remove_pipes(exec_ptr);
                    }
                    create_pipes(exec_ptr);
                    run_program_in_background(exec_ptr, program_name);
                    exec_ptr->running_cell = true;
                }

                // block till we read input from the program, or we time out
                char output[MAX_RECIEVE_BUF];
                Result res = readLineWithTimeout(exec_ptr->recieve_pipe, output, MAX_RECIEVE_BUF, PROGRAM_TIMEOUT);
                if (res.is_err) { 
                    // program messed up, stop running it
                    kill(exec_ptr, &res.error, NULL);
                }

                // parse output as json
                cJSON *json = cJSON_Parse(output);
                if (json == NULL) {
                    Error error = err_enum(BAD_JSON);
                    kill(exec_ptr, &error, NULL);
                }

                // handle json command
                
            }
        }

        int num_running_programs = 0;
        for (ExecutionPointer* exec_ptr = exec_ptrs; exec_ptr < exec_ptrs + num_programs; exec_ptr++) {
            if (!exec_ptr->dead) {
                num_running_programs++;
            }
        }

        if (num_running_programs == 1) {
            break;
        }
    }
    free_grid(grid);
}
*/