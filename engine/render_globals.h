#include <cmath>
#include <cstdint>

//These are rendering globals, some needed by both cpu cores and multiple functions in the rendering subsystem

using color_t = uint16_t;

#define SCREEN_WIDTH  120
#define SCREEN_HEIGHT 120
#define PI 3.141592654

//Graphics adjustments
#define MAX_RENDER_TRIANGLES 1500 //Maximum triangles that should be passed to core 1 via triangle list (2x memory requirement!)
extern uint32_t number_triangles; //number of triangles to be rendered by core 1

extern uint8_t skip_frame; //notify update() function if a frame has been skipped
extern uint8_t shader_override; // change to use debug shaders
#ifdef DEBUG_INFO
extern uint32_t rendered_triangles;
#endif

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

extern struct triangle_16 *triangle_list_current; // current triangle list for core 0
extern struct triangle_16 *triangle_list_next; // next triangle list for core 1


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
    const struct light *lights;
};


extern int8_t light_falloff; //amount to decrease vertex colors by when lit/unlit
#define MAX_FALLOFF 4
#define LIGHT_DISTANCE (FIXED_POINT_FACTOR * 50000) //distance to a light source in which vertices are still lit
extern color_t sky; //used by Core1 when clearing Framebuffer



//3d transformation matrices
//camera info
extern float camera_position[3];
extern int32_t camera_position_fixed_point[3]; //only updated with update_camera()
extern float pitch;
extern float yaw;


//offsets used to shift all meshes and camera closer to origin to prevent overflows and precision errors in large worlds
#ifndef NO_GLOBAL_OFFSET
    extern int32_t global_offset_x;
    extern int32_t global_offset_z;
#endif


//view matrix 
extern float mat_camera[4][4];

//camera matrix (local space)
extern float mat_cam_rotate[4][4];



//perspective matrix (i.e. the one we want for games)
#define CAMERA_WIDTH 1.0
#define CAMERA_HEIGHT 1.0
#define ZFAR 40
#define ZNEAR 0.25
#define CAMERA_FOVX 180.0
#define CAMERA_FOVY 180.0


//final fixed_point transform matrix for rendering (view * projection)
extern int32_t mat_vp[4][4];

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

extern uint8_t animated_texture_offset;
extern uint8_t animated_texture_counter;

void update_camera();
void move_camera(float move);
void render_view_projection();

void clip_single_triangle(uint8_t vertex_in_screen, int32_t mat_vp[4][4], struct triangle_32 &input_triangle, struct triangle_32 &output_triangle, int32_t w1, int32_t w2, int32_t w3);
void clip_extra_triangle(uint8_t vertex_out_screen, int32_t mat_vp[4][4], struct triangle_32 &input_triangle, struct triangle_32 &output_triangle, struct triangle_32 &extra, int32_t w1, int32_t w2, int32_t w3);

uint32_t render_view_frustum_culling(int32_t x, int32_t y, int32_t z, int32_t x_offset, int32_t y_offset, int32_t z_offset);

void render_lighting(struct triangle_32 &in);

void render_model_16bit(triangle_16 *model, int32_t triangle_count);
void render_model_32bit(triangle_32 *model, int32_t triangle_count);
void render_model_16bit_flash(const triangle_16 *model, int32_t triangle_count);
void render_model_32bit_flash(const triangle_32 *model, int32_t triangle_count);

void render_rasterize(uint32_t num_triangle, color_t *fb);

int32_t render_sync();

void render_triangle(struct triangle_32 &in);