//handles lighting calculations by collecting light data stored in each chunk.
//every vertex is compared to the chunk it is in and the neighbouring chunks in a 3x3 fashion
#include "render_globals.h"
#include "render_math.h"
#include "chunk_globals.h"

#include "../chunk_data.h" // chunk_lights, WORLD_SIZE_

int8_t light_falloff = 0; 

int32_t process_lighting(const struct light &light, vertex_32 &vertex, color_t &color) {
    
    if (((light.position.x - vertex.x) * (light.position.x - vertex.x)) + 
        ((light.position.y - vertex.y) * (light.position.y - vertex.y)) + 
        ((light.position.z - vertex.z) * (light.position.z - vertex.z)) < LIGHT_DISTANCE ) {
        
        uint8_t r = color & 0x000F;
        uint8_t b = (color >> 8) & 0x000F;
        uint8_t g = (color >> 12) & 0x000F;

        //reduce colors
        //r -= light_falloff;
        b -= light_falloff;
        //g -= light_falloff;

        //if (r > 15)
        //    r = 0;

        
        if (b > 15)
            b = 0;


        //if (g > 15)
        //    g = 0;

        color = g;
        color <<= 4;
        color |= b;
        color <<= 8;
        color |= r;
        return 1;

    } else {
        return 0;
    }

}

color_t darken(color_t &color) {

    uint8_t r = color & 0x000F;
    uint8_t b = (color >> 8) & 0x000F;
    uint8_t g = (color >> 12) & 0x000F;

    //reduce colors
    r -= light_falloff;
    b -= light_falloff;
    g -= light_falloff;

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

void vertex_lighting(struct vertex_32 &in, color_t &color, int16_t chunk_x, int16_t chunk_y) {

    struct light new_light;

    //first check if a vertex can be locally illuminated by a chunk light
    for (int i = 0; i < chunk_lights[chunk_x][chunk_y].number; i++) {
        if (process_lighting(chunk_lights[chunk_x][chunk_y].lights[i], in, color) == 1) {
            return;
        }
    }


    //loop through chunks on a 3x3 grid and see if a vertex can get lit by any surrounding lights
    for(int x = -1; x < 2; x++) {

        int32_t newx = chunk_x + x;

        if (newx < 0 || newx >= WORLD_SIZE_X) {
            continue;
        }

        for(int y = -1; y < 2; y++) {
            int32_t newy = chunk_y + y;

            if (newy < 0 || newy >= WORLD_SIZE_Y) {
                continue;
            }

            if (!(x == 0 && y == 0)) {
                for (int i = 0; i < chunk_lights[newx][newy].number; i++) {
                    if (process_lighting(chunk_lights[newx][newy].lights[i], in, color) == 1) {
                        return;
                    }
                }
            }
        }
    }

    //we have to assume the vertex has not found a close enough light source by now, so darken it
    color = darken(color);

    return;

}

void render_lighting(struct triangle_32 &in) {

    //only perform lighting if player is in the city and there is an actual falloff to calculate
    if (/*player_area == AREA_OUTSKIRTS || */light_falloff == 0) {
        return;
    }

    //only process shader ids that are affected by dynamic light (1 & 2 currently)
    if (in.shader_id > 10) {
        return;
    }

    int32_t chunk_x;
    int32_t chunk_y;

    //if we are dealing with large worlds, put position back to original
    #ifndef NO_GLOBAL_OFFSET
    in.vertex1.x += global_offset_x * CHUNK_SIZE;
    in.vertex2.x += global_offset_x * CHUNK_SIZE;
    in.vertex3.x += global_offset_x * CHUNK_SIZE;

    in.vertex1.z += global_offset_z * CHUNK_SIZE;
    in.vertex2.z += global_offset_z * CHUNK_SIZE;
    in.vertex3.z += global_offset_z * CHUNK_SIZE;
    #endif

    chunk_locate(in.vertex1.x, in.vertex1.z, chunk_y, chunk_x);

    //if triangle can't be assigned to a correct chunk, return
    if (chunk_x == -1 || chunk_y == -1) {
        return;
    }

    //try to illuminate each vertex in turn
    vertex_lighting(in.vertex1, in.vertex_parameter1.color, chunk_x, chunk_y);
    vertex_lighting(in.vertex2, in.vertex_parameter2.color, chunk_x, chunk_y);
    vertex_lighting(in.vertex3, in.vertex_parameter3.color, chunk_x, chunk_y);


    if (in.vertex_parameter1.color == in.vertex_parameter2.color && in.vertex_parameter2.color == in.vertex_parameter3.color) {
        in.shader_id = 1;
    } else {
        in.shader_id = 2;
    }

}