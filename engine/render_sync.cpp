#include "picosystem.hpp"

using namespace picosystem;

int32_t render_sync() {


    uint32_t core1_time = 0;

    //we have to check if core 1 has completed its task
    //if it has, we can swap framebuffers and triangle lists
    if (multicore_fifo_pop_timeout_us(5, &core1_time)) {
        //instead of copying the framebuffer to the screen buffer (which is wasteful),
        //we simply swap in the framebuffer as the new screen buffer and use the old
        //screen buffer as the new framebuffer
        buffer_t *TEMP_FB;
        TEMP_FB = SCREEN;
        SCREEN = FRAMEBUFFER;
        FRAMEBUFFER = TEMP_FB;
        target(SCREEN);


        //swap triangle lists
        struct triangle_16 *temp_triangle_list;
        temp_triangle_list = triangle_list_current;
        triangle_list_current = triangle_list_next;
        triangle_list_next = temp_triangle_list;
        

        //if any writes to flash memory are to be done, do it here before core1 is released



        //once we have done all the needed transfers, we can tell core 1 to start rasterizing again
        //as argument we pass the expected amount of triangles to render
        multicore_fifo_push_blocking(number_triangles);


        //performance counters
        #ifdef FRAME_COUNTER
            if (core1_time < 25000) {
                perf_25_below++;
            } else if (core1_time < 50000) {
                perf_50_below++;
            } else if (core1_time < 75000) {
                perf_75_below++;
            } else if (core1_time >= 75000) {
                perf_75_above++;
            }
        #endif

    //if core 1 is still rendering, we have to keep the old frame and old triangle lists
    } else {
        //skipped_frames++;
    }


    #ifdef CPU_LED_LOAD
    if (core1_time > 50000) {
        led(25, 0, 0);
    } else if (core1_time > 25000) {
        led(25, 25, 0);
    } else if (core1_time > 0) {
        led(0, 25, 0);
    }
    #endif


    return core1_time;
}