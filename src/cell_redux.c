#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#include "world/world_interface.h"
#include "world/grid.h"
#include "fighter_interface/fighter_interface.h"
#include "world/execution_pointer.h"
#include "utility/utils.h"
#include "world/commands.h"

#define MAX_RECIEVE_BUF 512
#define MAX_ITERATIONS 10000
#define PROGRAM_TIMEOUT 5

void init_rng() {
    srand(time(NULL));
}

void main(int argc, int argv[]) {
    init_rng();
    Grid* grid = init_grid();

    int num_fighters;
    char** fighter_names = get_fighters(&num_fighters);

    ExecutionPointer exec_ptrs[num_fighters];
    for (int fighter_id = 0; fighter_id < num_fighters; fighter_id++) {
        char* fighter_name = fighter_names[fighter_id];

        Point random_point = {rand() % GRID_SIZE, rand() % GRID_SIZE};
        Cell* cell = get_cell_at(grid, random_point);
        cell->function_name = strfmt("%s_init", fighter_name);
        cell->data = NULL;

        exec_ptrs[fighter_id].id = fighter_id;
        exec_ptrs[fighter_id].loc = random_point;
        exec_ptrs[fighter_id].dead = false;
    }

    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        for (ExecutionPointer* exec_ptr = exec_ptrs; exec_ptr < exec_ptrs + num_fighters; exec_ptr++) {
            if (exec_ptr->dead) {
                continue;
            }
            char* function_name = get_cell_at(grid, exec_ptr->loc)->function_name;
            if (function_name == NULL) {
                exec_ptr->dead = true;
            } else {
                world_interface___COPIED_GRID = copy_grid(grid);
                memset(world_interface___commands, 0, sizeof(world_interface___commands));
                world_interface___num_commands = 0;

                call_function_with_name(function_name);

                for (int i = 0; i < world_interface___num_commands; i++) {
                    run_command(exec_ptr, &world_interface___commands[i], grid);
                }
                free_grid(world_interface___COPIED_GRID);
            }
        }
    }
    free_grid(grid);

    for (ExecutionPointer* exec_ptr = exec_ptrs; exec_ptr < exec_ptrs + num_fighters; exec_ptr++) {
        printf("%s is alive", fighter_names[exec_ptr->id]);
    }
}