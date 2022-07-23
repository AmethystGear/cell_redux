#include <stdlib.h>
Stats sonic_stats = {.speed = 58,.range = 1,.power = 5 };

static void homing_attack() {
    world_kill ();
}

static int rand_between (int min, int max) {
    return rand () % (max - min) + min;
}

static void init() {
  int max_speed = sonic_stats.speed / 2;
  int min_speed = -sonic_stats.speed / 2;
  world_call ("sonic_init", rand_between (min_speed, max_speed), rand_between (min_speed, max_speed));
  world_write_function ("sonic_homing_attack", 1, 0);
  world_write_function ("sonic_homing_attack", -1, 0);
  world_write_function ("sonic_homing_attack", 0, 1);
  world_write_function ("sonic_homing_attack", 0, -1);
}