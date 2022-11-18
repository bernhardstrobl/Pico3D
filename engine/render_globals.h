#include "picosystem.hpp"
//These are rendering globals, some needed by both cpu cores and multiple functions in the rendering subsystem
using namespace picosystem;

#define SCREEN_WIDTH  120
#define SCREEN_HEIGHT 120
#define PI 3.141592654

//Graphics adjustments
#define MAX_RENDER_TRIANGLES 1500 //Maximum triangles that should be passed to core 1 via triangle list (2x memory requirement!)
uint32_t number_triangles = 0; //number of triangles to be rendered by core 1

uint8_t skip_frame = 1; //notify update() function if a frame has been skipped
uint8_t shader_override = 0; // change to use debug shaders
#ifdef DEBUG_INFO
uint32_t rendered_triangles = 0;
#endif

#ifdef FRAME_COUNTER
uint32_t perf_25_below = 0;
uint32_t perf_50_below = 0;
uint32_t perf_75_below = 0;
uint32_t perf_75_above = 0;
#endif

//Framebuffer for second core to render into
color_t framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT] __attribute__ ((aligned (4))) = { };
buffer_t *FRAMEBUFFER = buffer(SCREEN_WIDTH, SCREEN_HEIGHT, framebuffer);
color_t *fb;

int16_t zbuffer[SCREEN_WIDTH * SCREEN_HEIGHT] __attribute__ ((aligned (4))) = { };

//full 32 bit fixed point vertex
struct vertex_32 {
    int32_t x;
    int32_t y;
    int32_t z;
};

//reduced range 16 bit vertex point (using default 10 bit fraction gives 6 bits i.e. 32 meters in each direction)
struct vertex_16 {
    int16_t x;
    int16_t y;
    int16_t z;
};

struct vertex_floating_point {
    float x;
    float y;
    float z;
};


//UV maps share storage with vertex colors since only one or the other is used
union color_or_uv {
    color_t color;
    uint8_t uv[2];
};


//default triangle used when placing things into world coordinates and performing final transform to view space
struct triangle_32 {
    struct vertex_32 vertex1; //xyz coordinates for each vertex point
    struct vertex_32 vertex2;
    struct vertex_32 vertex3;
    uint8_t shader_id;
    uint8_t texture_id;
    union color_or_uv vertex_parameter1;
    union color_or_uv vertex_parameter2;
    union color_or_uv vertex_parameter3;
};

//reduced range triangle using 16 bits fixed point for vertex points (greatly reduces ram consumption)
//used for small objects not exceeding 32 meters in each direction and for final rendering after transform
//by Core1. Only uses 28 vs 44 bytes per triangle (near 40% savings)
struct triangle_16 {
    struct vertex_16 vertex1; //xyz coordinates for each vertex point
    struct vertex_16 vertex2;
    struct vertex_16 vertex3;
    uint8_t shader_id;
    uint8_t texture_id;
    union color_or_uv vertex_parameter1;
    union color_or_uv vertex_parameter2;
    union color_or_uv vertex_parameter3;
    uint8_t chunk_x; //used by the chunk cache, so that the needed 32 bit world coordinates can be calculated on the fly
    uint8_t chunk_y;
};

//floating point triangle (only use if it makes development easier or in small amounts, has low performance)
struct triangle_floating_point {
    struct vertex_floating_point vertex1; //xyz coordinates for each vertex point
    struct vertex_floating_point vertex2;
    struct vertex_floating_point vertex3;
    uint8_t shader_id;
    uint8_t texture_id;
    union color_or_uv vertex_parameter1;
    union color_or_uv vertex_parameter2;
    union color_or_uv vertex_parameter3;
};

struct triangle_16 triangle_list1[MAX_RENDER_TRIANGLES]; //alternate between filling triangle lists on core 0
struct triangle_16 triangle_list2[MAX_RENDER_TRIANGLES]; //and rendering their contents on core 1
struct triangle_16 *triangle_list_current = triangle_list1; // current triangle list for core 0
struct triangle_16 *triangle_list_next = triangle_list2; // next triangle list for core 1


//textures are stored in a list so the rasterizer can access them by ID
struct texture {
    uint8_t texture_size;
    uint8_t unused1;
    uint16_t unused2;
    const color_t *image;
};


//lighting system
//lights
struct light {
    struct vertex_32 position;
};

//lights in a chunk
struct chunk_lighting {
    int32_t number;
    struct light *lights;
};


int8_t light_falloff = 0; //amount to decrease vertex colors by when lit/unlit
#define MAX_FALLOFF 4
#define LIGHT_DISTANCE (FIXED_POINT_FACTOR * 50000) //distance to a light source in which vertices are still lit
color_t sky; //used by Core1 when clearing Framebuffer



//3d transformation matrices
//camera info
float camera_position[3] = {0.0, 0.0, 0.0};
int32_t camera_position_fixed_point[3] = {0, 0, 0}; //not always updated
float pitch = 0;
float yaw = 0;


//view matrix 
float mat_camera[4][4] = {{ 1.0, 0.0, 0.0, 0.0},
                          { 0.0, 1.0, 0.0, 0.0},
                          { 0.0, 0.0, 1.0, 0.0},
                          { 0.0, 0.0, 0.0, 1.0}};

//camera matrix (local space)
float mat_cam_rotate[4][4] = {{ 1.0, 0.0, 0.0, 0.0},
                              { 0.0, 1.0, 0.0, 0.0},
                              { 0.0, 0.0, 1.0, 0.0},
                              { 0.0, 0.0, 0.0, 1.0}};



//perspective matrix (i.e. the one we want for games)
#define CAMERA_WIDTH 1.0
#define CAMERA_HEIGHT 1.0
#define ZFAR 40
#define ZNEAR 0.25
#define CAMERA_FOVX 180.0
#define CAMERA_FOVY 180.0

//perspective projection matrix
float mat_projection[4][4] = {{ atan((CAMERA_FOVX * PI / 180) * 0.5), 0.0, 0.0, 0.0},
                              { 0.0, atan((CAMERA_FOVY * PI / 180) * 0.5), 0.0, 0.0},
                              { 0.0, 0.0, -((ZFAR + ZNEAR) / (ZFAR - ZNEAR)), -((2.0*ZFAR*ZNEAR) / (ZFAR - ZNEAR))},
                              { 0.0, 0.0, -1.0, 0.0}};


//final fixed_point transform matrix for rendering (view * projection)
int32_t mat_vp[4][4] = {{ 0, 0, 0, 0},
                        { 0, 0, 0, 0},
                        { 0, 0, 0, 0},
                        { 0, 0, 0, 0}};

/*
//Orthogonal camera (if anyone needs it)
#define CAMERA_WIDTH 2.0
#define CAMERA_HEIGHT 2.0
#define ZFAR 10.0
#define ZNEAR 0.0

//orthographic projection matrix
float mat_projection[4][4] = {{ 1 / CAMERA_WIDTH, 0.0, 0.0, 0.0},
                              { 0.0, 1 / CAMERA_HEIGHT, 0.0, 0.0},
                              { 0.0, 0.0, -(2 / (ZFAR - ZNEAR)), -((ZFAR + ZNEAR) / (ZFAR - ZNEAR))},
                              { 0.0, 0.0, 0.0, 1.0}};
*/
