//provides view-frustum culling functions to quickly see if an object even has to be drawn at all
#include "render_globals.h"
#include "render_math.h"
#include "chunk_globals.h"

//return the amount of points of an AABB bounding box that are still visible (size of bounding box determined by offsets)
uint32_t render_view_frustum_culling(int32_t x, int32_t y, int32_t z, int32_t x_offset, int32_t y_offset, int32_t z_offset) {
    
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

    uint32_t points = 0;

    //once all the points of the bounding box are set, transform them to view coordinates
    for (int i = 0; i < 8; i++) {
        int32_t w = ((mat_vp[3][0] * point_list[i].x) + (mat_vp[3][1] * point_list[i].y) + (mat_vp[3][2] * point_list[i].z) + (mat_vp[3][3] * FIXED_POINT_FACTOR)) / FIXED_POINT_FACTOR;

        if(!w)
            continue;

        int32_t output_z = (((mat_vp[2][0] * point_list[i].x) + (mat_vp[2][1] * point_list[i].y) + (mat_vp[2][2] * point_list[i].z) + (mat_vp[2][3] * FIXED_POINT_FACTOR))) / w;

        //discard points asap in z direction
        if (output_z > FIXED_POINT_FACTOR || output_z < 0) {
            continue;
        }

        int32_t output_x = (((mat_vp[0][0] * point_list[i].x) + (mat_vp[0][1] * point_list[i].y) + (mat_vp[0][2] * point_list[i].z) + (mat_vp[0][3] * FIXED_POINT_FACTOR))) / w;
        output_x = (output_x + FIXED_POINT_FACTOR) * (SCREEN_WIDTH - 1) / FIXED_POINT_FACTOR / 2;

        if (output_x < 0 || output_x >= SCREEN_WIDTH) {
            continue;
        }

        int32_t output_y = (((mat_vp[1][0] * point_list[i].x) + (mat_vp[1][1] * point_list[i].y) + (mat_vp[1][2] * point_list[i].z) + (mat_vp[1][3] * FIXED_POINT_FACTOR))) / w;
        output_y = SCREEN_HEIGHT - ((output_y + FIXED_POINT_FACTOR) * (SCREEN_HEIGHT - 1)) / FIXED_POINT_FACTOR / 2;


        if (output_y < 0 || output_y >= SCREEN_HEIGHT) {
            continue;
        }

        points++;

    }

    return points;
}