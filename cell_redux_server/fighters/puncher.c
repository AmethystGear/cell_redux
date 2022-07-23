Stats puncher_stats = {.speed = 3, .range = 10, .power = 5};

static void bomb() {
    world_kill();
}

static void init() {
    world_write_function("puncher_bomb", 4, 0);
    world_write_function("puncher_bomb", 5, 0);
    world_write_function("puncher_bomb", 6, 0);
    world_write_function("puncher_bomb", 7, 0);
    world_call("puncher_init", 3, 0);
}