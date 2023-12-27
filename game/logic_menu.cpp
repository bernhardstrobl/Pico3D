//display and handle different menus (main, shop and death etc.)
//menus usually remove control in some way from the player by deactivating normal movement
#include "picosystem.hpp"

#include "logic_globals.h"

using namespace picosystem;

int32_t menu = MENU_START;

#ifdef BENCHMARK
extern int32_t benchmark_complete;
#endif

#ifdef FRAME_COUNTER
extern uint32_t perf_25_below;
extern uint32_t perf_50_below;
extern uint32_t perf_75_below;
extern uint32_t perf_75_above;
#endif

void display_menu() {

    if (menu == MENU_MAIN) {
        pen(15, 15, 15);

        text("MENU:", 0, 0);

        text("Health: " + str(player_health), 0, 20);
        text("Ammo: " + str(player_ammo), 0, 30);
        text("Kills: " + str(player_kills), 0, 40);
        text("Money: " + str(player_money) + "$", 0, 50);
        text("Battery:" + str(battery()), 0, 60);

        //additional debug info if needed
        #ifdef FRAME_COUNTER
        text("40:" + str(perf_25_below), 60, 0);
        text("20:" + str(perf_50_below), 60, 10);
        text("13:" + str(perf_75_below), 60, 20);
        text("<13:" + str(perf_75_above), 60, 30);
        #endif

        //additional debug info if needed
        #ifdef DEBUG_INFO
        //amount of triangles passed on to Core1 for rasterization
        text("#R: " + str(rendered_triangles), 0, 70);

        //amount of triangles stored in the chunk cache
        text("#C: " + str(cached_triangles), 60, 70);

        //Core 0 logic (update) time
        text("C0U:" + str(logic_time), 0, 80);
        #endif


        //Info Text to exit menu/change certain settings
        text("UP/DOWN: Display(" + str(brightness) + ")", 0, 100);
        text("Y: Exit", 0, 110);
        //text("Y: Exit   A: Sound ON", 0, 110);

    //Start screen/splash screen
    } else if (menu == MENU_START) {
        pen(15, 15, 15);

        #ifdef BENCHMARK
        if (benchmark_complete == 0) {
            text("BENCHMARKING", 0, 0);
        }
        #else
        if (demo_progress < 2500) {
            text("Pico3D Engine", 28, 20);
        }

        if ((demo_progress / 32) % 2 == 0) {
            text("Press any button", 20, 90);
        }
        #endif

    } else if (menu == MENU_DEATH) {
        pen(15, 0, 0);
        text("YOU DIED!", 38, 20);
    }

}