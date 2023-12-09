//This file contains code on handling the loading of chunks from flash into the chunk array
#include "render_globals.h"
#include "render_math.h"
#include "chunk_globals.h"

#include "../chunk_data.h" // lodN_chunks, WORLD_SIZE_

int32_t cached_triangles = 0; //amount of triangles currently in the cache

static int32_t current_chunk_x = -1;
static int32_t current_chunk_y = -1;

struct triangle_16 chunk_cache[MAX_CHUNK_CACHE_TRIANGLES];


//locate chunk based on location given in world coordinates (fixed point)
void chunk_locate(int32_t x, int32_t y, int32_t &chunk_x, int32_t &chunk_y) {
    
    if (x < CHUNK_OFFSET_Y) {
        chunk_y = -1;
    } else {
        chunk_y = ((x - CHUNK_OFFSET_Y) / CHUNK_SIZE);
        //player is outside the world
        if (chunk_y >= WORLD_SIZE_Y) {
            chunk_y = -1;
        }
    }
    
    if (y < CHUNK_OFFSET_X) {
        chunk_x = -1;
    } else {
        chunk_x = ((y - CHUNK_OFFSET_X) / CHUNK_SIZE);
        //player is outside the world
        if (chunk_x >= WORLD_SIZE_X) {
            chunk_x = -1;
        }
    }

}


void load_lod0_chunk(int32_t x, int32_t y) {



    int32_t num_triangles = lod0_chunks[x][y].num_triangles;

    //make sure the chunk fits in cache, otherwise skip
    if (cached_triangles + num_triangles >= MAX_CHUNK_CACHE_TRIANGLES)
        return;


    for (int i = 0; i < num_triangles; i++) {
        
        chunk_cache[cached_triangles].vertex1.x = lod0_chunks[x][y].triangles[i].vertex1.x;
        chunk_cache[cached_triangles].vertex1.y = lod0_chunks[x][y].triangles[i].vertex1.y;
        chunk_cache[cached_triangles].vertex1.z = lod0_chunks[x][y].triangles[i].vertex1.z;

        chunk_cache[cached_triangles].vertex2.x = lod0_chunks[x][y].triangles[i].vertex2.x;
        chunk_cache[cached_triangles].vertex2.y = lod0_chunks[x][y].triangles[i].vertex2.y;
        chunk_cache[cached_triangles].vertex2.z = lod0_chunks[x][y].triangles[i].vertex2.z;

        chunk_cache[cached_triangles].vertex3.x = lod0_chunks[x][y].triangles[i].vertex3.x;
        chunk_cache[cached_triangles].vertex3.y = lod0_chunks[x][y].triangles[i].vertex3.y;
        chunk_cache[cached_triangles].vertex3.z = lod0_chunks[x][y].triangles[i].vertex3.z;

        chunk_cache[cached_triangles].shader_id = lod0_chunks[x][y].triangles[i].shader_id;
        chunk_cache[cached_triangles].texture_id = lod0_chunks[x][y].triangles[i].texture_id;

        chunk_cache[cached_triangles].vertex_parameter1.color = lod0_chunks[x][y].triangles[i].vertex_parameter1.color;
        chunk_cache[cached_triangles].vertex_parameter2.color = lod0_chunks[x][y].triangles[i].vertex_parameter2.color;
        chunk_cache[cached_triangles].vertex_parameter3.color = lod0_chunks[x][y].triangles[i].vertex_parameter3.color;

        chunk_cache[cached_triangles].chunk_x = x; 
        chunk_cache[cached_triangles].chunk_y = y; 

        cached_triangles++;
    }

}

void load_lod1_chunk(int32_t x, int32_t y) {



    int32_t num_triangles = lod1_chunks[x][y].num_triangles;

    //make sure the chunk fits in cache, otherwise skip
    if (cached_triangles + num_triangles >= MAX_CHUNK_CACHE_TRIANGLES)
        return;


    for (int i = 0; i < num_triangles; i++) {
        
        chunk_cache[cached_triangles].vertex1.x = lod1_chunks[x][y].triangles[i].vertex1.x;
        chunk_cache[cached_triangles].vertex1.y = lod1_chunks[x][y].triangles[i].vertex1.y;
        chunk_cache[cached_triangles].vertex1.z = lod1_chunks[x][y].triangles[i].vertex1.z;

        chunk_cache[cached_triangles].vertex2.x = lod1_chunks[x][y].triangles[i].vertex2.x;
        chunk_cache[cached_triangles].vertex2.y = lod1_chunks[x][y].triangles[i].vertex2.y;
        chunk_cache[cached_triangles].vertex2.z = lod1_chunks[x][y].triangles[i].vertex2.z;

        chunk_cache[cached_triangles].vertex3.x = lod1_chunks[x][y].triangles[i].vertex3.x;
        chunk_cache[cached_triangles].vertex3.y = lod1_chunks[x][y].triangles[i].vertex3.y;
        chunk_cache[cached_triangles].vertex3.z = lod1_chunks[x][y].triangles[i].vertex3.z;

        chunk_cache[cached_triangles].shader_id = lod1_chunks[x][y].triangles[i].shader_id;
        chunk_cache[cached_triangles].texture_id = lod1_chunks[x][y].triangles[i].texture_id;

        chunk_cache[cached_triangles].vertex_parameter1.color = lod1_chunks[x][y].triangles[i].vertex_parameter1.color;
        chunk_cache[cached_triangles].vertex_parameter2.color = lod1_chunks[x][y].triangles[i].vertex_parameter2.color;
        chunk_cache[cached_triangles].vertex_parameter3.color = lod1_chunks[x][y].triangles[i].vertex_parameter3.color;

        chunk_cache[cached_triangles].chunk_x = x; 
        chunk_cache[cached_triangles].chunk_y = y; 

        cached_triangles++;
    }

}


void chunk_cache_fill() {

    //we get the array position of the center chunk where the player is
    int32_t chunk_x;
    int32_t chunk_y;

    cached_triangles = 0;


    chunk_locate(camera_position_fixed_point[0], camera_position_fixed_point[2], chunk_x, chunk_y);

    //if the player is out of bounds we are screwed (don't let the player leave the world)
    if (chunk_x == -1 || chunk_y == -1) {
        return;
    }

    //load the cache in an onion layer like fashion starting with the chunk the player is in
    load_lod0_chunk(chunk_x, chunk_y);


    int32_t x = chunk_x - (LOD0_GRID_WIDTH / 2);
    int32_t y = chunk_y - (LOD0_GRID_WIDTH / 2);


    //then loop through the remaining lod0 chunks
    for (int i = 0; i < LOD0_GRID_WIDTH; i++) {
        y = chunk_y - (LOD0_GRID_WIDTH / 2);
        for (int j = 0; j < LOD0_GRID_WIDTH; j++) {
            if (x < 0 || x >= WORLD_SIZE_X || y < 0 || y >= WORLD_SIZE_Y) {
            } else if (x == chunk_x && y == chunk_y) {
            } else {
                load_lod0_chunk(x, y);
            }
            y++;
        }
        x++;
    }

    //if we have hit the cache limit, return since we can't add any more tris
    if (cached_triangles >= MAX_CHUNK_CACHE_TRIANGLES) {
        return;
    }


    x = chunk_x - (GRID_WIDTH / 2);
    y = chunk_y - (GRID_WIDTH / 2);

    //then loop through LOD1 chunks
    for (int i = 0; i < GRID_WIDTH; i++) {
        y = chunk_y - (GRID_WIDTH / 2);
        for (int j = 0; j < GRID_WIDTH; j++) {
            if ((i < LOD1_GRID_WIDTH  || i >= LOD1_GRID_WIDTH + LOD0_GRID_WIDTH) || 
                    (j < LOD1_GRID_WIDTH  || j >= LOD1_GRID_WIDTH + LOD0_GRID_WIDTH)) {
                
                if (x < 0 || x >= WORLD_SIZE_X || y < 0 || y >= WORLD_SIZE_Y) {
                } else {
                    load_lod1_chunk(x, y);
                }
            }
            y++;
        }
        x++;
    }


}

void render_chunks() {


    //we get the array position of the center chunk where the player is
    int32_t chunk_x;
    int32_t chunk_y;

    chunk_locate(camera_position_fixed_point[0], camera_position_fixed_point[2], chunk_x, chunk_y);
    
    //refresh chunk cache if player leaves previous chunk
    if (chunk_x != current_chunk_x || chunk_y != current_chunk_y) {
        current_chunk_x = chunk_x;
        current_chunk_y = chunk_y;
        chunk_cache_fill();
    }

    //since we don't have enough time in the drawing() function, we add the triangles for the chunks to the triangle list here
    if (skip_frame == 0) {

        int16_t old_x = -1;
        int16_t old_y = -1;
        int32_t offset_x = 0;
        int32_t offset_y = 0;
        //Load scenery from chunk cache
        for (int i = 0; i < cached_triangles; i++) {

            if (old_x != chunk_cache[i].chunk_x || old_y != chunk_cache[i].chunk_y) {
                offset_x = (chunk_cache[i].chunk_x * CHUNK_SIZE) + CHUNK_OFFSET_X + (CHUNK_SIZE / 2);
                offset_y = (chunk_cache[i].chunk_y * CHUNK_SIZE) + CHUNK_OFFSET_Y + (CHUNK_SIZE / 2);
                old_x = chunk_cache[i].chunk_x;
                old_y = chunk_cache[i].chunk_y;
            }
            struct triangle_32 new_triangle;

            new_triangle.vertex1.x = chunk_cache[i].vertex1.x + offset_y;
            new_triangle.vertex1.y = chunk_cache[i].vertex1.y;
            new_triangle.vertex1.z = chunk_cache[i].vertex1.z + offset_x;

            new_triangle.vertex2.x = chunk_cache[i].vertex2.x + offset_y;
            new_triangle.vertex2.y = chunk_cache[i].vertex2.y;
            new_triangle.vertex2.z = chunk_cache[i].vertex2.z + offset_x;

            new_triangle.vertex3.x = chunk_cache[i].vertex3.x + offset_y;
            new_triangle.vertex3.y = chunk_cache[i].vertex3.y;
            new_triangle.vertex3.z = chunk_cache[i].vertex3.z + offset_x;

            new_triangle.shader_id = chunk_cache[i].shader_id;
            new_triangle.texture_id = chunk_cache[i].texture_id;

            new_triangle.vertex_parameter1.color = chunk_cache[i].vertex_parameter1.color;
            new_triangle.vertex_parameter2.color = chunk_cache[i].vertex_parameter2.color;
            new_triangle.vertex_parameter3.color = chunk_cache[i].vertex_parameter3.color;
            

            //apply vertex lighting
            //render_lighting(new_triangle, chunk_cache[i].chunk_y, chunk_cache[i].chunk_x);

            render_triangle(new_triangle);

            //quit loading more if maximum triangles have been reached
            if (number_triangles == MAX_RENDER_TRIANGLES) {
                break;
            }
        }
    }
}