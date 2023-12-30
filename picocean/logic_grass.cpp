#include "logic_globals.h"
#include "../engine/render_globals.h"
#include "../engine/render_math.h"
#include "../engine/chunk_globals.h"

#include "grass.h"

#define GRASS_DEPTH 10
#define GRASS_DISTANCE 512 //in fixed point
#define GRASS_SWAY 512

int16_t grass_tip_offset = 0;
int16_t grass_tip_divisor = 8;
int8_t grass_tip_direction = 1;
int8_t grass_div_direction = 1;
int8_t grass_update = 0;


void logic_grass() {

    //grass tip offset is calculated here to emulate swaying motion (update every 5 frames)
    if (grass_update == 5) {

        if (grass_tip_direction == 1) {
            grass_tip_offset += (GRASS_SWAY / grass_tip_divisor);
        } else {
            grass_tip_offset -= (GRASS_SWAY / grass_tip_divisor);
        }

        if (grass_div_direction == 1) {
            grass_tip_divisor *= 2;
        } else {
            grass_tip_divisor /= 2;
        }

        if(grass_tip_divisor == GRASS_SWAY / 2) {
            grass_tip_direction *= -1;
            grass_div_direction *= -1;
        } else if (grass_tip_divisor == 8) {
            grass_div_direction *= -1;
        }

        grass_update = 0;
    } else {
        grass_update++;

    }

}





//this determines where grass is being called on
uint8_t grass_chunks[12][12] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
                                {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
                                {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
                                {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
                                {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
                                {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
                                {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
                                {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                };


//this modifies the grass color since we want different lighting dependent on time of day
color_t grass_color(color_t color) {
    if (light_falloff == 0) {
        return color;
    } else {

        uint8_t r = color & 0x000F;
        uint8_t b = (color >> 8) & 0x000F;
        uint8_t g = (color >> 12) & 0x000F;

        //reduce colors
        r-=light_falloff;
        b+=light_falloff; //gives the grass a blue sheen at night
        g-=light_falloff;

        if (r > 15)
            r = 0;

        
        if (b > 15)
            b = 0;


        if (g > 15)
            g = 0;

        color = g;
        color <<= 4;
        color |= b;
        color <<= 8;
        color |= r;

        return color;
    }
}



void render_grass() {

    //we get the array position of the center chunk where the player is
    int32_t chunk_x;
    int32_t chunk_y;

    chunk_locate(camera_position_fixed_point[0], camera_position_fixed_point[2], chunk_x, chunk_y);

    //load objects into the scene
    //Grass/foliage
    if (chunk_y >= 6) {
        int32_t start_x = (camera_position_fixed_point[0] / FIXED_POINT_FACTOR) * FIXED_POINT_FACTOR;
        int32_t start_y = (camera_position_fixed_point[2] / FIXED_POINT_FACTOR) * FIXED_POINT_FACTOR;

        start_x -= GRASS_DISTANCE;
        start_y -= GRASS_DISTANCE;

        int32_t offset_x = start_x;
        int32_t offset_y = start_y;

        
        //build up grass like an onion around the player to reduce overdraw
        int32_t end = 1;
        int32_t width = 2;

        for (int o = 0; o < GRASS_DEPTH; o++) {

            for (int i = 0; i < width; i++) {
                offset_y = start_y;
                for (int j = 0; j < width; j++) {

                    //only draw if no grass yet exists at that position
                    if ((i == 0 || i == end) || (j == 0 || j == end)) {

                        //check if a chunk even has grass on it
                        chunk_locate(offset_x, offset_y, chunk_x, chunk_y);

                        if (grass_chunks[chunk_x][chunk_y] == 1) {

                            for (int k = 0; k < GRASS_MODEL; k++) {
                                struct triangle_32 new_triangle;

                                new_triangle.vertex1.x = grass_model[k].vertex1.x + offset_x;
                                new_triangle.vertex1.y = grass_model[k].vertex1.y;
                                new_triangle.vertex1.z = grass_model[k].vertex1.z + offset_y;
                                
                                new_triangle.vertex2.x = grass_model[k].vertex2.x + offset_x + grass_tip_offset;
                                new_triangle.vertex2.y = grass_model[k].vertex2.y;
                                new_triangle.vertex2.z = grass_model[k].vertex2.z + offset_y;

                                new_triangle.vertex3.x = grass_model[k].vertex3.x + offset_x;
                                new_triangle.vertex3.y = grass_model[k].vertex3.y;
                                new_triangle.vertex3.z = grass_model[k].vertex3.z + offset_y;

                                new_triangle.shader_id = grass_model[k].shader_id;
                                new_triangle.vertex_parameter1.color = grass_color(grass_model[k].vertex_parameter1.color);
                                new_triangle.vertex_parameter2.color = grass_color(grass_model[k].vertex_parameter2.color);
                                new_triangle.vertex_parameter3.color = grass_color(grass_model[k].vertex_parameter3.color);

                                render_triangle(new_triangle);
                            }
                        }
                    }

                    offset_y += GRASS_DISTANCE;
                }
                
                offset_x += GRASS_DISTANCE;
            }

            end += 2;
            width += 2;

            start_x -= GRASS_DISTANCE;
            start_y -= GRASS_DISTANCE;

            offset_x = start_x;
            offset_y = start_y;
        }
    }

    return;


}