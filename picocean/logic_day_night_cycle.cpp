#include "logic_globals.h"
#include "../engine/render_globals.h"

uint32_t global_time;
static int8_t daylight = 0;

//process sky color based on time, also provide color bias for shaders
void logic_day_night_cycle() {

    uint8_t sky_r;
    uint8_t sky_g;
    uint8_t sky_b;

    daylight = (global_time % (QUARTER_DAY * 4)) / QUARTER_DAY;
    int32_t progress = global_time % QUARTER_DAY;

    //define color pallette depending on day
    //day
    if (daylight == 0) {
        sky_r = DAY_R;
        sky_g = DAY_G;
        sky_b = DAY_B;

        light_falloff = 0;

    //dusk
    } else if (daylight == 1) {
        //calculate bias toward night
        int32_t r = ((NIGHT_R - DAY_R) * progress) / QUARTER_DAY;
        int32_t g = ((NIGHT_G - DAY_G) * progress) / QUARTER_DAY;
        int32_t b = ((NIGHT_B - DAY_B) * progress) / QUARTER_DAY;

        sky_r = DAY_R + r;
        sky_g = DAY_G + g;
        sky_b = DAY_B + b;

        light_falloff = (MAX_FALLOFF * progress) / QUARTER_DAY;

    //night
    } else if (daylight == 2) {

        sky_r = NIGHT_R;
        sky_g = NIGHT_G;
        sky_b = NIGHT_B;

        light_falloff = MAX_FALLOFF;

    //dawn
    } else {

        int32_t r = ((DAY_R - NIGHT_R) * progress) / QUARTER_DAY;
        int32_t g = ((DAY_G - NIGHT_G) * progress) / QUARTER_DAY;
        int32_t b = ((DAY_B - NIGHT_B) * progress) / QUARTER_DAY;
        sky_r = NIGHT_R + r;
        sky_g = NIGHT_G + g;
        sky_b = NIGHT_B + b;

        light_falloff = (MAX_FALLOFF * (QUARTER_DAY - progress)) / QUARTER_DAY;

    }


    //write final sky color
    sky = sky_g;
    sky <<= 4;
    sky |= sky_b;
    sky <<= 8;
    sky |= sky_r;


}