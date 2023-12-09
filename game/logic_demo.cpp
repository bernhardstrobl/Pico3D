//the demo logic takes care of moving the camera across the landscape to showcase the engine
#include "logic_globals.h"
#include "../engine/render_globals.h"

int32_t demo_progress = 0;

#ifdef BENCHMARK
extern uint32_t avg_frametime;
extern uint32_t benchmark_frames;
extern int32_t benchmark_complete;
#endif

void logic_demo() {

    //start of demo
    if (demo_progress == 0) {
        //in the city center
        camera_position[0] = -35;
        camera_position[1] = 4;
        camera_position[2] = -40;
        pitch = 0;
        yaw = 0;
    
    //move to city center
    } else if (demo_progress < 1500) {
        camera_position[2] += 0.03;

    //move to pagoda
    } else if (demo_progress == 1500) {
        //camera_position[0] = -35;
        //camera_position[1] = 4;
        camera_position[2] = 40;
        yaw = PI;

    //from pagoda to city center
    } else if (demo_progress < 2500) {
        camera_position[2] -= 0.03;

    //move to city center facing shop
    } else if (demo_progress == 2500) {
        //camera_position[0] = -35;
        //camera_position[1] = 4;
        camera_position[2] = 5;
        yaw = PI / 2;

    //from city center to outskirts
    } else if (demo_progress < 4500) {
        camera_position[0] += 0.03;

    //down in the grass of the outskirts
    } else if (demo_progress == 4500) {
        camera_position[0] = 35;
        camera_position[1] = 1.8;
        camera_position[2] = -40;
        yaw = 0;

    //through the outskirts
    } else if (demo_progress < 7000) {
        camera_position[2] += 0.03;
    }


    update_camera();
    demo_progress++;
    if (demo_progress >= 7000) {
        demo_progress = 0;
        #ifdef BENCHMARK
            if(benchmark_complete == 0) {
                benchmark_complete = 1;
                avg_frametime = avg_frametime / benchmark_frames;
            }
        #endif
    }
}