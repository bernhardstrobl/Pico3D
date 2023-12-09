//handles npc logic for npcs that are important for the game storyline like quest givers and shop keepers
#include "logic_globals.h"
#include "../engine/render_globals.h"
#include "../engine/render_math.h"
#include "npc.h"

int32_t close_npc;

struct npc npc_quest_list[MAX_QUEST_NPCS];

void init_quest_npcs() {

    //Guard at the gate
    npc_quest_list[0].status = 1;
    npc_quest_list[0].direction = 2;
    npc_quest_list[0].shirt_color = 0x0F00; 
    npc_quest_list[0].dialogue = 0; 
    npc_quest_list[0].x = -5 * FIXED_POINT_FACTOR;
    npc_quest_list[0].y = -1 * FIXED_POINT_FACTOR;


    //Seller at the stable
    npc_quest_list[1].status = 1;
    npc_quest_list[1].direction = 2;
    npc_quest_list[1].shirt_color = 0x0F0F; 
    npc_quest_list[1].dialogue = 10; 
    npc_quest_list[1].x = 45 * FIXED_POINT_FACTOR;
    npc_quest_list[1].y = -41 * FIXED_POINT_FACTOR;


    //Fireplace guy
    npc_quest_list[2].status = 1;
    npc_quest_list[2].direction = 1;
    npc_quest_list[2].shirt_color = 0xAF0A;
    npc_quest_list[2].dialogue = 20;
    npc_quest_list[2].x = 48 * FIXED_POINT_FACTOR;
    npc_quest_list[2].y = -35 * FIXED_POINT_FACTOR;
}


//this code handles talking to nearby npcs
void talk_quest_npc() {
    if (close_npc != -1) {
        //npcs with negative 1 open menus (not implemented)
        if (npc_quest_list[close_npc].dialogue == -1) {

        } else {
            dialogue_display = npc_quest_list[close_npc].dialogue;
            npc_quest_list[close_npc].dialogue++; //increase dialogue counter

            //give out same text for guard once conversation limit is reached
            if (npc_quest_list[close_npc].dialogue == 7) {
                npc_quest_list[close_npc].dialogue = 6;
            }

            //Stable owner
            if (npc_quest_list[close_npc].dialogue == 12 || npc_quest_list[close_npc].dialogue == 13) {

                if (player_money >= QUEST_AMMO_COST) {
                    player_money -= QUEST_AMMO_COST;
                    player_ammo += QUEST_AMMO_PURCHASE;
                    npc_quest_list[close_npc].dialogue = 11;
                    dialogue_display = 11;
                } else {
                    npc_quest_list[close_npc].dialogue = 12;
                    dialogue_display = 12;
                }
            }

            //give out same text for fireplace guy
            if (npc_quest_list[close_npc].dialogue == 27) {
                npc_quest_list[close_npc].dialogue = 26;
            }

            dialogue_time_remain = DIALOGUE_TIMER;
        }
    }

}

void render_quest_npcs() {

    if (skip_frame == 1) {
        return;
    }

    for (int i = 0; i < MAX_QUEST_NPCS; i++) {
        if (npc_quest_list[i].status != -1) {

            //we only load npcs if they are close to the player
            int32_t distancex = npc_quest_list[i].x - camera_position_fixed_point[0];
            if (distancex < 0) {
                distancex *= -1;
            }

            int32_t distancey = npc_quest_list[i].y - camera_position_fixed_point[2];
            if (distancey < 0) {
                distancey *= -1;
            }

            //we also only load them in if their bounding box is visible
            if (render_view_frustum_culling(npc_quest_list[i].x, FIXED_POINT_FACTOR, npc_quest_list[i].y, FIXED_POINT_FACTOR / 2, FIXED_POINT_FACTOR, FIXED_POINT_FACTOR / 2) == 0) {
                continue;
            }

            if (distancex < NPC_VIEW_DISTANCE && distancey < NPC_VIEW_DISTANCE) {
                for (int j = 0; j < NPC; j++) {

                    struct triangle_32 new_triangle;

                    int32_t x1 = npc[j].vertex1.x;
                    int32_t y1 = npc[j].vertex1.y;
                    int32_t z1 = npc[j].vertex1.z;
                    
                    int32_t x2 = npc[j].vertex2.x;
                    int32_t y2 = npc[j].vertex2.y;
                    int32_t z2 = npc[j].vertex2.z;

                    int32_t x3 = npc[j].vertex3.x;
                    int32_t y3 = npc[j].vertex3.y;
                    int32_t z3 = npc[j].vertex3.z;

                    //reorient depending on orientation of npc
                    //going east
                    if (npc_quest_list[i].direction == 0) {

                        int32_t temp1 = x1;
                        int32_t temp2 = x2;
                        int32_t temp3 = x3;
                        x1 = z1;
                        x2 = z2;
                        x3 = z3;
                        z1 = temp1;
                        z2 = temp2;
                        z3 = temp3;

                        z1 *= -1;
                        z2 *= -1;
                        z3 *= -1;

                    //west
                    } else if (npc_quest_list[i].direction == 1) {

                        int32_t temp1 = x1;
                        int32_t temp2 = x2;
                        int32_t temp3 = x3;
                        x1 = z1;
                        x2 = z2;
                        x3 = z3;
                        z1 = temp1;
                        z2 = temp2;
                        z3 = temp3;

                        x1 *= -1;
                        x2 *= -1;
                        x3 *= -1;
                    
                    //north
                    } else if (npc_quest_list[i].direction == 3) {
                        x1 *= -1;
                        x2 *= -1;
                        x3 *= -1;
                        z1 *= -1;
                        z2 *= -1;
                        z3 *= -1;
                    }



                    new_triangle.vertex1.x = x1 + npc_quest_list[i].x;
                    new_triangle.vertex1.y = y1;
                    new_triangle.vertex1.z = z1 + npc_quest_list[i].y;
                    
                    new_triangle.vertex2.x = x2 + npc_quest_list[i].x;
                    new_triangle.vertex2.y = y2;
                    new_triangle.vertex2.z = z2 + npc_quest_list[i].y;

                    new_triangle.vertex3.x = x3 + npc_quest_list[i].x;
                    new_triangle.vertex3.y = y3;
                    new_triangle.vertex3.z = z3 + npc_quest_list[i].y;


                    new_triangle.shader_id = npc[j].shader_id;
                    if(npc[j].vertex_parameter1.color == 0xf000) {
                        new_triangle.vertex_parameter1.color = npc_quest_list[i].shirt_color;
                        new_triangle.vertex_parameter2.color = npc_quest_list[i].shirt_color;
                        new_triangle.vertex_parameter3.color = npc_quest_list[i].shirt_color;
                    } else {
                        new_triangle.vertex_parameter1.color = npc[j].vertex_parameter1.color;
                        new_triangle.vertex_parameter2.color = npc[j].vertex_parameter2.color;
                        new_triangle.vertex_parameter3.color = npc[j].vertex_parameter3.color;
                    }
                    render_triangle(new_triangle);
                    
                }
            }
        }
    }
}