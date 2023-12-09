#include "picosystem.hpp"

static int32_t last_rand = 0;

//we have a general purpose pseudorandom RNG here (complete garbage but it will do)
int32_t random(int32_t rand_min, int32_t rand_max) {


    int32_t rand = (picosystem::time() + last_rand) % ((rand_max + 1) - rand_min);
    last_rand = rand;
    return rand_min + rand;
    //return 4;
}