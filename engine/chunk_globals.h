//This file holds globals chunk information including the
//data structures for the chunk cache

//World size defined by the amount of chunks in x and y directions
//This is currently handled by the Blender exporter and directly put into chunk_data.h
//#define WORLD_SIZE_X 12 //amount of chunks in x orientation
//#define WORLD_SIZE_Y 12 //amount of chunks in y orientation

#define CHUNK_UNITS 10 //10 meter size per chunk (max is around 64)

#define CHUNK_SIZE (FIXED_POINT_FACTOR * CHUNK_UNITS) //Total fixed point representation of one chunk
//starting location of chunks in world coordinates
#define CHUNK_OFFSET_X (-(WORLD_SIZE_X * CHUNK_SIZE) / 2)
#define CHUNK_OFFSET_Y (-(WORLD_SIZE_Y * CHUNK_SIZE) / 2)


//chunk cache settings
#define LOD0_GRID_WIDTH 3 //inner core 3x3 chunks with highest detail (only odd numbers)
#define LOD1_GRID_WIDTH 2 //outer layer, making a shell with lower details (even numbers)
#define GRID_WIDTH ((LOD1_GRID_WIDTH * 2) + LOD0_GRID_WIDTH)//grid width of the whole chunk cache
#define TOTAL_CHUNKS (GRID_WIDTH * GRID_WIDTH)

#define MAX_CHUNK_CACHE_TRIANGLES 1800 //the amount of triangles the chunk cache can store

extern int32_t cached_triangles; //amount of triangles currently in the cache

//a single chunk points to its own triangle lists with geometry
struct chunk{
    uint16_t num_triangles;
    uint16_t reserved; //unused
    const struct triangle_16 *triangles; //chunks in flash/storage need to have their pointer set to const
};


//chunk cache is one big triangle list
extern struct triangle_16 chunk_cache[MAX_CHUNK_CACHE_TRIANGLES];



//function definitions for common operations
void chunk_locate(int32_t x, int32_t y, int32_t &chunk_x, int32_t &chunk_y);

void render_chunks();