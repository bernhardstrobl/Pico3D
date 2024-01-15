#include "logic_globals.h"
#include "../engine/render_globals.h"
#include "../engine/render_math.h"
#include "../engine/chunk_globals.h"

uint32_t global_time;
static int8_t daylight = 0;

//uses a more complex skybox with horizon, ocean etc.
#ifdef PICOCEAN
extern uint16_t sky_begin;
extern uint16_t horizon_begin;

extern color_t sky_color;
extern color_t horizon_color;
extern color_t ocean_color;
#endif

//determine the starting point of the sky, horizon and ocean
void logic_skybox() {




    

    sky_begin = 0;
    horizon_begin = 999;


    int32_t x = camera_position_fixed_point[0];
    int32_t z = camera_position_fixed_point[2];

    //if we are dealing with large worlds, reset position close to origin
    #ifndef NO_GLOBAL_OFFSET
    x -= global_offset_x * CHUNK_SIZE;
    z -= global_offset_z * CHUNK_SIZE;
    #endif



    //use the position of the player to create a point in front of them far in the horizon
    //assume a position at origin moved from camera
    struct vertex_32 point;
    struct vertex_32 rotated_point;

    point.x = 100 * FIXED_POINT_FACTOR;
    point.z = 0;

    rotated_point.x = (point.x * sin(yaw)) + (point.z * cos(yaw));
    rotated_point.z = (point.x * cos(yaw)) - (point.z * sin(yaw));


    //add the rotated point onto the camera offset
    point.x = x + rotated_point.x;
    point.y = 0;
    point.z = z + rotated_point.z;


    //transform point into screen space
    int32_t w = ((mat_vp[3][0] * point.x) + (mat_vp[3][1] * point.y) + (mat_vp[3][2] * point.z) + (mat_vp[3][3] * FIXED_POINT_FACTOR)) / FIXED_POINT_FACTOR;

    int32_t output_y = (((mat_vp[1][0] * point.x) + (mat_vp[1][1] * point.y) + (mat_vp[1][2] * point.z) + (mat_vp[1][3] * FIXED_POINT_FACTOR))) / w;
    output_y = SCREEN_HEIGHT - ((output_y + FIXED_POINT_FACTOR) * (SCREEN_HEIGHT - 1)) / FIXED_POINT_FACTOR / 2;

    horizon_begin = output_y;


    //do the same for the sky beginning
    point.y = -250 * FIXED_POINT_FACTOR;
    //transform point into screen space
    w = ((mat_vp[3][0] * point.x) + (mat_vp[3][1] * point.y) + (mat_vp[3][2] * point.z) + (mat_vp[3][3] * FIXED_POINT_FACTOR)) / FIXED_POINT_FACTOR;
    output_y = (((mat_vp[1][0] * point.x) + (mat_vp[1][1] * point.y) + (mat_vp[1][2] * point.z) + (mat_vp[1][3] * FIXED_POINT_FACTOR))) / w;
    output_y = SCREEN_HEIGHT - ((output_y + FIXED_POINT_FACTOR) * (SCREEN_HEIGHT - 1)) / FIXED_POINT_FACTOR / 2;

    sky_begin = output_y;


    //if the sky is below the horizon, ignore
    if (sky_begin >= horizon_begin) {
        sky_begin = 0;
    }

    //make sure ocean is drawn when looking down
    if (horizon_begin > SCREEN_HEIGHT && pitch < 0.5) {
        sky_begin = 0;
        horizon_begin = 0;
    }



    //clamp values
    if (sky_begin < 0) {
        sky_begin = 0;
    }

    if (horizon_begin < 0) {
        horizon_begin = 0;
    }

    if (sky_begin >= SCREEN_HEIGHT) {
        sky_begin = SCREEN_HEIGHT - 1;
    }

    if (horizon_begin >= SCREEN_HEIGHT) {
        horizon_begin = SCREEN_HEIGHT - 1;
    }


}


//process sky color based on time, also provide color bias for shaders
void logic_day_night_cycle() {

    //calculate using large bounding box surrounding player where horizon & sky begin


    logic_skybox();



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

