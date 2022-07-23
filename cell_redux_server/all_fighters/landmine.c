Stats landmine_stats = {.speed = 0, .range = 6, .power = 58};

static void bomb() {
    world_kill();
}

static void init() {
    int range = landmine_stats.range / 2;
    for (int x = -range; x < range + 1; x++) {
        for (int y = -range; y < range + 1; y++) {
            if (x == 0 && y == 0) {
                continue; // skip when it's our own square
            }
            world_write_function("landmine_bomb", x, y);
        }
    }
}