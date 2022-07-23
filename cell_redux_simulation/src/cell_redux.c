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
#include "world/grid.h"
#include "utility/error_handling/signalhandler.h"
#include "world/world_globals.h"

#define MAX_ITERATIONS 18000
#define MAX_SALT_SIZE 256

static void init_rng() {
    srand(time(NULL));
}

static int get_rand_between(int min, int max) {
    return (rand() % (max - min)) + min;
}


void shuffle(ExecutionPointer* array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          ExecutionPointer t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

void main(int argc, int argv[]) {
    init_rng();
    Grid* grid = init_grid();
    grid__world_globals = grid;

    int num_fighters;
    char** fighter_names = get_fighters(&num_fighters);


    FILE* fp = fopen("../cell_redux_server/battle_data", "w+");
    ExecutionPointer exec_ptrs[num_fighters];
    for (int fighter_id = 0; fighter_id < num_fighters; fighter_id++) {
        char* fighter_name = fighter_names[fighter_id];

        Point random_point = {rand() % GRID_SIZE, rand() % GRID_SIZE};
        Cell* cell = get_cell_at(grid, random_point);
        cell->function_name = strfmt("%s_init", fighter_name);
        cell->data = NULL;

        putc(get_function_index(cell->function_name) + 65, fp);
        putc(random_point.x + 33, fp);
        putc(random_point.y + 33, fp);

        exec_ptrs[fighter_id].id = fighter_id;
        exec_ptrs[fighter_id].loc = random_point;
        exec_ptrs[fighter_id].dead = false;

        putc(exec_ptrs[fighter_id].id + 65, fp);
        putc(exec_ptrs[fighter_id].loc.x + 33, fp);
        putc(exec_ptrs[fighter_id].loc.y + 33, fp);
    }

    
    FILE* metadata = fopen("../cell_redux_server/battle_metadata", "w+");
    fprintf(metadata, "%d %d ", GRID_SIZE, num_fighters);
    for (int i = 0; i < num_functions(); i++) {
        fputs(get_function(i), metadata);
        putc(' ', metadata);
    }
    fclose(metadata);


    cvector_vector_type(Command) all_commands[ARRAY_LEN(exec_ptrs)];
    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        int num_players_alive = 0;
        long checksum = hash_grid(grid);
        shuffle(exec_ptrs, num_fighters);
        for (ExecutionPointer* exec_ptr = exec_ptrs; exec_ptr < exec_ptrs + num_fighters; exec_ptr++) {
            if (exec_ptr->dead) {
                continue;
            }
            num_players_alive++;
            char* fighter_name = fighter_names[exec_ptr->id];
            char* function_name = get_cell_at(grid, exec_ptr->loc)->function_name;
            if (function_name == NULL) {
                exec_ptr->dead = true;
                printf("%s died because it tried to run an empty cell\n", fighter_name);
                continue;
            }

            location__world_globals = exec_ptr->loc;
            stats__world_globals = get_fighter_stats(fighter_name);
            commands__world_globals = NULL;

            num_commands__world_globals = 0;

            // We need a way to double check if the player is breaking the rules and modifying the grid directly.
            // To do this, we hash the grid and check if the grid hash is the same after the function call.
            call_function_with_name(function_name);
            if (checksum != hash_grid(grid)) {
                // the program messed with the grid, kill it
                exec_ptr->dead = true;
                printf("%s tried to break into the grid, nice try, but you're dead\n", fighter_name);
                continue;
            }

            if (cvector_size(commands__world_globals) > stats__world_globals.power) {
                fprintf(stderr, "%s made more calls than it's power value\n", fighter_name);
                exec_ptr->dead = true;
                continue;
            }

            int n_times_run_call = 0;
            for (int i = 0; i < cvector_size(commands__world_globals); i++) {
                if (commands__world_globals[i].command_type == CALL) {
                    n_times_run_call++;
                }
            }

            if (n_times_run_call > 1) {
                fprintf(stderr, "%s tried to move twice in one turn\n", fighter_name);
                exec_ptr->dead = true;
                continue;
            }

            all_commands[exec_ptr->id] = commands__world_globals;
        }

        for (int i = 0; i < num_fighters; i++) {
            if (exec_ptrs[i].dead) {
                continue;
            }
            int id = exec_ptrs[i].id;
            for (int j = 0; j < cvector_size(all_commands[id]); j++) {
                char* fighter_name = fighter_names[id];
                Stats stats = get_fighter_stats(fighter_name);
                char* funcdiff = NULL;
                Point locdiff;
                if (!run_command(&exec_ptrs[i], &all_commands[id][j], grid, &stats, fighter_name, &funcdiff, &locdiff)) {
                    exec_ptrs[i].dead = true;
                    break;
                }
                putc(get_function_index(funcdiff) + 65, fp);
                putc(locdiff.x + 33, fp);
                putc(locdiff.y + 33, fp);

                putc(exec_ptrs[i].id + 65, fp);
                putc(exec_ptrs[i].loc.x + 33, fp);
                putc(exec_ptrs[i].loc.y + 33, fp);
            }
            cvector_free(all_commands[id]);
        }

        if (num_players_alive <= 1) {
            break;
        }
    }
    free_grid(grid);
    fclose(fp);

    // print everyone still alive
    for (ExecutionPointer* exec_ptr = exec_ptrs; exec_ptr < exec_ptrs + num_fighters; exec_ptr++) {
        if (!exec_ptr->dead) {
            printf("%s\n", fighter_names[exec_ptr->id]);
        }
    }
}