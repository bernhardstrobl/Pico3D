//This file holds globals chunk information including the
//data structures for the chunk cache

//World size defined by the amount of chunks in x and y directions
//This is currently handled by the Blender exporter and directly put into chunk_data.h
//#define WORLD_SIZE_X 16 //amount of chunks in x orientation
//#define WORLD_SIZE_Y 16 //amount of chunks in y orientation

#define CHUNK_SIZE (FIXED_POINT_FACTOR * 10) //10 meter size per chunk
//starting location of chunks in world coordinates
#define CHUNK_OFFSET_X (-(WORLD_SIZE_X * CHUNK_SIZE) / 2)
#define CHUNK_OFFSET_Y (-(WORLD_SIZE_Y * CHUNK_SIZE) / 2)


//chunk cache settings
#define LOD0_GRID_WIDTH 3 //inner core 3x3 chunks with highest detail (only odd numbers)
#define LOD1_GRID_WIDTH 2 //outer layer, making a shell with lower details (even numbers)
#define GRID_WIDTH ((LOD1_GRID_WIDTH * 2) + LOD0_GRID_WIDTH)//grid width of the whole chunk cache
#define TOTAL_CHUNKS (GRID_WIDTH * GRID_WIDTH)

#define MAX_CHUNK_CACHE_TRIANGLES 2000 //the amount of triangles the chunk cache can store

int32_t cached_triangles = 0; //amount of triangles currently in the cache

int32_t current_chunk_x = -1;
int32_t current_chunk_y = -1;

//a single chunk points to its own triangle lists with geometry
struct chunk{
    int8_t lod_level = -1;
    uint8_t num_triangles;
    uint8_t reserved1; //unused
    uint8_t reserved2; //unused
    const struct triangle_16 *triangles; //chunks in flash/storage need to have their pointer set to const
};


//chunk cache is one big triangle list
struct triangle_16 chunk_cache[MAX_CHUNK_CACHE_TRIANGLES];


//chunk based physics
#define CHUNK_BORDER (FIXED_POINT_FACTOR * 1)


//function definitions for common operations
uint8_t chunk_traversable(int32_t x, int32_t y, uint8_t character_type);
void chunk_locate(int32_t x, int32_t y, int32_t &chunk_x, int32_t &chunk_y);