#include "logic_globals.h"
#include "../engine/render_globals.h"
#include "../engine/render_math.h"
#include "../engine/chunk_globals.h"

//When shooting at something in the outskirts, see if a zombie is hit

//basically this code does similar things to the frustum culling code, but additionaly
//creates a 2D bounding box using the extremes of the transformed vertices
//the desired screen point is then checked to see if its within those bounds

int32_t logic_raycast(int32_t x, int32_t y, int32_t z, int32_t x_offset, int32_t y_offset, int32_t z_offset, int32_t screen_point_x, int32_t screen_point_y) {

    //if we are dealing with large worlds, reset position close to origin
    #ifndef NO_GLOBAL_OFFSET
    x -= global_offset_x * CHUNK_SIZE;
    z -= global_offset_z * CHUNK_SIZE;
    #endif

    //Generate the 8 points
    struct vertex_32 point_list[8];
    point_list[0].x = x + x_offset;
    point_list[0].y = y + y_offset;
    point_list[0].z = z + z_offset;

    point_list[1].x = x - x_offset;
    point_list[1].y = y + y_offset;
    point_list[1].z = z + z_offset;

    point_list[2].x = x + x_offset;
    point_list[2].y = y - y_offset;
    point_list[2].z = z + z_offset;

    point_list[3].x = x + x_offset;
    point_list[3].y = y + y_offset;
    point_list[3].z = z - z_offset;

    point_list[4].x = x - x_offset;
    point_list[4].y = y - y_offset;
    point_list[4].z = z + z_offset;

    point_list[5].x = x + x_offset;
    point_list[5].y = y - y_offset;
    point_list[5].z = z - z_offset;

    point_list[6].x = x - x_offset;
    point_list[6].y = y + y_offset;
    point_list[6].z = z - z_offset;

    point_list[7].x = x - x_offset;
    point_list[7].y = y - y_offset;
    point_list[7].z = z - z_offset;


    //points of the bounding box
    int32_t x_min = SCREEN_WIDTH;
    int32_t y_min = SCREEN_HEIGHT;
    int32_t x_max = 0;
    int32_t y_max = 0;



    //once all the points of the AABB bounding box are set, transform them to view coordinates
    for (int i = 0; i < 8; i++) {
        int32_t w = ((mat_vp[3][0] * point_list[i].x) + (mat_vp[3][1] * point_list[i].y) + (mat_vp[3][2] * point_list[i].z) + (mat_vp[3][3] * FIXED_POINT_FACTOR)) / FIXED_POINT_FACTOR;

        int32_t output_z = (((mat_vp[2][0] * point_list[i].x) + (mat_vp[2][1] * point_list[i].y) + (mat_vp[2][2] * point_list[i].z) + (mat_vp[2][3] * FIXED_POINT_FACTOR))) / w;

        //discard points asap in z direction
        if (output_z > FIXED_POINT_FACTOR || output_z < 0) {
            continue;
        }

        int32_t output_x = (((mat_vp[0][0] * point_list[i].x) + (mat_vp[0][1] * point_list[i].y) + (mat_vp[0][2] * point_list[i].z) + (mat_vp[0][3] * FIXED_POINT_FACTOR))) / w;
        output_x = (output_x + FIXED_POINT_FACTOR) * (SCREEN_WIDTH - 1) / FIXED_POINT_FACTOR / 2;

        int32_t output_y = (((mat_vp[1][0] * point_list[i].x) + (mat_vp[1][1] * point_list[i].y) + (mat_vp[1][2] * point_list[i].z) + (mat_vp[1][3] * FIXED_POINT_FACTOR))) / w;
        output_y = SCREEN_HEIGHT - ((output_y + FIXED_POINT_FACTOR) * (SCREEN_HEIGHT - 1)) / FIXED_POINT_FACTOR / 2;

        //see if the bounding box can be expanded
        if (output_x < x_min) {
            x_min = output_x;
        }

        if (output_x > x_max) {
            x_max = output_x;
        }

        if (output_y < y_min) {
            y_min = output_y;
        }

        if (output_y > y_max) {
            y_max = output_y;
        }

    }


    //evaluate if the screen point is within the bounding box
    if (screen_point_x >= x_min && screen_point_x <= x_max && screen_point_y >= y_min && screen_point_y <= y_max) {
        return 1;
    } else {
        return 0;
    }

}


void logic_shoot() {
    
    //only allow shooting if player has enough ammo
    if (player_ammo == 0) {
        return;
    }

    //reduce ammo regardless of outcome
    player_ammo--;

    for (int i = 0; i < MAX_ZOMBIES; i++) {

        //only allow killing zombies that are active (and not dying)
        if (zombie_list[i].status == -1 || zombie_list[i].status == 3 || zombie_list[i].status == 5) {
            continue;
        }

        //we only allow shooting of zombies if they are close enough to the player
        int32_t distancex = zombie_list[i].x - camera_position_fixed_point[0];
        if (distancex < 0) {
            distancex *= -1;
        }

        int32_t distancey = zombie_list[i].y - camera_position_fixed_point[2];
        if (distancey < 0) {
            distancey *= -1;
        }

        if (distancex < ZOMBIE_SHOOT_DISTANCE && distancey < ZOMBIE_SHOOT_DISTANCE) {
            if (logic_raycast(zombie_list[i].x, FIXED_POINT_FACTOR, zombie_list[i].y, FIXED_POINT_FACTOR / 2, FIXED_POINT_FACTOR, FIXED_POINT_FACTOR / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)) {
                
                //reduce zombie health
                zombie_list[i].health -= SHOTGUN_DAMAGE;

                //if a zombie is killed,
                if(zombie_list[i].health <= 0){

                    //play death animation and increase kill count
                    zombie_list[i].status = 3;
                    zombie_list[i].progress = 0;
                    player_kills++;
                    player_money += QUEST_KILL_REWARD;
                
                //otherwise play slouch animation
                } else {
                    
                    zombie_list[i].status = 2;
                    zombie_list[i].progress = 0;

                }
            }

        }
    }

}

