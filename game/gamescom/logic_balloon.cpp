//Controls zombie behavior. Similar to logic_npc but more complicated since it reacts to player behavior.

#include "../logic_globals.h"
#include "../../engine/render_globals.h"
#include "../../engine/render_math.h"
#include "../../engine/chunk_globals.h"

#include "../chunk_data.h"

#include "balloon.h"
#include "balloon_pop.h"

struct npc zombie_list[MAX_ZOMBIES];

void logic_zombies() {


    //generate new npcs if possible
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (zombie_list[i].status == -1 && player_kills <= 50) {


            //spawn zombies on the middle of random chunks
            //narrowed down to areas where it makes sense
            int32_t chunk_x = random(1, 10);
            int32_t chunk_y = random(6, 10);

            if (chunk_physics[chunk_x][chunk_y] == 2) {
                zombie_list[i].status = 1;
                zombie_list[i].direction = random(0, 3);
                zombie_list[i].progress = 0;
                zombie_list[i].x = (chunk_y * CHUNK_SIZE) + ((CHUNK_SIZE / 2) + CHUNK_OFFSET_Y);
                zombie_list[i].y = (chunk_x * CHUNK_SIZE) + ((CHUNK_SIZE / 2) + CHUNK_OFFSET_X);
                
                uint8_t r = random(0, 15);
                uint8_t g = random(0, 15);
                uint8_t b = random(0, 15);

                //create random shirt color
                uint16_t color = g;
                color <<= 4;
                color |= b;
                color <<= 8;
                color |= r;

                zombie_list[i].shirt_color = color;
                zombie_list[i].health = ZOMBIE_HEALTH;

                break; //only generate one new npc per frame
            }
        }
    }

    //process npcs
    for (int i = 0; i < MAX_ZOMBIES; i++) {


        //normal moving
        if (zombie_list[i].status == 1) {



            int32_t speed = (ZOMBIE_RUN_SPEED + (player_kills * 3));

            int32_t newx = zombie_list[i].x;
            int32_t newy = zombie_list[i].y;



            //sometimes change direction at random
            int32_t new_direction = random(0, 250);
            if (new_direction == 0) {
                zombie_list[i].direction = random(0, 3);
            }


            if (zombie_list[i].direction == 0) {
                newx += speed;
            } else if (zombie_list[i].direction == 1) {
                newx -= speed;
            } else if (zombie_list[i].direction == 2) {
                newy += speed;
            } else if (zombie_list[i].direction == 3) {
                newy -= speed;
            }


            //check if new area is traversable
            uint8_t traversable = chunk_traversable(newx, newy, 2);

            if (traversable == 1) {
                zombie_list[i].x = newx;
                zombie_list[i].y = newy;

            //if we can't go there, choose a new direction
            } else {
                zombie_list[i].direction = random(0, 3);
            }


        //balloon pop
        } else if (zombie_list[i].status == 2) {
            
            //reset balloon
            if (zombie_list[i].progress >= ZOMBIE_ATTACK_DURATION - 1) {
                zombie_list[i].progress = 0;
                zombie_list[i].status = -1;
            } else {
                zombie_list[i].progress++;
            }
        }
    }
}




void pop_animation(struct triangle_16 *npc_movement, int16_t progress, int32_t i) {

    for (int32_t j = 0; j < BALLOON; j++) {
        struct triangle_32 new_triangle;

        int32_t x1 = balloon[j].vertex1.x + (((npc_movement[j].vertex1.x - balloon[j].vertex1.x) / (ZOMBIE_ATTACK_DURATION)) * progress);
        int32_t y1 = balloon[j].vertex1.y + (((npc_movement[j].vertex1.y - balloon[j].vertex1.y) / (ZOMBIE_ATTACK_DURATION)) * progress);
        int32_t z1 = balloon[j].vertex1.z + (((npc_movement[j].vertex1.z - balloon[j].vertex1.z) / (ZOMBIE_ATTACK_DURATION)) * progress);

        int32_t x2 = balloon[j].vertex2.x + (((npc_movement[j].vertex2.x - balloon[j].vertex2.x) / (ZOMBIE_ATTACK_DURATION)) * progress);
        int32_t y2 = balloon[j].vertex2.y + (((npc_movement[j].vertex2.y - balloon[j].vertex2.y) / (ZOMBIE_ATTACK_DURATION)) * progress);
        int32_t z2 = balloon[j].vertex2.z + (((npc_movement[j].vertex2.z - balloon[j].vertex2.z) / (ZOMBIE_ATTACK_DURATION)) * progress);

        int32_t x3 = balloon[j].vertex3.x + (((npc_movement[j].vertex3.x - balloon[j].vertex3.x) / (ZOMBIE_ATTACK_DURATION)) * progress);
        int32_t y3 = balloon[j].vertex3.y + (((npc_movement[j].vertex3.y - balloon[j].vertex3.y) / (ZOMBIE_ATTACK_DURATION)) * progress);
        int32_t z3 = balloon[j].vertex3.z + (((npc_movement[j].vertex3.z - balloon[j].vertex3.z) / (ZOMBIE_ATTACK_DURATION)) * progress);


        new_triangle.vertex1.x = x1 + zombie_list[i].x;
        new_triangle.vertex1.y = y1;
        new_triangle.vertex1.z = z1 + zombie_list[i].y;
        
        new_triangle.vertex2.x = x2 + zombie_list[i].x;
        new_triangle.vertex2.y = y2;
        new_triangle.vertex2.z = z2 + zombie_list[i].y;

        new_triangle.vertex3.x = x3 + zombie_list[i].x;
        new_triangle.vertex3.y = y3;
        new_triangle.vertex3.z = z3 + zombie_list[i].y;

        
        new_triangle.shader_id = 2;

        new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
        new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;

        if (j % 3 == 0) {
            new_triangle.vertex_parameter3.color = 0xF000;
        } else if (j % 3 == 1) {
            new_triangle.vertex_parameter3.color = 0x0F00;
        } else if (j % 3 == 2) {
            new_triangle.vertex_parameter3.color = 0x000F;
        }


        render_triangle(new_triangle);
    }
 
    return;
}


//overwritten with a bunch of balloon logic but we keep the terminology
void render_zombies() {

    //depending on time of day, alter behavior
    int32_t daylight = (global_time % (QUARTER_DAY * 4)) / QUARTER_DAY;

    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (zombie_list[i].status != -1) {

            //we only load npcs if they are close to the player
            int32_t distancex = zombie_list[i].x - camera_position_fixed_point[0];
            if (distancex < 0) {
                distancex *= -1;
            }

            int32_t distancey = zombie_list[i].y - camera_position_fixed_point[2];
            if (distancey < 0) {
                distancey *= -1;
            }

            //we also only load them in if their bounding box is visible
            if (render_view_frustum_culling(zombie_list[i].x, FIXED_POINT_FACTOR * 4, zombie_list[i].y, FIXED_POINT_FACTOR / 2, FIXED_POINT_FACTOR, FIXED_POINT_FACTOR / 2) == 0) {
                continue;
            }

            if (distancex < ZOMBIE_VIEW_DISTANCE && distancey < ZOMBIE_VIEW_DISTANCE) {

                //depending on the status, we have to perform blending
                if (zombie_list[i].status == 1) {

                    for (int j = 0; j < BALLOON; j++) {

                        struct triangle_32 new_triangle;

                        new_triangle.vertex1.x = balloon[j].vertex1.x + zombie_list[i].x;
                        new_triangle.vertex1.y = balloon[j].vertex1.y;
                        new_triangle.vertex1.z = balloon[j].vertex1.z + zombie_list[i].y;
                        
                        new_triangle.vertex2.x = balloon[j].vertex2.x + zombie_list[i].x;
                        new_triangle.vertex2.y = balloon[j].vertex2.y;
                        new_triangle.vertex2.z = balloon[j].vertex2.z + zombie_list[i].y;

                        new_triangle.vertex3.x = balloon[j].vertex3.x + zombie_list[i].x;
                        new_triangle.vertex3.y = balloon[j].vertex3.y;
                        new_triangle.vertex3.z = balloon[j].vertex3.z + zombie_list[i].y;

                        new_triangle.shader_id = 2;
                        
                        
                        new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
                        new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
                        
                        
                        if (j % 3 == 0) {
                            new_triangle.vertex_parameter3.color = 0xF000;
                        } else if (j % 3 == 1) {
                            new_triangle.vertex_parameter3.color = 0x0F00;
                        } else if (j % 3 == 2) {
                            new_triangle.vertex_parameter3.color = 0x000F;
                        }


                        render_triangle(new_triangle);
                    }

                //pop animation
                } else if (zombie_list[i].status == 2) {
                
                    int16_t progress = zombie_list[i].progress % ZOMBIE_ATTACK_DURATION;
                    
                    struct triangle_16 *npc_movement;

                    npc_movement = balloon_pop;

                    pop_animation(npc_movement, progress, i);
                

                } 

                if (number_triangles > ZOMBIE_TRIANGLE_BUDGET) {
                    break;
                }

            }
        }
    }
    return;
}