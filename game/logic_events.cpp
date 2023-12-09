//This file handles events that the player will encounter and changes game state if needed
#include "logic_globals.h"
#include "../engine/render_globals.h"
#include "../engine/render_math.h"

int32_t player_health = 100;
int32_t player_ammo = 100;
int32_t player_kills = 0;
int32_t player_money = 100;

static int32_t death_reset = 0;

#ifdef GAMESCOM
int32_t input_idle_timer = 0;
#endif

void logic_new_game() {

    //reset all the values to initial game values
    //move camera to starting position
    //camera height, based roughly on an average mortal
    camera_position[1] = 1.8; //height: 1.8 m

    //in the city center
    camera_position[0] = -35;
    camera_position[2] = 0;

    //start next to city gates (usefull if testing object loading at origin with e.g. render_model_16bit etc.)
    //camera_position[0] = -8;
    //camera_position[2] = 8;

    //in the outskirts
    //camera_position[0] = 35;
    //camera_position[2] = -20;

    pitch = 0;
    yaw = 0;

    update_camera();

    //gameplay variables
    player_health = 100;
    player_ammo = 100;
    player_kills = 0;
    player_money = 100;

    #ifdef GAMESCOM
    input_idle_timer = 0;
    #endif

    //initialise quest npcs which are not randomly generated
    init_quest_npcs();
    

}



void logic_events() {

    //if a player loses their health, change to death menu (basically removing all controls)
    if (player_health <= 0) {
        menu = MENU_DEATH;

        //drop camera into the ground
        camera_position[1] = 1.0; //height: 1.8 m
        update_camera();
    }


    //handle events during gameplay
    if (menu == 0) {
    
        #ifdef GAMESCOM
        if (input_idle_timer >= INPUT_TIMEOUT) {
            menu = MENU_START;
            player_kills = 0;
            demo_progress = 0;
        }

        input_idle_timer++;
        #endif

        // allow the player to talk to one of the quest npcs if they are within range
        close_npc = -1;
        if (player_area != AREA_OUTSKIRTS) {
            for (int i = 0; i < MAX_QUEST_NPCS; i++) {
                if (npc_quest_list[MAX_QUEST_NPCS].status != -1) {
                    //we only load npcs if they are close to the player
                    int32_t distancex = npc_quest_list[i].x - camera_position_fixed_point[0];
                    if (distancex < 0) {
                        distancex *= -1;
                    }

                    int32_t distancey = npc_quest_list[i].y - camera_position_fixed_point[2];
                    if (distancey < 0) {
                        distancey *= -1;
                    }

                    if (distancex < QUEST_NPC_TALK_DISTANCE && distancey < QUEST_NPC_TALK_DISTANCE) {
                        close_npc = i;
                    }
                }
            }
        }

    } else if (menu == MENU_START) {
        //if the game is in the splash screen, hand over camera control to the demo logic
        logic_demo();

    } else if (menu == MENU_DEATH) {
        if (death_reset >= DEATH_DURATION) {
            menu = MENU_START;
            death_reset = 0;
            demo_progress = 0;
            logic_new_game();
        } else {
            death_reset++;
        }
    }

}
