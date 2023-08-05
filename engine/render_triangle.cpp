//function to transform incoming triangle, perform clipping & lighting and put it in the triangle list if visible
//the triangle will then be rasterized by render_rasterize() on Core1 on the next frame
#include "render_globals.h"
#include "render_math.h"
#include "chunk_globals.h"

uint32_t number_triangles = 0;

#ifdef DEBUG_INFO
uint32_t rendered_triangles = 0;
#endif

static struct triangle_16 triangle_list1[MAX_RENDER_TRIANGLES]; //alternate between filling triangle lists on core 0
static struct triangle_16 triangle_list2[MAX_RENDER_TRIANGLES]; //and rendering their contents on core 1
struct triangle_16 *triangle_list_current = triangle_list1; // current triangle list for core 0
struct triangle_16 *triangle_list_next = triangle_list2; // next triangle list for core 1

#define CODE_INSIDE 0
#define CODE_LEFT 1
#define CODE_RIGHT 2
#define CODE_BOTTOM 4
#define CODE_TOP 8

uint8_t compute_code (int32_t x, int32_t y) {
    uint8_t code = CODE_INSIDE;

    if (x < 0) {
        code |= CODE_LEFT;
    } else if (x > SCREEN_WIDTH) {
        code |= CODE_RIGHT;
    } else if (y < 0) {
        code |= CODE_BOTTOM;
    } else if (y > SCREEN_HEIGHT) {
        code |= CODE_TOP;
    }

    return code;
}


void render_triangle(struct triangle_32 &in) {

    //we have to end things early if too many triangles have already been added
    if (number_triangles >= MAX_RENDER_TRIANGLES) {
        return;
    }

    //if we are dealing with large worlds, reset position close to origin
    #ifndef NO_GLOBAL_OFFSET
    in.vertex1.x -= global_offset_x * CHUNK_SIZE;
    in.vertex2.x -= global_offset_x * CHUNK_SIZE;
    in.vertex3.x -= global_offset_x * CHUNK_SIZE;

    in.vertex1.z -= global_offset_z * CHUNK_SIZE;
    in.vertex2.z -= global_offset_z * CHUNK_SIZE;
    in.vertex3.z -= global_offset_z * CHUNK_SIZE;
    #endif

    int32_t z_near = ZNEAR * FIXED_POINT_FACTOR * FIXED_POINT_FACTOR;

    struct triangle_32 output_triangle;
    struct triangle_32 extra_triangle; //if the near plane clipper produces an additional tri

    int8_t render_extra_triangle = 0;

    int32_t w1 = ((mat_vp[3][0] * in.vertex1.x) + (mat_vp[3][1] * in.vertex1.y) + (mat_vp[3][2] * in.vertex1.z) + (mat_vp[3][3] * FIXED_POINT_FACTOR)) / FIXED_POINT_FACTOR;
    int32_t w2 = ((mat_vp[3][0] * in.vertex2.x) + (mat_vp[3][1] * in.vertex2.y) + (mat_vp[3][2] * in.vertex2.z) + (mat_vp[3][3] * FIXED_POINT_FACTOR)) / FIXED_POINT_FACTOR;
    int32_t w3 = ((mat_vp[3][0] * in.vertex3.x) + (mat_vp[3][1] * in.vertex3.y) + (mat_vp[3][2] * in.vertex3.z) + (mat_vp[3][3] * FIXED_POINT_FACTOR)) / FIXED_POINT_FACTOR;

    //check z values
    int32_t check_z1 = (((mat_vp[2][0] * in.vertex1.x) + (mat_vp[2][1] * in.vertex1.y) + (mat_vp[2][2] * in.vertex1.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR)));
    int32_t check_z2 = (((mat_vp[2][0] * in.vertex2.x) + (mat_vp[2][1] * in.vertex2.y) + (mat_vp[2][2] * in.vertex2.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR)));
    int32_t check_z3 = (((mat_vp[2][0] * in.vertex3.x) + (mat_vp[2][1] * in.vertex3.y) + (mat_vp[2][2] * in.vertex3.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR)));

    //if any z values are beyond 0, we have a triangle jutting behind the near plane
    int32_t vert_behind_camera = 0;
    if (check_z1 < z_near) {
        vert_behind_camera++;
    }
    if (check_z2 < z_near) {
        vert_behind_camera++;
    }
    if (check_z3 < z_near) {
        vert_behind_camera++;
    }


    //if a triangle is clipped, we will need to rearrange the parameters
    uint8_t rearrange_parameters;

    //we now have to make a decision on what to do with the geometry if any of it is behind the camera
    //if no geometry behind the camera, the triangle does not have to be clipped, perform normal projection matrix multiply
    if (vert_behind_camera == 0) {
        
        output_triangle.vertex1.x = (((mat_vp[0][0] * in.vertex1.x) + (mat_vp[0][1] * in.vertex1.y) + (mat_vp[0][2] * in.vertex1.z) + (mat_vp[0][3] * FIXED_POINT_FACTOR))) / w1;
        output_triangle.vertex1.y = (((mat_vp[1][0] * in.vertex1.x) + (mat_vp[1][1] * in.vertex1.y) + (mat_vp[1][2] * in.vertex1.z) + (mat_vp[1][3] * FIXED_POINT_FACTOR))) / w1;
        output_triangle.vertex1.z = (((mat_vp[2][0] * in.vertex1.x) + (mat_vp[2][1] * in.vertex1.y) + (mat_vp[2][2] * in.vertex1.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR))) / w1;

        output_triangle.vertex2.x = (((mat_vp[0][0] * in.vertex2.x) + (mat_vp[0][1] * in.vertex2.y) + (mat_vp[0][2] * in.vertex2.z) + (mat_vp[0][3] * FIXED_POINT_FACTOR))) / w2;
        output_triangle.vertex2.y = (((mat_vp[1][0] * in.vertex2.x) + (mat_vp[1][1] * in.vertex2.y) + (mat_vp[1][2] * in.vertex2.z) + (mat_vp[1][3] * FIXED_POINT_FACTOR))) / w2;
        output_triangle.vertex2.z = (((mat_vp[2][0] * in.vertex2.x) + (mat_vp[2][1] * in.vertex2.y) + (mat_vp[2][2] * in.vertex2.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR))) / w2;

        output_triangle.vertex3.x = (((mat_vp[0][0] * in.vertex3.x) + (mat_vp[0][1] * in.vertex3.y) + (mat_vp[0][2] * in.vertex3.z) + (mat_vp[0][3] * FIXED_POINT_FACTOR))) / w3;
        output_triangle.vertex3.y = (((mat_vp[1][0] * in.vertex3.x) + (mat_vp[1][1] * in.vertex3.y) + (mat_vp[1][2] * in.vertex3.z) + (mat_vp[1][3] * FIXED_POINT_FACTOR))) / w3;
        output_triangle.vertex3.z = (((mat_vp[2][0] * in.vertex3.x) + (mat_vp[2][1] * in.vertex3.y) + (mat_vp[2][2] * in.vertex3.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR))) / w3;

        rearrange_parameters = 0;
        

    //if only a single vertex is behind the camera, the triangle needs to be split in two and an additional triangle needs to be rendered
    } else if (vert_behind_camera == 1) {
        

        //if we know we have only one vertex behind the camera, we have to create two triangles after clipping
        //check which vertex is behind the near plane
        if (check_z1 < z_near) {
            clip_extra_triangle(1, mat_vp, in, output_triangle, extra_triangle, w1, w2, w3);
            rearrange_parameters = 0;

        } else if (check_z2 < z_near) {
            clip_extra_triangle(2, mat_vp, in, output_triangle, extra_triangle, w2, w3, w1);
            rearrange_parameters = 1;

        } else if (check_z3 < z_near) {
            clip_extra_triangle(3, mat_vp, in, output_triangle, extra_triangle, w3, w1, w2);
            rearrange_parameters = 2;
        }

        render_extra_triangle = 1;

    //if two vertex points are behind the camera, the triangle can simply be shortened using the near plane
    } else if (vert_behind_camera == 2) {

        if (check_z1 > z_near) {
            clip_single_triangle(1, mat_vp, in, output_triangle, w2, w3, w1);
            rearrange_parameters = 1;

        } else if (check_z2 > z_near) {
            clip_single_triangle(2, mat_vp, in, output_triangle, w3, w1, w2);
            rearrange_parameters = 2;

        } else if (check_z3 > z_near) {
            clip_single_triangle(3, mat_vp, in, output_triangle, w1, w2, w3);
            rearrange_parameters = 0;
        }

    //discard the triangle otherwise
    } else {
        return;
    }


    //we may have to render an additional triangle as a result from the near clipping plane
    //this label gets called at the end if such a triangle exists
    render_extra_triangle:



    //early out if a tri beyond the view point frustum, culling in Z direction
    if (output_triangle.vertex1.z > FIXED_POINT_FACTOR || output_triangle.vertex2.z > FIXED_POINT_FACTOR || output_triangle.vertex3.z > FIXED_POINT_FACTOR
        || output_triangle.vertex1.z <= 0 || output_triangle.vertex2.z <= 0 || output_triangle.vertex3.z <= 0) {
        return;
    }

    int32_t x1;
    int32_t y1;
    x1 = (output_triangle.vertex1.x + FIXED_POINT_FACTOR) * (SCREEN_WIDTH - 1) / FIXED_POINT_FACTOR / 2;
    y1 = SCREEN_HEIGHT - ((output_triangle.vertex1.y + FIXED_POINT_FACTOR) * (SCREEN_HEIGHT - 1)) / FIXED_POINT_FACTOR / 2;

    int32_t x2;
    int32_t y2;
    x2 = (output_triangle.vertex2.x + FIXED_POINT_FACTOR) * (SCREEN_WIDTH - 1) / FIXED_POINT_FACTOR / 2;
    y2 = SCREEN_HEIGHT - ((output_triangle.vertex2.y + FIXED_POINT_FACTOR) * (SCREEN_HEIGHT - 1)) / FIXED_POINT_FACTOR / 2;

    int32_t x3;
    int32_t y3;
    x3 = (output_triangle.vertex3.x + FIXED_POINT_FACTOR) * (SCREEN_WIDTH - 1) / FIXED_POINT_FACTOR / 2;
    y3 = SCREEN_HEIGHT - ((output_triangle.vertex3.y + FIXED_POINT_FACTOR) * (SCREEN_HEIGHT - 1)) / FIXED_POINT_FACTOR / 2;


    //First perform any culling to discard triangles

    //Cohen Sutherland (Only perform trivial rejects for simplicity since we skip out on screen clipping)
    uint8_t code1 = compute_code(x1, y1);
    uint8_t code2 = compute_code(x2, y2);
    uint8_t code3 = compute_code(x3, y3);

    if (code1 & code2 & code3) {
        return;
    }
    



    //Perform back face culling
    int32_t z1 = output_triangle.vertex1.z;
    int32_t z2 = output_triangle.vertex2.z;
    int32_t z3 = output_triangle.vertex3.z;

    struct vertex_32 v1; 
    struct vertex_32 v2; 
    v1.x = x3 - x1;
    v1.y = y3 - y1;
    v1.z = z3 - z1;

    v2.x = x2 - x1;
    v2.y = y2 - y1;
    v2.z = z2 - z1;

    //calculate Z normal
    int32_t Nz = v1.x * v2.y - v1.y * v2.x;
    
    //if triangle is pointing away, discard it
    if (Nz < 0) {
        return;
    }

    //per-vertex dynamic lighting (has to be done with original world coordinates)
    //do not recalculate if an extra triangle is added
    if (render_extra_triangle != -1) {
        render_lighting(in);
    }

    //if the shaders are meant to glow at night, reduce them to the correct shader_id and return
    if (in.shader_id > 10 && in.shader_id < 20) {
        in.shader_id -= 10;
    } 

    //if the triangle still exists at this point, we can add it to the triangle list
    triangle_list_next[number_triangles].vertex1.x = x1;
    triangle_list_next[number_triangles].vertex1.y = y1;
    triangle_list_next[number_triangles].vertex1.z = z1;
    
    triangle_list_next[number_triangles].vertex2.x = x2;
    triangle_list_next[number_triangles].vertex2.y = y2;
    triangle_list_next[number_triangles].vertex2.z = z2;

    triangle_list_next[number_triangles].vertex3.x = x3;
    triangle_list_next[number_triangles].vertex3.y = y3;
    triangle_list_next[number_triangles].vertex3.z = z3;


    triangle_list_next[number_triangles].shader_id = in.shader_id;
    triangle_list_next[number_triangles].texture_id = in.texture_id;

    //depending on if a triangle had to be clipped we need to rearrange parameters
    if (rearrange_parameters == 0) {
        triangle_list_next[number_triangles].vertex_parameter1.color = in.vertex_parameter1.color;
        triangle_list_next[number_triangles].vertex_parameter2.color = in.vertex_parameter2.color;
        triangle_list_next[number_triangles].vertex_parameter3.color = in.vertex_parameter3.color;

    } else if (rearrange_parameters == 1) {
        triangle_list_next[number_triangles].vertex_parameter1.color = in.vertex_parameter2.color;
        triangle_list_next[number_triangles].vertex_parameter2.color = in.vertex_parameter3.color;
        triangle_list_next[number_triangles].vertex_parameter3.color = in.vertex_parameter1.color;

    } else if (rearrange_parameters == 2) {
        triangle_list_next[number_triangles].vertex_parameter1.color = in.vertex_parameter3.color;
        triangle_list_next[number_triangles].vertex_parameter2.color = in.vertex_parameter1.color;
        triangle_list_next[number_triangles].vertex_parameter3.color = in.vertex_parameter2.color;

    }
    



    //if we have an extra triangle caused by near plane clipping, we skip back and add it as well
    if (render_extra_triangle == 1 && number_triangles <= MAX_RENDER_TRIANGLES - 2) {
        render_extra_triangle = -1;
        output_triangle = extra_triangle;

        //extra triangles always have a determined parameter order
        rearrange_parameters = 0;
        in.vertex_parameter1.color = triangle_list_next[number_triangles].vertex_parameter1.color;
        in.vertex_parameter2.color = triangle_list_next[number_triangles].vertex_parameter3.color;
        in.vertex_parameter3.color = triangle_list_next[number_triangles].vertex_parameter2.color;

        number_triangles++;
        goto render_extra_triangle;

    } else {
        number_triangles++;
        return;
    }
    
}