//displays text when a player changes areas, npcs leave comments and shows things like health/ammo in the outskirts etc.
//this differs from menus in that information is only overlaid and movement is never compromised
#include "picosystem.hpp"

#include "logic_globals.h"
#include "../engine/render_globals.h"
#include "../engine/chunk_globals.h"

using namespace picosystem;

#define INFO_TIMER 120 //display any info for around 3 seconds

extern int32_t show_battery;

int32_t player_area = 0;

int32_t info_time_remain = 0;
int32_t info_display = 0;

int32_t dialogue_time_remain = 0;
int32_t dialogue_display = 0;

//check if the area where the player has changed from the previous frame
void logic_player_area() {

    //we get the array position of the center chunk where the player is
    int32_t chunk_x;
    int32_t chunk_y;

    chunk_locate(camera_position_fixed_point[0], camera_position_fixed_point[2], chunk_x, chunk_y);

    int32_t new_area;

    if (chunk_y >= 6) {
        new_area = AREA_OUTSKIRTS;
        if (chunk_y >= 10 && chunk_x < 3) {
            new_area = AREA_OUTSKIRT_STABLES;
        }
    } else if (chunk_x > 7) {
        new_area = AREA_YAKUZA_ALLEY;
    } else if (chunk_x < 5) {
        new_area = AREA_DOWNTOWN;
    } else {
        new_area = AREA_CITY_CENTER;
    }

    if (new_area != player_area) {
        info_time_remain = INFO_TIMER;
        player_area = new_area;
    }
}


void display_info() {

    //we only display ammo and health when the player is in the outskirts
    if (player_area == AREA_OUTSKIRTS) {

        #ifndef GAMESCOM
        if (player_health < 30) {
            pen(15, 0, 0);
        } else {
            pen(15, 15, 15);
        }

        text("+" + str(player_health), 0, 0);

        if(player_ammo < 30) {
            pen(15, 0, 0);
        } else {
            pen(15, 15, 15);
        }

        //Position text depending on amount of bullets left
        if (player_ammo == 0) {
            text("No Ammo", 75, 0);
        } else if (player_ammo > 99) {
            text(str(player_ammo), 100, 0);
        } else if (player_ammo > 9) {
            text(str(player_ammo), 107, 0);
        } else {
            text(str(player_ammo), 114, 0);
        }


        //we only show score when shooting balloons
        #else

        pen(15, 15, 15);
        text(str(player_kills), 0, 0);

        #endif


        //show cross hairs (or point) for shooting
        pen(15, 0, 0);
        pixel(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        pixel(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 1);
        pixel(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 2);
        pixel(SCREEN_WIDTH / 2 - 1, SCREEN_HEIGHT / 2);
        pixel(SCREEN_WIDTH / 2 - 2, SCREEN_HEIGHT / 2);
        pixel(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 1);
        pixel(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 2);
        pixel(SCREEN_WIDTH / 2 + 1, SCREEN_HEIGHT / 2);
        pixel(SCREEN_WIDTH / 2 + 2, SCREEN_HEIGHT / 2);
    }

    //show area name if needed
    pen(15, 15, 15);
    if (info_time_remain != 0) {
        if (player_area == AREA_OUTSKIRTS) {
            text("Outskirts", 38, 20);
        } else if (player_area == AREA_YAKUZA_ALLEY) {
            text("Back Alley", 35, 20);
        } else if (player_area == AREA_DOWNTOWN) {
            text("Downtown", 38, 20);
        } else if (player_area == AREA_CITY_CENTER) {
            text("City Center", 35, 20);
        } else if (player_area == AREA_OUTSKIRT_STABLES) {
            text("Outskirt", 37, 20);
            text("Stable", 40, 30);
        }
        info_time_remain--;
    }

    //show talk button if quest/shop npc is close
    if (close_npc != -1) {
        if (close_npc == 1 && (npc_quest_list[close_npc].dialogue == 11 || npc_quest_list[close_npc].dialogue == 12)) {
            text("A: Buy Ammo", 0, 110);
        }else {
            text("A: Talk", 0, 110);
        }
    }

    if (show_battery == 1) {
        text("Battery:" + str(battery()), 60, 110);
    }

    //show dialogue from npc
    if (dialogue_time_remain != 0) {

        #ifdef GAMESCOM
        switch(dialogue_display) {
            case 0: text("Hey there!", 0, 100); break;
            case 1: text("It's pretty boring", 0, 90); text("being on guard duty", 0, 100);break;
            case 2: text("So I set up some", 0, 90); text("balloons outside", 0, 100); break;
            case 3: text("See if you can", 0, 90); text("shoot them all down", 0, 100); break;
            case 4: text("The gates close", 0, 90); text("at night though", 0, 100); break;
            case 5: text("But the balloons", 0, 90); text("glow in the dark!", 0, 100); break;
            case 6: if (player_kills < 100) {
                        text("See if you can shoot", 0, 90); 
                        text("all the balloons down!", 0, 100); break;
                    } else {
                        text("Wow you got them all!", 0, 90); 
                        text("You're a balloon master!", 0, 100); break;
                    }
        }
        #else
        switch(dialogue_display) {
            case 0: text("Hey there! You seem", 0, 90); text("to be new around here.", 0, 100);break;
            case 1: text("I am guarding the", 0, 90); text("city from zombies.", 0, 100);break;
            case 2: text("Be careful if you", 0, 90); text("go out, they will attack.", 0, 100); break;
            case 3: text("The gates close at", 0, 90); text("night. Be back by then.", 0, 100); break;
            case 4: text("Zombies are more", 0, 90); text("aggressive in the dark.", 0, 100); break;
            case 5: text("Try not to run out", 0, 90); text("of ammo out there.", 0, 100); break;
            case 6: if (player_kills <= 100) {
                        text("See if you can kill", 0, 90); 
                        text("a couple of the zombies.", 0, 100); break;
                    } else {
                        text("Wow you got over a 100!", 0, 90); 
                        text("You're a zombie killer!", 0, 100); break;
                    }

            
            case 10:text("Need Bullets?", 0, 90); text("10$ for 5.", 0, 100);break;
            case 11:text("Good hunting.", 0, 90); text(str(player_money + QUEST_AMMO_COST) + "$ -> " + str(player_money) + "$", 0, 100); break;
            case 12:text("You don't have", 0, 90); text("enough money.", 0, 100); break;


            case 20: text("Oh, well done for", 0, 90); text("making it here.", 0, 100);break;
            case 21: text("The fire keeps the", 0, 90); text("zombies away.", 0, 100);break;
            case 22: text("This place used to", 0, 90); text("have horses but...", 0, 100); break;
            case 23: text("I mean look at the", 0, 90); text("surroundings...", 0, 100); break;
            case 24: text("The stable owner is", 0, 90); text("pretty unhappy.", 0, 100); break;
            case 25: text("Sells ammo to anyone", 0, 90); text("hoping it will help.", 0, 100); break;
            case 26: text("Don't get killed and", 0, 90); text("get some rest here.", 0, 100); break;
        }
        #endif

        dialogue_time_remain--;
    }

    

}
