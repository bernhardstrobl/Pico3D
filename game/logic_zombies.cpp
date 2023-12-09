//Controls zombie behavior. Similar to logic_npc but more complicated since it reacts to player behavior.
#include "logic_globals.h"
#include "../engine/render_globals.h"
#include "../engine/render_math.h"
#include "../engine/chunk_globals.h"

#include "../chunk_data.h"

#include "zombie_fast_stand.h"
#include "zombie_fast_left.h"
#include "zombie_fast_right.h"
#include "zombie_slouch.h"
#include "zombie_dead.h"
#include "zombie_attack.h"

struct npc zombie_list[MAX_ZOMBIES];

void logic_zombies() {

    //destroy npcs if they are out of reach of player
    /*
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (zombie_list[i].status != -1) {

            int32_t distancex = zombie_list[i].x - camera_position_fixed_point[0];
            if (distancex < 0) {
                distancex *= -1;
            }

            int32_t distancey = zombie_list[i].y - camera_position_fixed_point[2];
            if (distancey < 0) {
                distancey *= -1;
            }
            if (distancex > ZOMBIE_DESTROY_DISTANCE && distancey > ZOMBIE_DESTROY_DISTANCE) {
                zombie_list[i].status = -1;
                break; //only destroy one npc per frame
            }
        }
    }*/

    //generate new npcs if possible
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (zombie_list[i].status == -1) {


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

        //depending on time of day, alter behavior
        int32_t daylight = (global_time % (QUARTER_DAY * 4)) / QUARTER_DAY;


        //normal walking
        if (zombie_list[i].status == 1) {




            int32_t newx = zombie_list[i].x;
            int32_t newy = zombie_list[i].y;


            //we only load npcs if they are close to the player
            int32_t distancex = zombie_list[i].x - camera_position_fixed_point[0];
            if (distancex < 0) {
                distancex *= -1;
            }

            int32_t distancey = zombie_list[i].y - camera_position_fixed_point[2];
            if (distancey < 0) {
                distancey *= -1;
            }

            //we do additional tracking of player distance if the player is in the outskirts and is not dead
            if (player_area == AREA_OUTSKIRTS && player_health > 0 && menu != MENU_START) {
                if (distancex < ZOMBIE_ATTACK_DISTANCE && distancey < ZOMBIE_ATTACK_DISTANCE) {

                    //we switch into attack mode if the zombies is close enough
                    if (distancex < distancey) {

                        if(zombie_list[i].y - camera_position_fixed_point[2] <= 0) {
                            zombie_list[i].direction = 2;
                        } else {
                            zombie_list[i].direction = 3;
                        }
                    } else {
                        if(zombie_list[i].x - camera_position_fixed_point[0] <= 0) {
                            zombie_list[i].direction = 0;
                        } else {
                            zombie_list[i].direction = 1;
                        }
                    }


                    zombie_list[i].status = 4;
                    zombie_list[i].progress = 0;
                    continue;


                //if the zombie is close enough, track the player

                } else if (daylight == 2 && distancex < ZOMBIE_TRACK_DISTANCE_NIGHT && distancey < ZOMBIE_TRACK_DISTANCE_NIGHT) {
                    //we orientate the zombie to the player using the longer distance on either axis
                    if (distancex < distancey) {
                        if(zombie_list[i].y - camera_position_fixed_point[2] <= 0) {
                            zombie_list[i].direction = 2;
                        } else {
                            zombie_list[i].direction = 3;
                        }
                    } else {
                        if(zombie_list[i].x - camera_position_fixed_point[0] <= 0) {
                            zombie_list[i].direction = 0;
                        } else {
                            zombie_list[i].direction = 1;
                        }
                    }

                } else if (daylight != 0 && distancex < ZOMBIE_TRACK_DISTANCE_DAY && distancey < ZOMBIE_TRACK_DISTANCE_DAY) {
                    //we orientate the zombie to the player using the longer distance on either axis
                    if (distancex < distancey) {
                        if(zombie_list[i].y - camera_position_fixed_point[2] <= 0) {
                            zombie_list[i].direction = 2;
                        } else {
                            zombie_list[i].direction = 3;
                        }
                    } else {
                        if(zombie_list[i].x - camera_position_fixed_point[0] <= 0) {
                            zombie_list[i].direction = 0;
                        } else {
                            zombie_list[i].direction = 1;
                        }
                    }

                } else {
                    
                    //sometimes change direction at random
                    int32_t new_direction = random(0, 250);
                    if (new_direction == 0) {
                        zombie_list[i].direction = random(0, 3);
                    }
                }
            } else {

                //sometimes change direction at random
                int32_t new_direction = random(0, 250);
                if (new_direction == 0) {
                    zombie_list[i].direction = random(0, 3);
                }
            }


            /*//sometimes change direction at random
            int32_t new_direction = random(0, 250);
            if (new_direction == 0) {
                zombie_list[i].direction = random(0, 3);
            }*/

            //increase speed at night
            if (daylight == 2) {
                if (zombie_list[i].direction == 0) {
                    newx += ZOMBIE_RUN_SPEED;
                } else if (zombie_list[i].direction == 1) {
                    newx -= ZOMBIE_RUN_SPEED;
                } else if (zombie_list[i].direction == 2) {
                    newy += ZOMBIE_RUN_SPEED;
                } else if (zombie_list[i].direction == 3) {
                    newy -= ZOMBIE_RUN_SPEED;
                }
            } else {

                if (zombie_list[i].direction == 0) {
                    newx += ZOMBIE_WALK_SPEED;
                } else if (zombie_list[i].direction == 1) {
                    newx -= ZOMBIE_WALK_SPEED;
                } else if (zombie_list[i].direction == 2) {
                    newy += ZOMBIE_WALK_SPEED;
                } else if (zombie_list[i].direction == 3) {
                    newy -= ZOMBIE_WALK_SPEED;
                }
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

            if (daylight == 2) {
                //reset walk animation
                if (zombie_list[i].progress >= ZOMBIE_RUN_TIME) {
                    zombie_list[i].progress = 0;
                } else {
                    zombie_list[i].progress++;
                }
            } else {
                //reset walk animation
                if (zombie_list[i].progress >= ZOMBIE_WALK_TIME) {
                    zombie_list[i].progress = 0;
                } else {
                    zombie_list[i].progress++;
                }
            }

        //slouched after being attacked
        } else if (zombie_list[i].status == 2) {
            
            //reset zombie to normal walking
            if (zombie_list[i].progress >= ZOMBIE_SLOUCH_DURATION) {
                zombie_list[i].progress = 0;
                zombie_list[i].status = 1;
            } else {
                zombie_list[i].progress++;
            }

        //death animation
        } else if (zombie_list[i].status == 3) {
            
            //reset zombie eventually
            if (zombie_list[i].progress >= ZOMBIE_DEATH_DURATION - 1) {

                zombie_list[i].progress = 0;
                zombie_list[i].status = 5;
            } else {
                zombie_list[i].progress++;
            }

        //dead & decomposing animation
        } else if (zombie_list[i].status == 5) {
            
            //reset zombie eventually
            if (zombie_list[i].progress >= ZOMBIE_DESPAWN_DURATION - 1) {
                
                zombie_list[i].progress = 0;
                zombie_list[i].status = -1;
            } else {
                zombie_list[i].progress++;
            }
        
        //attack animation
        } else if (zombie_list[i].status == 4) {
            
            //reset zombie eventually
            if (zombie_list[i].progress >= ZOMBIE_ATTACK_DURATION - 1) {
                zombie_list[i].progress = 0;
                zombie_list[i].status = 1;

                //if the player is close enough, remove life
                int32_t distancex = zombie_list[i].x - camera_position_fixed_point[0];
                    if (distancex < 0) {
                    distancex *= -1;
                }

                int32_t distancey = zombie_list[i].y - camera_position_fixed_point[2];
                if (distancey < 0) {
                    distancey *= -1;
                }

                if (distancex < ZOMBIE_ATTACK_DISTANCE && distancey < ZOMBIE_ATTACK_DISTANCE) {
                    player_health -= ZOMBIE_ATTACK_DAMAGE;
                }

            } else {
                zombie_list[i].progress++;
            }
        }
    }
}

void zombie_dead_pose(struct triangle_16 *npc_movement, int16_t progress, int32_t i) {

    for (int32_t j = 0; j < ZOMBIE_FAST_STAND; j++) {
        struct triangle_32 new_triangle;

        int32_t x1 = zombie_slouch[j].vertex1.x + (((npc_movement[j].vertex1.x - zombie_slouch[j].vertex1.x) / ZOMBIE_DEATH_DURATION) * progress);
        int32_t y1 = zombie_slouch[j].vertex1.y + (((npc_movement[j].vertex1.y - zombie_slouch[j].vertex1.y) / ZOMBIE_DEATH_DURATION) * progress);
        int32_t z1 = zombie_slouch[j].vertex1.z + (((npc_movement[j].vertex1.z - zombie_slouch[j].vertex1.z) / ZOMBIE_DEATH_DURATION) * progress);

        int32_t x2 = zombie_slouch[j].vertex2.x + (((npc_movement[j].vertex2.x - zombie_slouch[j].vertex2.x) / ZOMBIE_DEATH_DURATION) * progress);
        int32_t y2 = zombie_slouch[j].vertex2.y + (((npc_movement[j].vertex2.y - zombie_slouch[j].vertex2.y) / ZOMBIE_DEATH_DURATION) * progress);
        int32_t z2 = zombie_slouch[j].vertex2.z + (((npc_movement[j].vertex2.z - zombie_slouch[j].vertex2.z) / ZOMBIE_DEATH_DURATION) * progress);

        int32_t x3 = zombie_slouch[j].vertex3.x + (((npc_movement[j].vertex3.x - zombie_slouch[j].vertex3.x) / ZOMBIE_DEATH_DURATION) * progress);
        int32_t y3 = zombie_slouch[j].vertex3.y + (((npc_movement[j].vertex3.y - zombie_slouch[j].vertex3.y) / ZOMBIE_DEATH_DURATION) * progress);
        int32_t z3 = zombie_slouch[j].vertex3.z + (((npc_movement[j].vertex3.z - zombie_slouch[j].vertex3.z) / ZOMBIE_DEATH_DURATION) * progress);


        //reorient zombie depending on orientation of npc
        //going east
        if (zombie_list[i].direction == 0) {

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
        } else if (zombie_list[i].direction == 1) {

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
        } else if (zombie_list[i].direction == 3) {
            x1 *= -1;
            x2 *= -1;
            x3 *= -1;
            z1 *= -1;
            z2 *= -1;
            z3 *= -1;
        }
        //default orientation to be modelled in Blender is South



        new_triangle.vertex1.x = x1 + zombie_list[i].x;
        new_triangle.vertex1.y = y1;
        new_triangle.vertex1.z = z1 + zombie_list[i].y;
        
        new_triangle.vertex2.x = x2 + zombie_list[i].x;
        new_triangle.vertex2.y = y2;
        new_triangle.vertex2.z = z2 + zombie_list[i].y;

        new_triangle.vertex3.x = x3 + zombie_list[i].x;
        new_triangle.vertex3.y = y3;
        new_triangle.vertex3.z = z3 + zombie_list[i].y;



        new_triangle.shader_id = zombie_fast_stand[j].shader_id;

        
        if(zombie_fast_stand[j].vertex_parameter1.color == 0xf000) {

            new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
            new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
            new_triangle.vertex_parameter3.color = zombie_list[i].shirt_color;

        } else {

            
            if(zombie_fast_stand[j].vertex_parameter1.color == 0xf000) {
                new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter1.color = zombie_fast_stand[j].vertex_parameter1.color;
            }

            if(zombie_fast_stand[j].vertex_parameter2.color == 0xf000) {
                new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter2.color = zombie_fast_stand[j].vertex_parameter2.color;
            }

            if(zombie_fast_stand[j].vertex_parameter3.color == 0xf000) {
                new_triangle.vertex_parameter3.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter3.color = zombie_fast_stand[j].vertex_parameter3.color;
            }
        }


        render_triangle(new_triangle);
    }
 
    return;
}

void zombie_slouch_pose(struct triangle_16 *npc_movement, int16_t progress, int32_t i) {

    for (int32_t j = 0; j < ZOMBIE_FAST_STAND; j++) {
        struct triangle_32 new_triangle;

        int32_t x1 = zombie_fast_stand[j].vertex1.x + (((npc_movement[j].vertex1.x - zombie_fast_stand[j].vertex1.x) / ZOMBIE_SLOUCH_DURATION) * progress);
        int32_t y1 = zombie_fast_stand[j].vertex1.y + (((npc_movement[j].vertex1.y - zombie_fast_stand[j].vertex1.y) / ZOMBIE_SLOUCH_DURATION) * progress);
        int32_t z1 = zombie_fast_stand[j].vertex1.z + (((npc_movement[j].vertex1.z - zombie_fast_stand[j].vertex1.z) / ZOMBIE_SLOUCH_DURATION) * progress);

        int32_t x2 = zombie_fast_stand[j].vertex2.x + (((npc_movement[j].vertex2.x - zombie_fast_stand[j].vertex2.x) / ZOMBIE_SLOUCH_DURATION) * progress);
        int32_t y2 = zombie_fast_stand[j].vertex2.y + (((npc_movement[j].vertex2.y - zombie_fast_stand[j].vertex2.y) / ZOMBIE_SLOUCH_DURATION) * progress);
        int32_t z2 = zombie_fast_stand[j].vertex2.z + (((npc_movement[j].vertex2.z - zombie_fast_stand[j].vertex2.z) / ZOMBIE_SLOUCH_DURATION) * progress);

        int32_t x3 = zombie_fast_stand[j].vertex3.x + (((npc_movement[j].vertex3.x - zombie_fast_stand[j].vertex3.x) / ZOMBIE_SLOUCH_DURATION) * progress);
        int32_t y3 = zombie_fast_stand[j].vertex3.y + (((npc_movement[j].vertex3.y - zombie_fast_stand[j].vertex3.y) / ZOMBIE_SLOUCH_DURATION) * progress);
        int32_t z3 = zombie_fast_stand[j].vertex3.z + (((npc_movement[j].vertex3.z - zombie_fast_stand[j].vertex3.z) / ZOMBIE_SLOUCH_DURATION) * progress);


        //reorient zombie depending on orientation of npc
        //going east
        if (zombie_list[i].direction == 0) {

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
        } else if (zombie_list[i].direction == 1) {

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
        } else if (zombie_list[i].direction == 3) {
            x1 *= -1;
            x2 *= -1;
            x3 *= -1;
            z1 *= -1;
            z2 *= -1;
            z3 *= -1;
        }
        //default orientation to be modelled in Blender is South



        new_triangle.vertex1.x = x1 + zombie_list[i].x;
        new_triangle.vertex1.y = y1;
        new_triangle.vertex1.z = z1 + zombie_list[i].y;
        
        new_triangle.vertex2.x = x2 + zombie_list[i].x;
        new_triangle.vertex2.y = y2;
        new_triangle.vertex2.z = z2 + zombie_list[i].y;

        new_triangle.vertex3.x = x3 + zombie_list[i].x;
        new_triangle.vertex3.y = y3;
        new_triangle.vertex3.z = z3 + zombie_list[i].y;



        new_triangle.shader_id = zombie_fast_stand[j].shader_id;

        
        if(zombie_fast_stand[j].vertex_parameter1.color == 0xf000) {

            new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
            new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
            new_triangle.vertex_parameter3.color = zombie_list[i].shirt_color;

        } else {

            
            if(zombie_fast_stand[j].vertex_parameter1.color == 0xf000) {
                new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter1.color = zombie_fast_stand[j].vertex_parameter1.color;
            }

            if(zombie_fast_stand[j].vertex_parameter2.color == 0xf000) {
                new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter2.color = zombie_fast_stand[j].vertex_parameter2.color;
            }

            if(zombie_fast_stand[j].vertex_parameter3.color == 0xf000) {
                new_triangle.vertex_parameter3.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter3.color = zombie_fast_stand[j].vertex_parameter3.color;
            }
        }


        render_triangle(new_triangle);
    }
 
    return;
}

void zombie_attack_pose(struct triangle_16 *npc_movement, int16_t progress, int32_t i) {

    for (int32_t j = 0; j < ZOMBIE_FAST_STAND; j++) {
        struct triangle_32 new_triangle;

        int32_t x1 = zombie_slouch[j].vertex1.x + (((npc_movement[j].vertex1.x - zombie_slouch[j].vertex1.x) / ZOMBIE_ATTACK_DURATION) * progress);
        int32_t y1 = zombie_slouch[j].vertex1.y + (((npc_movement[j].vertex1.y - zombie_slouch[j].vertex1.y) / ZOMBIE_ATTACK_DURATION) * progress);
        int32_t z1 = zombie_slouch[j].vertex1.z + (((npc_movement[j].vertex1.z - zombie_slouch[j].vertex1.z) / ZOMBIE_ATTACK_DURATION) * progress);

        int32_t x2 = zombie_slouch[j].vertex2.x + (((npc_movement[j].vertex2.x - zombie_slouch[j].vertex2.x) / ZOMBIE_ATTACK_DURATION) * progress);
        int32_t y2 = zombie_slouch[j].vertex2.y + (((npc_movement[j].vertex2.y - zombie_slouch[j].vertex2.y) / ZOMBIE_ATTACK_DURATION) * progress);
        int32_t z2 = zombie_slouch[j].vertex2.z + (((npc_movement[j].vertex2.z - zombie_slouch[j].vertex2.z) / ZOMBIE_ATTACK_DURATION) * progress);

        int32_t x3 = zombie_slouch[j].vertex3.x + (((npc_movement[j].vertex3.x - zombie_slouch[j].vertex3.x) / ZOMBIE_ATTACK_DURATION) * progress);
        int32_t y3 = zombie_slouch[j].vertex3.y + (((npc_movement[j].vertex3.y - zombie_slouch[j].vertex3.y) / ZOMBIE_ATTACK_DURATION) * progress);
        int32_t z3 = zombie_slouch[j].vertex3.z + (((npc_movement[j].vertex3.z - zombie_slouch[j].vertex3.z) / ZOMBIE_ATTACK_DURATION) * progress);


        //reorient zombie depending on orientation of npc
        //going east
        if (zombie_list[i].direction == 0) {

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
        } else if (zombie_list[i].direction == 1) {

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
        } else if (zombie_list[i].direction == 3) {
            x1 *= -1;
            x2 *= -1;
            x3 *= -1;
            z1 *= -1;
            z2 *= -1;
            z3 *= -1;
        }
        //default orientation to be modelled in Blender is South



        new_triangle.vertex1.x = x1 + zombie_list[i].x;
        new_triangle.vertex1.y = y1;
        new_triangle.vertex1.z = z1 + zombie_list[i].y;
        
        new_triangle.vertex2.x = x2 + zombie_list[i].x;
        new_triangle.vertex2.y = y2;
        new_triangle.vertex2.z = z2 + zombie_list[i].y;

        new_triangle.vertex3.x = x3 + zombie_list[i].x;
        new_triangle.vertex3.y = y3;
        new_triangle.vertex3.z = z3 + zombie_list[i].y;



        new_triangle.shader_id = zombie_fast_stand[j].shader_id;

        
        if(zombie_fast_stand[j].vertex_parameter1.color == 0xf000) {

            new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
            new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
            new_triangle.vertex_parameter3.color = zombie_list[i].shirt_color;

        } else {

            
            if(zombie_fast_stand[j].vertex_parameter1.color == 0xf000) {
                new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter1.color = zombie_fast_stand[j].vertex_parameter1.color;
            }

            if(zombie_fast_stand[j].vertex_parameter2.color == 0xf000) {
                new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter2.color = zombie_fast_stand[j].vertex_parameter2.color;
            }

            if(zombie_fast_stand[j].vertex_parameter3.color == 0xf000) {
                new_triangle.vertex_parameter3.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter3.color = zombie_fast_stand[j].vertex_parameter3.color;
            }
        }


        render_triangle(new_triangle);
    }
 
    return;
}

void zombie_walk(struct triangle_16 *npc_movement, int16_t progress, int32_t i) {

    for (int32_t j = 0; j < ZOMBIE_FAST_STAND; j++) {
        struct triangle_32 new_triangle;

        int32_t x1 = zombie_fast_stand[j].vertex1.x + (((npc_movement[j].vertex1.x - zombie_fast_stand[j].vertex1.x) / (ZOMBIE_WALK_TIME / 4)) * progress);
        int32_t y1 = zombie_fast_stand[j].vertex1.y + (((npc_movement[j].vertex1.y - zombie_fast_stand[j].vertex1.y) / (ZOMBIE_WALK_TIME / 4)) * progress);
        int32_t z1 = zombie_fast_stand[j].vertex1.z + (((npc_movement[j].vertex1.z - zombie_fast_stand[j].vertex1.z) / (ZOMBIE_WALK_TIME / 4)) * progress);

        int32_t x2 = zombie_fast_stand[j].vertex2.x + (((npc_movement[j].vertex2.x - zombie_fast_stand[j].vertex2.x) / (ZOMBIE_WALK_TIME / 4)) * progress);
        int32_t y2 = zombie_fast_stand[j].vertex2.y + (((npc_movement[j].vertex2.y - zombie_fast_stand[j].vertex2.y) / (ZOMBIE_WALK_TIME / 4)) * progress);
        int32_t z2 = zombie_fast_stand[j].vertex2.z + (((npc_movement[j].vertex2.z - zombie_fast_stand[j].vertex2.z) / (ZOMBIE_WALK_TIME / 4)) * progress);

        int32_t x3 = zombie_fast_stand[j].vertex3.x + (((npc_movement[j].vertex3.x - zombie_fast_stand[j].vertex3.x) / (ZOMBIE_WALK_TIME / 4)) * progress);
        int32_t y3 = zombie_fast_stand[j].vertex3.y + (((npc_movement[j].vertex3.y - zombie_fast_stand[j].vertex3.y) / (ZOMBIE_WALK_TIME / 4)) * progress);
        int32_t z3 = zombie_fast_stand[j].vertex3.z + (((npc_movement[j].vertex3.z - zombie_fast_stand[j].vertex3.z) / (ZOMBIE_WALK_TIME / 4)) * progress);


        //reorient zombie depending on orientation of npc
        //going east
        if (zombie_list[i].direction == 0) {

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
        } else if (zombie_list[i].direction == 1) {

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
        } else if (zombie_list[i].direction == 3) {
            x1 *= -1;
            x2 *= -1;
            x3 *= -1;
            z1 *= -1;
            z2 *= -1;
            z3 *= -1;
        }
        //default orientation to be modelled in Blender is South



        new_triangle.vertex1.x = x1 + zombie_list[i].x;
        new_triangle.vertex1.y = y1;
        new_triangle.vertex1.z = z1 + zombie_list[i].y;
        
        new_triangle.vertex2.x = x2 + zombie_list[i].x;
        new_triangle.vertex2.y = y2;
        new_triangle.vertex2.z = z2 + zombie_list[i].y;

        new_triangle.vertex3.x = x3 + zombie_list[i].x;
        new_triangle.vertex3.y = y3;
        new_triangle.vertex3.z = z3 + zombie_list[i].y;



        new_triangle.shader_id = zombie_fast_stand[j].shader_id;

        
        if(zombie_fast_stand[j].vertex_parameter1.color == 0xf000) {

            new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
            new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
            new_triangle.vertex_parameter3.color = zombie_list[i].shirt_color;

        } else {

            
            if(zombie_fast_stand[j].vertex_parameter1.color == 0xf000) {
                new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter1.color = zombie_fast_stand[j].vertex_parameter1.color;
            }

            if(zombie_fast_stand[j].vertex_parameter2.color == 0xf000) {
                new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter2.color = zombie_fast_stand[j].vertex_parameter2.color;
            }

            if(zombie_fast_stand[j].vertex_parameter3.color == 0xf000) {
                new_triangle.vertex_parameter3.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter3.color = zombie_fast_stand[j].vertex_parameter3.color;
            }
        }


        render_triangle(new_triangle);
    }
 
    return;
}

void zombie_decompose(int16_t progress, int32_t i) {

    for (int32_t j = 0; j < ZOMBIE_FAST_STAND; j++) {
        struct triangle_32 new_triangle;

        int32_t x1 = zombie_dead[j].vertex1.x;
        int32_t y1 = zombie_dead[j].vertex1.y - progress;
        int32_t z1 = zombie_dead[j].vertex1.z;

        int32_t x2 = zombie_dead[j].vertex2.x;
        int32_t y2 = zombie_dead[j].vertex2.y - progress;
        int32_t z2 = zombie_dead[j].vertex2.z;

        int32_t x3 = zombie_dead[j].vertex3.x;
        int32_t y3 = zombie_dead[j].vertex3.y - progress;
        int32_t z3 = zombie_dead[j].vertex3.z;


        //reorient zombie depending on orientation of npc
        //going east
        if (zombie_list[i].direction == 0) {

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
        } else if (zombie_list[i].direction == 1) {

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
        } else if (zombie_list[i].direction == 3) {
            x1 *= -1;
            x2 *= -1;
            x3 *= -1;
            z1 *= -1;
            z2 *= -1;
            z3 *= -1;
        }
        //default orientation to be modelled in Blender is South



        new_triangle.vertex1.x = x1 + zombie_list[i].x;
        new_triangle.vertex1.y = y1;
        new_triangle.vertex1.z = z1 + zombie_list[i].y;
        
        new_triangle.vertex2.x = x2 + zombie_list[i].x;
        new_triangle.vertex2.y = y2;
        new_triangle.vertex2.z = z2 + zombie_list[i].y;

        new_triangle.vertex3.x = x3 + zombie_list[i].x;
        new_triangle.vertex3.y = y3;
        new_triangle.vertex3.z = z3 + zombie_list[i].y;



        new_triangle.shader_id = zombie_fast_stand[j].shader_id;

        
        if(zombie_fast_stand[j].vertex_parameter1.color == 0xf000) {

            new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
            new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
            new_triangle.vertex_parameter3.color = zombie_list[i].shirt_color;

        } else {

            
            if(zombie_fast_stand[j].vertex_parameter1.color == 0xf000) {
                new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter1.color = zombie_fast_stand[j].vertex_parameter1.color;
            }

            if(zombie_fast_stand[j].vertex_parameter2.color == 0xf000) {
                new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter2.color = zombie_fast_stand[j].vertex_parameter2.color;
            }

            if(zombie_fast_stand[j].vertex_parameter3.color == 0xf000) {
                new_triangle.vertex_parameter3.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter3.color = zombie_fast_stand[j].vertex_parameter3.color;
            }
        }


        render_triangle(new_triangle);
    }
 
    return;
}

void zombie_run(struct triangle_16 *npc_movement, int16_t progress, int32_t i) {

    for (int32_t j = 0; j < ZOMBIE_FAST_STAND; j++) {
        struct triangle_32 new_triangle;

        int32_t x1 = zombie_fast_stand[j].vertex1.x + (((npc_movement[j].vertex1.x - zombie_fast_stand[j].vertex1.x) / (ZOMBIE_RUN_TIME / 4)) * progress);
        int32_t y1 = zombie_fast_stand[j].vertex1.y + (((npc_movement[j].vertex1.y - zombie_fast_stand[j].vertex1.y) / (ZOMBIE_RUN_TIME / 4)) * progress);
        int32_t z1 = zombie_fast_stand[j].vertex1.z + (((npc_movement[j].vertex1.z - zombie_fast_stand[j].vertex1.z) / (ZOMBIE_RUN_TIME / 4)) * progress);

        int32_t x2 = zombie_fast_stand[j].vertex2.x + (((npc_movement[j].vertex2.x - zombie_fast_stand[j].vertex2.x) / (ZOMBIE_RUN_TIME / 4)) * progress);
        int32_t y2 = zombie_fast_stand[j].vertex2.y + (((npc_movement[j].vertex2.y - zombie_fast_stand[j].vertex2.y) / (ZOMBIE_RUN_TIME / 4)) * progress);
        int32_t z2 = zombie_fast_stand[j].vertex2.z + (((npc_movement[j].vertex2.z - zombie_fast_stand[j].vertex2.z) / (ZOMBIE_RUN_TIME / 4)) * progress);

        int32_t x3 = zombie_fast_stand[j].vertex3.x + (((npc_movement[j].vertex3.x - zombie_fast_stand[j].vertex3.x) / (ZOMBIE_RUN_TIME / 4)) * progress);
        int32_t y3 = zombie_fast_stand[j].vertex3.y + (((npc_movement[j].vertex3.y - zombie_fast_stand[j].vertex3.y) / (ZOMBIE_RUN_TIME / 4)) * progress);
        int32_t z3 = zombie_fast_stand[j].vertex3.z + (((npc_movement[j].vertex3.z - zombie_fast_stand[j].vertex3.z) / (ZOMBIE_RUN_TIME / 4)) * progress);


        //reorient zombie depending on orientation of npc
        //going east
        if (zombie_list[i].direction == 0) {

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
        } else if (zombie_list[i].direction == 1) {

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
        } else if (zombie_list[i].direction == 3) {
            x1 *= -1;
            x2 *= -1;
            x3 *= -1;
            z1 *= -1;
            z2 *= -1;
            z3 *= -1;
        }
        //default orientation to be modelled in Blender is South



        new_triangle.vertex1.x = x1 + zombie_list[i].x;
        new_triangle.vertex1.y = y1;
        new_triangle.vertex1.z = z1 + zombie_list[i].y;
        
        new_triangle.vertex2.x = x2 + zombie_list[i].x;
        new_triangle.vertex2.y = y2;
        new_triangle.vertex2.z = z2 + zombie_list[i].y;

        new_triangle.vertex3.x = x3 + zombie_list[i].x;
        new_triangle.vertex3.y = y3;
        new_triangle.vertex3.z = z3 + zombie_list[i].y;



        new_triangle.shader_id = zombie_fast_stand[j].shader_id;

        
        if(zombie_fast_stand[j].vertex_parameter1.color == 0xf000) {

            new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
            new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
            new_triangle.vertex_parameter3.color = zombie_list[i].shirt_color;

        } else {

            
            if(zombie_fast_stand[j].vertex_parameter1.color == 0xf000) {
                new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter1.color = zombie_fast_stand[j].vertex_parameter1.color;
            }

            if(zombie_fast_stand[j].vertex_parameter2.color == 0xf000) {
                new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter2.color = zombie_fast_stand[j].vertex_parameter2.color;
            }

            if(zombie_fast_stand[j].vertex_parameter3.color == 0xf000) {
                new_triangle.vertex_parameter3.color = zombie_list[i].shirt_color;
            } else {
                new_triangle.vertex_parameter3.color = zombie_fast_stand[j].vertex_parameter3.color;
            }
        }


        render_triangle(new_triangle);
    }
 
    return;
}


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
            if (render_view_frustum_culling(zombie_list[i].x, FIXED_POINT_FACTOR, zombie_list[i].y, FIXED_POINT_FACTOR / 2, FIXED_POINT_FACTOR, FIXED_POINT_FACTOR / 2) == 0) {
                continue;
            }

            if (distancex < ZOMBIE_VIEW_DISTANCE && distancey < ZOMBIE_VIEW_DISTANCE) {

                //depending on the status, we have to perform blending
                if (zombie_list[i].status == 1) {

                    int16_t walk_stage;
                    int16_t progress;

                    if (daylight == 2) {
                        walk_stage = zombie_list[i].progress / (ZOMBIE_RUN_TIME / 4);
                        progress = zombie_list[i].progress % (ZOMBIE_RUN_TIME / 4);

                    } else {
                        walk_stage = zombie_list[i].progress / (ZOMBIE_WALK_TIME / 4);
                        progress = zombie_list[i].progress % (ZOMBIE_WALK_TIME / 4);
                    }

                    struct triangle_16 *npc_movement;
                    //walk left forward
                    if (walk_stage == 0) {
                        npc_movement = zombie_fast_left;


                        if (daylight == 2) {
                            zombie_run(npc_movement, progress, i);
                        } else {
                            zombie_walk(npc_movement, progress, i);
                        }

                    
                    //walk left back
                    } else if (walk_stage == 1) {
                        npc_movement = zombie_fast_left;
                        if (daylight == 2) {
                            progress = (ZOMBIE_RUN_TIME / 4) - progress;
                            zombie_run(npc_movement, progress, i);
                        } else {
                            progress = (NPC_WALK_TIME / 4) - progress;
                            zombie_walk(npc_movement, progress, i);
                        }
                    
                    //walk right forward
                    } else if (walk_stage == 2) {
                        npc_movement = zombie_fast_right;
                        if (daylight == 2) {
                            zombie_run(npc_movement, progress, i);
                        } else {
                            zombie_walk(npc_movement, progress, i);
                        }

                    //walk right back
                    } else if (walk_stage == 3) {
                        npc_movement = zombie_fast_right;
                        if (daylight == 2) {
                            progress = (ZOMBIE_RUN_TIME / 4) - progress;
                            zombie_run(npc_movement, progress, i);
                        } else {
                            progress = (ZOMBIE_WALK_TIME / 4) - progress;
                            zombie_walk(npc_movement, progress, i);
                        }

                    } else {

                        for (int j = 0; j < ZOMBIE_FAST_STAND; j++) {

                            struct triangle_32 new_triangle;

                            new_triangle.vertex1.x = zombie_fast_stand[j].vertex1.x + zombie_list[i].x;
                            new_triangle.vertex1.y = zombie_fast_stand[j].vertex1.y;
                            new_triangle.vertex1.z = zombie_fast_stand[j].vertex1.z + zombie_list[i].y;
                            
                            new_triangle.vertex2.x = zombie_fast_stand[j].vertex2.x + zombie_list[i].x;
                            new_triangle.vertex2.y = zombie_fast_stand[j].vertex2.y;
                            new_triangle.vertex2.z = zombie_fast_stand[j].vertex2.z + zombie_list[i].y;

                            new_triangle.vertex3.x = zombie_fast_stand[j].vertex3.x + zombie_list[i].x;
                            new_triangle.vertex3.y = zombie_fast_stand[j].vertex3.y;
                            new_triangle.vertex3.z = zombie_fast_stand[j].vertex3.z + zombie_list[i].y;

                            new_triangle.shader_id = zombie_fast_stand[j].shader_id;
                            
                            if(zombie_fast_stand[j].vertex_parameter1.color == 0xf000) {
                                new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
                            } else {
                                new_triangle.vertex_parameter1.color = zombie_fast_stand[j].vertex_parameter1.color;
                            }

                            if(zombie_fast_stand[j].vertex_parameter2.color == 0xf000) {
                                new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
                            } else {
                                new_triangle.vertex_parameter2.color = zombie_fast_stand[j].vertex_parameter2.color;
                            }

                            if(zombie_fast_stand[j].vertex_parameter3.color == 0xf000) {
                                new_triangle.vertex_parameter3.color = zombie_list[i].shirt_color;
                            } else {
                                new_triangle.vertex_parameter3.color = zombie_fast_stand[j].vertex_parameter3.color;
                            }


                            render_triangle(new_triangle);
                            
                        }
                    }

                //slouching zombie
                } else if (zombie_list[i].status == 2) {
                
                    int16_t progress = zombie_list[i].progress % ZOMBIE_SLOUCH_DURATION;
                    
                    progress = ZOMBIE_SLOUCH_DURATION - progress;

                    struct triangle_16 *npc_movement;

                    npc_movement = zombie_slouch;

                    zombie_slouch_pose(npc_movement, progress, i);
                

                //dying zombie
                } else if (zombie_list[i].status == 3) {
                
                    int16_t progress = zombie_list[i].progress % ZOMBIE_DEATH_DURATION;
                    

                    struct triangle_16 *npc_movement;

                    npc_movement = zombie_dead;

                    zombie_dead_pose(npc_movement, progress, i);
                
                //decomposing zombie
                } else if (zombie_list[i].status == 5) {
                
                    int16_t progress = zombie_list[i].progress % ZOMBIE_DESPAWN_DURATION;
                    

                    struct triangle_16 *npc_movement;


                    zombie_decompose(progress, i);
                


                //attacking zombie
                } else if (zombie_list[i].status == 4) {
                
                    int16_t progress = zombie_list[i].progress % ZOMBIE_ATTACK_DURATION;
                    
                    progress = ZOMBIE_ATTACK_DURATION - progress;

                    struct triangle_16 *npc_movement;

                    npc_movement = zombie_attack;

                    zombie_attack_pose(npc_movement, progress, i);
                

                } else {
                
                    for (int j = 0; j < ZOMBIE_FAST_STAND; j++) {

                        struct triangle_32 new_triangle;

                        new_triangle.vertex1.x = zombie_fast_stand[j].vertex1.x + zombie_list[i].x;
                        new_triangle.vertex1.y = zombie_fast_stand[j].vertex1.y;
                        new_triangle.vertex1.z = zombie_fast_stand[j].vertex1.z + zombie_list[i].y;
                        
                        new_triangle.vertex2.x = zombie_fast_stand[j].vertex2.x + zombie_list[i].x;
                        new_triangle.vertex2.y = zombie_fast_stand[j].vertex2.y;
                        new_triangle.vertex2.z = zombie_fast_stand[j].vertex2.z + zombie_list[i].y;

                        new_triangle.vertex3.x = zombie_fast_stand[j].vertex3.x + zombie_list[i].x;
                        new_triangle.vertex3.y = zombie_fast_stand[j].vertex3.y;
                        new_triangle.vertex3.z = zombie_fast_stand[j].vertex3.z + zombie_list[i].y;

                        new_triangle.shader_id = zombie_fast_stand[j].shader_id;

                            if(zombie_fast_stand[j].vertex_parameter1.color == 0xf000) {
                                new_triangle.vertex_parameter1.color = zombie_list[i].shirt_color;
                            } else {
                                new_triangle.vertex_parameter1.color = zombie_fast_stand[j].vertex_parameter1.color;
                            }

                            if(zombie_fast_stand[j].vertex_parameter2.color == 0xf000) {
                                new_triangle.vertex_parameter2.color = zombie_list[i].shirt_color;
                            } else {
                                new_triangle.vertex_parameter2.color = zombie_fast_stand[j].vertex_parameter2.color;
                            }

                            if(zombie_fast_stand[j].vertex_parameter3.color == 0xf000) {
                                new_triangle.vertex_parameter3.color = zombie_list[i].shirt_color;
                            } else {
                                new_triangle.vertex_parameter3.color = zombie_fast_stand[j].vertex_parameter3.color;
                            }

                        render_triangle(new_triangle);
                        


                    }
                }

                if (number_triangles > ZOMBIE_TRIANGLE_BUDGET) {
                    break;
                }

            }
        }
    }
    return;
}