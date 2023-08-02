//Controls NPC behavior in the city. Pretty similar to logic_zombies so some code duplication
//helps to keep things separate though
#include "logic_globals.h"
#include "../engine/render_globals.h"
#include "../engine/render_math.h"
#include "../engine/chunk_globals.h"

#include "../chunk_data.h"

#include "npc.h"
#include "npcleft.h"
#include "npcright.h"

struct npc npc_list[MAX_NPCS];

void logic_npc() {

    //destroy npcs if they are out of reach of player
    /*
    for (int i = 0; i < MAX_NPCS; i++) {
        if (npc_list[i].status != -1) {

            int32_t distancex = npc_list[i].x - camera_position_fixed_point[0];
            if (distancex < 0) {
                distancex *= -1;
            }

            int32_t distancey = npc_list[i].y - camera_position_fixed_point[2];
            if (distancey < 0) {
                distancey *= -1;
            }
            if (distancex > NPC_DESTROY_DISTANCE && distancey > NPC_DESTROY_DISTANCE) {
                npc_list[i].status = -1;
                break; //only destroy one npc per frame
            }
        }
    }*/

    //generate new npcs if possible
    for (int i = 0; i < MAX_NPCS; i++) {
        if (npc_list[i].status == -1) {

            //spawn npcs on the middle of random chunks
            //narrowed down to areas where it makes sense
            int32_t chunk_x = random(1, 10);
            int32_t chunk_y = random(1, 4);

            if (chunk_physics[chunk_x][chunk_y] == 1) {

                npc_list[i].status = 1;
                npc_list[i].direction = random(0, 3);
                npc_list[i].progress = 0;
                npc_list[i].x = (chunk_y * CHUNK_SIZE) + ((CHUNK_SIZE / 2) + CHUNK_OFFSET_Y);
                npc_list[i].y = (chunk_x * CHUNK_SIZE) + ((CHUNK_SIZE / 2) + CHUNK_OFFSET_X);
                
                uint8_t r = random(0, 15);
                uint8_t g = random(0, 15);
                uint8_t b = random(0, 15);

                //create random shirt color
                uint16_t color = g;
                color <<= 4;
                color |= b;
                color <<= 8;
                color |= r;

                npc_list[i].shirt_color = color;

                break; //only generate one new npcs per frame
            }

        }
    }

    //process npcs
    for (int i = 0; i < MAX_NPCS; i++) {
        if (npc_list[i].status != -1) {

            int32_t newx = npc_list[i].x;
            int32_t newy = npc_list[i].y;

            //sometimes change direction at random
            int32_t new_direction = random(0, 250);
            if (new_direction == 0) {
                npc_list[i].direction = random(0, 3);
            }


            if (npc_list[i].direction == 0) {
                newx += NPC_SPEED;
            } else if (npc_list[i].direction == 1) {
                newx -= NPC_SPEED;
            } else if (npc_list[i].direction == 2) {
                newy += NPC_SPEED;
            } else if (npc_list[i].direction == 3) {
                newy -= NPC_SPEED;
            }

            //check if new area is traversable
            uint8_t traversable = chunk_traversable(newx, newy, 1);

            if (traversable == 1) {
                npc_list[i].x = newx;
                npc_list[i].y = newy;

            //if we can't go there, choose a new direction
            } else {
                npc_list[i].direction = random(0, 3);
            }

            //reset walk animation
            if (npc_list[i].progress >= NPC_WALK_TIME) {
                npc_list[i].progress = 0;
            } else {
                npc_list[i].progress++;
            }

        }
    }

}


void npc_interpolate(struct triangle_16 *npc_movement, int16_t progress, int32_t i) {

    for (int32_t j = 0; j < NPC; j++) {
        struct triangle_32 new_triangle;

        int32_t x1 = npc[j].vertex1.x + (((npc_movement[j].vertex1.x - npc[j].vertex1.x) / (NPC_WALK_TIME / 4)) * progress);
        int32_t y1 = npc[j].vertex1.y + (((npc_movement[j].vertex1.y - npc[j].vertex1.y) / (NPC_WALK_TIME / 4)) * progress);
        int32_t z1 = npc[j].vertex1.z + (((npc_movement[j].vertex1.z - npc[j].vertex1.z) / (NPC_WALK_TIME / 4)) * progress);

        int32_t x2 = npc[j].vertex2.x + (((npc_movement[j].vertex2.x - npc[j].vertex2.x) / (NPC_WALK_TIME / 4)) * progress);
        int32_t y2 = npc[j].vertex2.y + (((npc_movement[j].vertex2.y - npc[j].vertex2.y) / (NPC_WALK_TIME / 4)) * progress);
        int32_t z2 = npc[j].vertex2.z + (((npc_movement[j].vertex2.z - npc[j].vertex2.z) / (NPC_WALK_TIME / 4)) * progress);

        int32_t x3 = npc[j].vertex3.x + (((npc_movement[j].vertex3.x - npc[j].vertex3.x) / (NPC_WALK_TIME / 4)) * progress);
        int32_t y3 = npc[j].vertex3.y + (((npc_movement[j].vertex3.y - npc[j].vertex3.y) / (NPC_WALK_TIME / 4)) * progress);
        int32_t z3 = npc[j].vertex3.z + (((npc_movement[j].vertex3.z - npc[j].vertex3.z) / (NPC_WALK_TIME / 4)) * progress);


        //reorient depending on orientation of npc
        //going east
        if (npc_list[i].direction == 0) {

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
        } else if (npc_list[i].direction == 1) {

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
        } else if (npc_list[i].direction == 3) {
            x1 *= -1;
            x2 *= -1;
            x3 *= -1;
            z1 *= -1;
            z2 *= -1;
            z3 *= -1;
        }



        new_triangle.vertex1.x = x1 + npc_list[i].x;
        new_triangle.vertex1.y = y1;
        new_triangle.vertex1.z = z1 + npc_list[i].y;
        
        new_triangle.vertex2.x = x2 + npc_list[i].x;
        new_triangle.vertex2.y = y2;
        new_triangle.vertex2.z = z2 + npc_list[i].y;

        new_triangle.vertex3.x = x3 + npc_list[i].x;
        new_triangle.vertex3.y = y3;
        new_triangle.vertex3.z = z3 + npc_list[i].y;



        new_triangle.shader_id = npc[j].shader_id;

        
        if(npc[j].vertex_parameter1.color == 0xf000) {

            new_triangle.vertex_parameter1.color = npc_list[i].shirt_color;
            new_triangle.vertex_parameter2.color = npc_list[i].shirt_color;
            new_triangle.vertex_parameter3.color = npc_list[i].shirt_color;

        } else {

            if (npc_list[i].direction == 0) {
                new_triangle.vertex_parameter1.color = npc[j].vertex_parameter3.color;
                new_triangle.vertex_parameter2.color = npc[j].vertex_parameter2.color;
                new_triangle.vertex_parameter3.color = npc[j].vertex_parameter1.color;
            } else {
                new_triangle.vertex_parameter1.color = npc[j].vertex_parameter1.color;
                new_triangle.vertex_parameter2.color = npc[j].vertex_parameter2.color;
                new_triangle.vertex_parameter3.color = npc[j].vertex_parameter3.color;
            }
        }


        render_triangle(new_triangle);
    }
 
    return;
}



void render_npcs() {
    for (int i = 0; i < MAX_NPCS; i++) {
        if (npc_list[i].status != -1) {

            //we only load npcs if they are close to the player
            int32_t distancex = npc_list[i].x - camera_position_fixed_point[0];
            if (distancex < 0) {
                distancex *= -1;
            }

            int32_t distancey = npc_list[i].y - camera_position_fixed_point[2];
            if (distancey < 0) {
                distancey *= -1;
            }

            //we also only load them in if their bounding box is visible
            if (render_view_frustum_culling(npc_list[i].x, FIXED_POINT_FACTOR, npc_list[i].y, FIXED_POINT_FACTOR / 2, FIXED_POINT_FACTOR, FIXED_POINT_FACTOR / 2) == 0) {
                continue;
            }

            if (distancex < NPC_VIEW_DISTANCE && distancey < NPC_VIEW_DISTANCE) {

                //depending on the status, we have to perform blending
                if (npc_list[i].status == 1) {
                    int16_t walk_stage = npc_list[i].progress / (NPC_WALK_TIME / 4);
                    int16_t progress = npc_list[i].progress % (NPC_WALK_TIME / 4);

                    struct triangle_16 *npc_movement;
                    //walk left forward
                    if (walk_stage == 0) {
                        npc_movement = npcleft;
                        npc_interpolate(npc_movement, progress, i);
                    
                    //walk left back
                    } else if (walk_stage == 1) {
                        progress = (NPC_WALK_TIME / 4) - progress;
                        npc_movement = npcleft;
                        npc_interpolate(npc_movement, progress, i);
                    
                    //walk right forward
                    } else if (walk_stage == 2) {
                        npc_movement = npcright;
                        npc_interpolate(npc_movement, progress, i);

                    //walk right back
                    } else if (walk_stage == 3) {
                        progress = (NPC_WALK_TIME / 4) - progress;
                        npc_movement = npcright;
                        npc_interpolate(npc_movement, progress, i);

                    } else {

                        for (int j = 0; j < NPC; j++) {

                            struct triangle_32 new_triangle;

                            new_triangle.vertex1.x = npc[j].vertex1.x + npc_list[i].x;
                            new_triangle.vertex1.y = npc[j].vertex1.y;
                            new_triangle.vertex1.z = npc[j].vertex1.z + npc_list[i].y;
                            
                            new_triangle.vertex2.x = npc[j].vertex2.x + npc_list[i].x;
                            new_triangle.vertex2.y = npc[j].vertex2.y;
                            new_triangle.vertex2.z = npc[j].vertex2.z + npc_list[i].y;

                            new_triangle.vertex3.x = npc[j].vertex3.x + npc_list[i].x;
                            new_triangle.vertex3.y = npc[j].vertex3.y;
                            new_triangle.vertex3.z = npc[j].vertex3.z + npc_list[i].y;

                            new_triangle.shader_id = npc[j].shader_id;
                            if(npc[j].vertex_parameter1.color == 0xf000) {
                                new_triangle.vertex_parameter1.color = npc_list[i].shirt_color;
                                new_triangle.vertex_parameter2.color = npc_list[i].shirt_color;
                                new_triangle.vertex_parameter3.color = npc_list[i].shirt_color;
                            } else {
                                new_triangle.vertex_parameter1.color = npc[j].vertex_parameter1.color;
                                new_triangle.vertex_parameter2.color = npc[j].vertex_parameter2.color;
                                new_triangle.vertex_parameter3.color = npc[j].vertex_parameter3.color;
                            }


                            render_triangle(new_triangle);
                            
                        }
                    }


                } else {
                
                    for (int j = 0; j < NPC; j++) {

                        struct triangle_32 new_triangle;

                        new_triangle.vertex1.x = npc[j].vertex1.x + npc_list[i].x;
                        new_triangle.vertex1.y = npc[j].vertex1.y;
                        new_triangle.vertex1.z = npc[j].vertex1.z + npc_list[i].y;
                        
                        new_triangle.vertex2.x = npc[j].vertex2.x + npc_list[i].x;
                        new_triangle.vertex2.y = npc[j].vertex2.y;
                        new_triangle.vertex2.z = npc[j].vertex2.z + npc_list[i].y;

                        new_triangle.vertex3.x = npc[j].vertex3.x + npc_list[i].x;
                        new_triangle.vertex3.y = npc[j].vertex3.y;
                        new_triangle.vertex3.z = npc[j].vertex3.z + npc_list[i].y;

                        new_triangle.shader_id = npc[j].shader_id;
                            if(npc[j].vertex_parameter1.color == 0xf000) {
                                new_triangle.vertex_parameter1.color = npc_list[i].shirt_color;
                                new_triangle.vertex_parameter2.color = npc_list[i].shirt_color;
                                new_triangle.vertex_parameter3.color = npc_list[i].shirt_color;
                            } else {
                                new_triangle.vertex_parameter1.color = npc[j].vertex_parameter1.color;
                                new_triangle.vertex_parameter2.color = npc[j].vertex_parameter2.color;
                                new_triangle.vertex_parameter3.color = npc[j].vertex_parameter3.color;
                            }


                        render_triangle(new_triangle);
                        


                    }
                }

                if (number_triangles > NPC_TRIANGLE_BUDGET) {
                    break;
                }

            }
        }
    }
    return;
}