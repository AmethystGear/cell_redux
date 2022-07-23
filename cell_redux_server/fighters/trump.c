Stats trump_stats = {.speed = 2,.range = 10,.power = 10 };

static void orange() {
    world_kill();
}

static void init() {
    world_write_function("trump_orange", 1, 0);
    world_write_function("trump_orange", -1, 0);
    world_write_function("trump_orange", 2, 0);
    world_write_function("trump_orange", -2, 0);
    world_write_function("trump_orange", 3, 0);
    world_write_function("trump_orange", -3, 0);
    world_write_function("trump_orange", 4, 0);
    world_write_function("trump_orange", -4, 0);
    world_call("trump_init", 0, 1);
}