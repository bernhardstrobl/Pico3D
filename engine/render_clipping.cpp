//This file contains clipping code for the required near plane clipping
#include "render_globals.h"
#include "render_math.h"

//produces a single clipped triangle if two vertices are behind the camera
void clip_single_triangle(uint8_t vertex_in_screen, int32_t mat_vp[4][4], struct triangle_32 &input_triangle, struct triangle_32 &output_triangle, int32_t w1, int32_t w2, int32_t w3) {

    struct triangle_32 in;

    //we have to resort the incoming parameters depening on which vertex is still visible
    if (vertex_in_screen == 1) {

        in.vertex1 = input_triangle.vertex2;
        in.vertex2 = input_triangle.vertex3;
        in.vertex3 = input_triangle.vertex1;

    } else if (vertex_in_screen == 2) {

        in.vertex1 = input_triangle.vertex3;
        in.vertex2 = input_triangle.vertex1;
        in.vertex3 = input_triangle.vertex2;

    //if the vertex in screen is 3, we can continue with the correct order since that is the way we process them
    } else {

        in.vertex1 = input_triangle.vertex1;
        in.vertex2 = input_triangle.vertex2;
        in.vertex3 = input_triangle.vertex3;

    }


    struct triangle_32 clip_tri;


    clip_tri.vertex1.x = (((mat_vp[0][0] * in.vertex1.x) + (mat_vp[0][1] * in.vertex1.y) + (mat_vp[0][2] * in.vertex1.z) + (mat_vp[0][3] * FIXED_POINT_FACTOR)));
    clip_tri.vertex1.y = (((mat_vp[1][0] * in.vertex1.x) + (mat_vp[1][1] * in.vertex1.y) + (mat_vp[1][2] * in.vertex1.z) + (mat_vp[1][3] * FIXED_POINT_FACTOR)));
    clip_tri.vertex1.z = (((mat_vp[2][0] * in.vertex1.x) + (mat_vp[2][1] * in.vertex1.y) + (mat_vp[2][2] * in.vertex1.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR)));

    clip_tri.vertex2.x = (((mat_vp[0][0] * in.vertex2.x) + (mat_vp[0][1] * in.vertex2.y) + (mat_vp[0][2] * in.vertex2.z) + (mat_vp[0][3] * FIXED_POINT_FACTOR)));
    clip_tri.vertex2.y = (((mat_vp[1][0] * in.vertex2.x) + (mat_vp[1][1] * in.vertex2.y) + (mat_vp[1][2] * in.vertex2.z) + (mat_vp[1][3] * FIXED_POINT_FACTOR)));
    clip_tri.vertex2.z = (((mat_vp[2][0] * in.vertex2.x) + (mat_vp[2][1] * in.vertex2.y) + (mat_vp[2][2] * in.vertex2.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR)));

    clip_tri.vertex3.x = (((mat_vp[0][0] * in.vertex3.x) + (mat_vp[0][1] * in.vertex3.y) + (mat_vp[0][2] * in.vertex3.z) + (mat_vp[0][3] * FIXED_POINT_FACTOR)));
    clip_tri.vertex3.y = (((mat_vp[1][0] * in.vertex3.x) + (mat_vp[1][1] * in.vertex3.y) + (mat_vp[1][2] * in.vertex3.z) + (mat_vp[1][3] * FIXED_POINT_FACTOR)));
    clip_tri.vertex3.z = (((mat_vp[2][0] * in.vertex3.x) + (mat_vp[2][1] * in.vertex3.y) + (mat_vp[2][2] * in.vertex3.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR)));
    
    //near plane z distance to compare with each vertex point
    int32_t z_near = ZNEAR * FIXED_POINT_FACTOR * FIXED_POINT_FACTOR;


    //solve for t (ratio of line to keep intact to vertex)
    int32_t t1 = -(clip_tri.vertex3.z - z_near) / ((clip_tri.vertex1.z - clip_tri.vertex3.z) / FIXED_POINT_FACTOR);

    //calculate corrected perspective division
    int32_t w1new = w3 + ((t1 * (w1 - w3)) / FIXED_POINT_FACTOR);

    int32_t t2 = -(clip_tri.vertex3.z - z_near) / ((clip_tri.vertex2.z - clip_tri.vertex3.z) / FIXED_POINT_FACTOR);

    int32_t w2new = w3 + ((t2 * (w2 - w3)) / FIXED_POINT_FACTOR);

    output_triangle.vertex1.x = (clip_tri.vertex3.x + (t1 * ((clip_tri.vertex1.x - clip_tri.vertex3.x) / FIXED_POINT_FACTOR))) / w1new;
    output_triangle.vertex1.y = (clip_tri.vertex3.y + (t1 * ((clip_tri.vertex1.y - clip_tri.vertex3.y) / FIXED_POINT_FACTOR))) / w1new;
    output_triangle.vertex1.z = (clip_tri.vertex3.z + (t1 * ((clip_tri.vertex1.z - clip_tri.vertex3.z) / FIXED_POINT_FACTOR))) / w1new;

    output_triangle.vertex2.x = (clip_tri.vertex3.x + (t2 * ((clip_tri.vertex2.x - clip_tri.vertex3.x) / FIXED_POINT_FACTOR))) / w2new;
    output_triangle.vertex2.y = (clip_tri.vertex3.y + (t2 * ((clip_tri.vertex2.y - clip_tri.vertex3.y) / FIXED_POINT_FACTOR))) / w2new;
    output_triangle.vertex2.z = (clip_tri.vertex3.z + (t2 * ((clip_tri.vertex2.z - clip_tri.vertex3.z) / FIXED_POINT_FACTOR))) / w2new;

    output_triangle.vertex3.x = (((mat_vp[0][0] * in.vertex3.x) + (mat_vp[0][1] * in.vertex3.y) + (mat_vp[0][2] * in.vertex3.z) + (mat_vp[0][3] * FIXED_POINT_FACTOR))) / w3;
    output_triangle.vertex3.y = (((mat_vp[1][0] * in.vertex3.x) + (mat_vp[1][1] * in.vertex3.y) + (mat_vp[1][2] * in.vertex3.z) + (mat_vp[1][3] * FIXED_POINT_FACTOR))) / w3;
    output_triangle.vertex3.z = (((mat_vp[2][0] * in.vertex3.x) + (mat_vp[2][1] * in.vertex3.y) + (mat_vp[2][2] * in.vertex3.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR))) / w3;

}



//produces two triangles from a single clipped one for the near plane
void clip_extra_triangle(uint8_t vertex_out_screen, int32_t mat_vp[4][4], struct triangle_32 &input_triangle, struct triangle_32 &output_triangle, struct triangle_32 &extra, int32_t w1, int32_t w2, int32_t w3) {


    struct triangle_32 in;

    //we have to resort the incoming parameters depening on which vertex is still visible
    if (vertex_out_screen == 2) {

        in.vertex1 = input_triangle.vertex2;
        in.vertex2 = input_triangle.vertex3;
        in.vertex3 = input_triangle.vertex1;

    } else if (vertex_out_screen == 3) {
        in.vertex1 = input_triangle.vertex3;
        in.vertex2 = input_triangle.vertex1;
        in.vertex3 = input_triangle.vertex2;

    //if the vertex in screen is 3, we can continue with the correct order since that is the way we process them
    } else {
        in.vertex1 = input_triangle.vertex1;
        in.vertex2 = input_triangle.vertex2;
        in.vertex3 = input_triangle.vertex3;
    }


    struct triangle_32 clip_tri;

    clip_tri.vertex1.x = (((mat_vp[0][0] * in.vertex1.x) + (mat_vp[0][1] * in.vertex1.y) + (mat_vp[0][2] * in.vertex1.z) + (mat_vp[0][3] * FIXED_POINT_FACTOR)));
    clip_tri.vertex1.y = (((mat_vp[1][0] * in.vertex1.x) + (mat_vp[1][1] * in.vertex1.y) + (mat_vp[1][2] * in.vertex1.z) + (mat_vp[1][3] * FIXED_POINT_FACTOR)));
    clip_tri.vertex1.z = (((mat_vp[2][0] * in.vertex1.x) + (mat_vp[2][1] * in.vertex1.y) + (mat_vp[2][2] * in.vertex1.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR)));

    clip_tri.vertex2.x = (((mat_vp[0][0] * in.vertex2.x) + (mat_vp[0][1] * in.vertex2.y) + (mat_vp[0][2] * in.vertex2.z) + (mat_vp[0][3] * FIXED_POINT_FACTOR)));
    clip_tri.vertex2.y = (((mat_vp[1][0] * in.vertex2.x) + (mat_vp[1][1] * in.vertex2.y) + (mat_vp[1][2] * in.vertex2.z) + (mat_vp[1][3] * FIXED_POINT_FACTOR)));
    clip_tri.vertex2.z = (((mat_vp[2][0] * in.vertex2.x) + (mat_vp[2][1] * in.vertex2.y) + (mat_vp[2][2] * in.vertex2.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR)));

    clip_tri.vertex3.x = (((mat_vp[0][0] * in.vertex3.x) + (mat_vp[0][1] * in.vertex3.y) + (mat_vp[0][2] * in.vertex3.z) + (mat_vp[0][3] * FIXED_POINT_FACTOR)));
    clip_tri.vertex3.y = (((mat_vp[1][0] * in.vertex3.x) + (mat_vp[1][1] * in.vertex3.y) + (mat_vp[1][2] * in.vertex3.z) + (mat_vp[1][3] * FIXED_POINT_FACTOR)));
    clip_tri.vertex3.z = (((mat_vp[2][0] * in.vertex3.x) + (mat_vp[2][1] * in.vertex3.y) + (mat_vp[2][2] * in.vertex3.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR)));
    
    //near plane z distance to compare with each vertex point
    int32_t z_near = ZNEAR * FIXED_POINT_FACTOR * FIXED_POINT_FACTOR;


    //process first triangle
    //we basically create a shortened version of the first triangle

    //solve for t
    int32_t t1 = -(clip_tri.vertex3.z - z_near) / ((clip_tri.vertex1.z - clip_tri.vertex3.z) / FIXED_POINT_FACTOR);

    //calculate corrected perspective division
    int32_t w1old = w1;
    w1 = w3 + ((t1 * (w1 - w3)) / FIXED_POINT_FACTOR);


    output_triangle.vertex1.x = (clip_tri.vertex3.x + (t1 * ((clip_tri.vertex1.x - clip_tri.vertex3.x) / FIXED_POINT_FACTOR))) / w1;
    output_triangle.vertex1.y = (clip_tri.vertex3.y + (t1 * ((clip_tri.vertex1.y - clip_tri.vertex3.y) / FIXED_POINT_FACTOR))) / w1;
    output_triangle.vertex1.z = (clip_tri.vertex3.z + (t1 * ((clip_tri.vertex1.z - clip_tri.vertex3.z) / FIXED_POINT_FACTOR))) / w1;

    //Keep the lines between V2 and v3 for first triangle
    output_triangle.vertex2.x = (((mat_vp[0][0] * in.vertex2.x) + (mat_vp[0][1] * in.vertex2.y) + (mat_vp[0][2] * in.vertex2.z) + (mat_vp[0][3] * FIXED_POINT_FACTOR))) / w2;
    output_triangle.vertex2.y = (((mat_vp[1][0] * in.vertex2.x) + (mat_vp[1][1] * in.vertex2.y) + (mat_vp[1][2] * in.vertex2.z) + (mat_vp[1][3] * FIXED_POINT_FACTOR))) / w2;
    output_triangle.vertex2.z = (((mat_vp[2][0] * in.vertex2.x) + (mat_vp[2][1] * in.vertex2.y) + (mat_vp[2][2] * in.vertex2.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR))) / w2;

    output_triangle.vertex3.x = (((mat_vp[0][0] * in.vertex3.x) + (mat_vp[0][1] * in.vertex3.y) + (mat_vp[0][2] * in.vertex3.z) + (mat_vp[0][3] * FIXED_POINT_FACTOR))) / w3;
    output_triangle.vertex3.y = (((mat_vp[1][0] * in.vertex3.x) + (mat_vp[1][1] * in.vertex3.y) + (mat_vp[1][2] * in.vertex3.z) + (mat_vp[1][3] * FIXED_POINT_FACTOR))) / w3;
    output_triangle.vertex3.z = (((mat_vp[2][0] * in.vertex3.x) + (mat_vp[2][1] * in.vertex3.y) + (mat_vp[2][2] * in.vertex3.z) + (mat_vp[2][3] * FIXED_POINT_FACTOR))) / w3;

    
    int32_t t2 = -(clip_tri.vertex2.z - z_near) / ((clip_tri.vertex1.z - clip_tri.vertex2.z) / FIXED_POINT_FACTOR);

    w2 = w2 + ((t2 * (w1old - w2)) / FIXED_POINT_FACTOR);

    //For the second extra triangle, reuse V1 and V2 (change them to keep a counter clockwise appearance), create new V3 in V2
    extra.vertex1.x = output_triangle.vertex1.x;
    extra.vertex1.y = output_triangle.vertex1.y;
    extra.vertex1.z = output_triangle.vertex1.z;

    extra.vertex2.x = (clip_tri.vertex2.x + (t2 * ((clip_tri.vertex1.x - clip_tri.vertex2.x) / FIXED_POINT_FACTOR))) / w2;
    extra.vertex2.y = (clip_tri.vertex2.y + (t2 * ((clip_tri.vertex1.y - clip_tri.vertex2.y) / FIXED_POINT_FACTOR))) / w2;
    extra.vertex2.z = (clip_tri.vertex2.z + (t2 * ((clip_tri.vertex1.z - clip_tri.vertex2.z) / FIXED_POINT_FACTOR))) / w2;

    extra.vertex3.x = output_triangle.vertex2.x;
    extra.vertex3.y = output_triangle.vertex2.y;
    extra.vertex3.z = output_triangle.vertex2.z;

}