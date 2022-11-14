#include "picosystem.hpp"
//File with math functions mainly for rendering
//Several functions have both fixed and floating point implementations
using namespace picosystem;

//When converting to fixed points, a scaling factor power 2 allows
//the compiler to perform bit-shifts instead of more expensive divisions/multiplications
#define FIXED_POINT_FACTOR 1024





int float_to_int (float in) {
    int32_t out = in * FIXED_POINT_FACTOR;
    return out;
}

float int_to_float (int32_t in) {
    float out = in * 1.0 / FIXED_POINT_FACTOR;
    return out;
}

//4x4 floating point matrix multiply
void mat_mul(float mat1[4][4], float mat2[4][4], float out[4][4]) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++){
            out[y][x] = 0;
            for (int z = 0; z < 4; z++){
                out[y][x] += mat1[y][z] * mat2[z][x];
            }
        }
    }
}


//4x4 matrix debug output
void mat_debug(float mat[4][4], int y_offset) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++){
            text(str(mat[y][x], 2), x * 30, y * 10 + y_offset);
        }
    }
}

void mat_debug_fixed_point(int32_t mat[4][4], int y_offset) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++){
            text(str(mat[y][x], 0), x * 30, y * 10 + y_offset);
        }
    }
}


//4x4 matrix with triangle multiply
void mat_tri_mul(float mat[4][4], struct triangle_floating_point &in, struct triangle_floating_point &out) {
    
    out.vertex1.x = (mat[0][0] * in.vertex1.x) + (mat[0][1] * in.vertex1.y) + (mat[0][2] * in.vertex1.z) + mat[0][3];
    out.vertex1.y = (mat[1][0] * in.vertex1.x) + (mat[1][1] * in.vertex1.y) + (mat[1][2] * in.vertex1.z) + mat[1][3];
    out.vertex1.z = (mat[2][0] * in.vertex1.x) + (mat[2][1] * in.vertex1.y) + (mat[2][2] * in.vertex1.z) + mat[2][3];

    out.vertex2.x = (mat[0][0] * in.vertex2.x) + (mat[0][1] * in.vertex2.y) + (mat[0][2] * in.vertex2.z) + mat[0][3];
    out.vertex2.y = (mat[1][0] * in.vertex2.x) + (mat[1][1] * in.vertex2.y) + (mat[1][2] * in.vertex2.z) + mat[1][3];
    out.vertex2.z = (mat[2][0] * in.vertex2.x) + (mat[2][1] * in.vertex2.y) + (mat[2][2] * in.vertex2.z) + mat[2][3];

    out.vertex3.x = (mat[0][0] * in.vertex3.x) + (mat[0][1] * in.vertex3.y) + (mat[0][2] * in.vertex3.z) + mat[0][3];
    out.vertex3.y = (mat[1][0] * in.vertex3.x) + (mat[1][1] * in.vertex3.y) + (mat[1][2] * in.vertex3.z) + mat[1][3];
    out.vertex3.z = (mat[2][0] * in.vertex3.x) + (mat[2][1] * in.vertex3.y) + (mat[2][2] * in.vertex3.z) + mat[2][3];

}

//4x4 matrix with triangle multiply with w division (for perspective correct vertices) using fixed point inputs
void mat_tri_mul_w_fixed_point(int32_t mat[4][4], struct triangle_32 &in, struct triangle_32 &out) {
    
    int32_t w;

    w = ((mat[3][0] * in.vertex1.x) + (mat[3][1] * in.vertex1.y) + (mat[3][2] * in.vertex1.z) + (mat[3][3] * FIXED_POINT_FACTOR)) / FIXED_POINT_FACTOR;
    //if (w < 1) {
    //    w = 1;
    //}
    out.vertex1.x = (((mat[0][0] * in.vertex1.x) + (mat[0][1] * in.vertex1.y) + (mat[0][2] * in.vertex1.z) + (mat[0][3] * FIXED_POINT_FACTOR))) / w;
    out.vertex1.y = (((mat[1][0] * in.vertex1.x) + (mat[1][1] * in.vertex1.y) + (mat[1][2] * in.vertex1.z) + (mat[1][3] * FIXED_POINT_FACTOR))) / w;
    out.vertex1.z = (((mat[2][0] * in.vertex1.x) + (mat[2][1] * in.vertex1.y) + (mat[2][2] * in.vertex1.z) + (mat[2][3] * FIXED_POINT_FACTOR))) / w;


    w = ((mat[3][0] * in.vertex2.x) + (mat[3][1] * in.vertex2.y) + (mat[3][2] * in.vertex2.z) + (mat[3][3] * FIXED_POINT_FACTOR)) / FIXED_POINT_FACTOR;
    //if (w < 1) {
    //    w = 1;
    //}
    out.vertex2.x = (((mat[0][0] * in.vertex2.x) + (mat[0][1] * in.vertex2.y) + (mat[0][2] * in.vertex2.z) + (mat[0][3] * FIXED_POINT_FACTOR))) / w;
    out.vertex2.y = (((mat[1][0] * in.vertex2.x) + (mat[1][1] * in.vertex2.y) + (mat[1][2] * in.vertex2.z) + (mat[1][3] * FIXED_POINT_FACTOR))) / w;
    out.vertex2.z = (((mat[2][0] * in.vertex2.x) + (mat[2][1] * in.vertex2.y) + (mat[2][2] * in.vertex2.z) + (mat[2][3] * FIXED_POINT_FACTOR))) / w;


    w = ((mat[3][0] * in.vertex3.x) + (mat[3][1] * in.vertex3.y) + (mat[3][2] * in.vertex3.z) + (mat[3][3] * FIXED_POINT_FACTOR)) / FIXED_POINT_FACTOR;
    //if (w < 1) {
    //    w = 1;
    //}
    out.vertex3.x = (((mat[0][0] * in.vertex3.x) + (mat[0][1] * in.vertex3.y) + (mat[0][2] * in.vertex3.z) + (mat[0][3] * FIXED_POINT_FACTOR))) / w;
    out.vertex3.y = (((mat[1][0] * in.vertex3.x) + (mat[1][1] * in.vertex3.y) + (mat[1][2] * in.vertex3.z) + (mat[1][3] * FIXED_POINT_FACTOR))) / w;
    out.vertex3.z = (((mat[2][0] * in.vertex3.x) + (mat[2][1] * in.vertex3.y) + (mat[2][2] * in.vertex3.z) + (mat[2][3] * FIXED_POINT_FACTOR))) / w;
}

//4x4 matrix with triangle multiply with w division (for perspective correct vertices)
void mat_tri_mul_w(float mat[4][4], struct triangle_floating_point &in, struct triangle_floating_point &out) {
    
    float w;

    w = ((mat[3][0] * in.vertex1.x) + (mat[3][1] * in.vertex1.y) + (mat[3][2] * in.vertex1.z) + mat[3][3]);
    out.vertex1.x = ((mat[0][0] * in.vertex1.x) + (mat[0][1] * in.vertex1.y) + (mat[0][2] * in.vertex1.z) + mat[0][3]) / w;
    out.vertex1.y = ((mat[1][0] * in.vertex1.x) + (mat[1][1] * in.vertex1.y) + (mat[1][2] * in.vertex1.z) + mat[1][3]) / w;
    out.vertex1.z = ((mat[2][0] * in.vertex1.x) + (mat[2][1] * in.vertex1.y) + (mat[2][2] * in.vertex1.z) + mat[2][3]) / w;

    w = ((mat[3][0] * in.vertex2.x) + (mat[3][1] * in.vertex2.y) + (mat[3][2] * in.vertex2.z) + mat[3][3]);
    out.vertex2.x = ((mat[0][0] * in.vertex2.x) + (mat[0][1] * in.vertex2.y) + (mat[0][2] * in.vertex2.z) + mat[0][3]) / w;
    out.vertex2.y = ((mat[1][0] * in.vertex2.x) + (mat[1][1] * in.vertex2.y) + (mat[1][2] * in.vertex2.z) + mat[1][3]) / w;
    out.vertex2.z = ((mat[2][0] * in.vertex2.x) + (mat[2][1] * in.vertex2.y) + (mat[2][2] * in.vertex2.z) + mat[2][3]) / w;

    w = ((mat[3][0] * in.vertex3.x) + (mat[3][1] * in.vertex3.y) + (mat[3][2] * in.vertex3.z) + mat[3][3]);
    out.vertex3.x = ((mat[0][0] * in.vertex3.x) + (mat[0][1] * in.vertex3.y) + (mat[0][2] * in.vertex3.z) + mat[0][3]) / w;
    out.vertex3.y = ((mat[1][0] * in.vertex3.x) + (mat[1][1] * in.vertex3.y) + (mat[1][2] * in.vertex3.z) + mat[1][3]) / w;
    out.vertex3.z = ((mat[2][0] * in.vertex3.x) + (mat[2][1] * in.vertex3.y) + (mat[2][2] * in.vertex3.z) + mat[2][3]) / w;

}

//Convert 4x4 Matrix to integer
void mat_convert_float_fixed(float mat_in[4][4], int32_t mat_out[4][4]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            mat_out[i][j] = float_to_int(mat_in[i][j]);
        }
    }
}




//triangle debug output
void tri_debug(struct triangle_floating_point in, int y_offset) {

    
    text("x:" + str(in.vertex1.x, 2), 0, 0 + y_offset);
    text("y:" + str(in.vertex1.y, 2), 40, 0 + y_offset);
    text("z:" + str(in.vertex1.z, 2), 80, 0 + y_offset);

    text("x:" + str(in.vertex2.x, 2), 0, 10 + y_offset);
    text("y:" + str(in.vertex2.y, 2), 40, 10 + y_offset);
    text("z:" + str(in.vertex2.z, 2), 80, 10 + y_offset);

    text("x:" + str(in.vertex3.x, 2), 0, 20 + y_offset);
    text("y:" + str(in.vertex3.y, 2), 40, 20 + y_offset);
    text("z:" + str(in.vertex3.z, 2), 80, 20 + y_offset);


}

void tri_debug_fixed_point(struct triangle_32 in, int y_offset) {

    
    text("x" + str(in.vertex1.x), 0, 0 + y_offset);
    text("y" + str(in.vertex1.y), 40, 0 + y_offset);
    text("z" + str(in.vertex1.z), 80, 0 + y_offset);

    text("x" + str(in.vertex2.x), 0, 10 + y_offset);
    text("y" + str(in.vertex2.y), 40, 10 + y_offset);
    text("z" + str(in.vertex2.z), 80, 10 + y_offset);

    text("x" + str(in.vertex3.x), 0, 20 + y_offset);
    text("y" + str(in.vertex3.y), 40, 20 + y_offset);
    text("z" + str(in.vertex3.z), 80, 20 + y_offset);


}

//4x4 matrix with triangle multiply with w division (for perspective correct vertices) using fixed point inputs
void mat_tri_mul_w_fixed_point_debug(int32_t mat[4][4], struct triangle_32 &in, struct triangle_32 &out) {
    
    int32_t w;

    w = ((mat[3][0] * in.vertex1.x) + (mat[3][1] * in.vertex1.y) + (mat[3][2] * in.vertex1.z) + (mat[3][3] * FIXED_POINT_FACTOR)) / FIXED_POINT_FACTOR;
    if (w < 1) {
        w = 1;
    }
    out.vertex1.x = (((mat[0][0] * in.vertex1.x) + (mat[0][1] * in.vertex1.y) + (mat[0][2] * in.vertex1.z) + (mat[0][3] * FIXED_POINT_FACTOR))) / w;
    out.vertex1.y = (((mat[1][0] * in.vertex1.x) + (mat[1][1] * in.vertex1.y) + (mat[1][2] * in.vertex1.z) + (mat[1][3] * FIXED_POINT_FACTOR))) / w;
    //text("oz" + str((((mat[2][0] * in.vertex1.x) + (mat[2][1] * in.vertex1.y) + (mat[2][2] * in.vertex1.z) + (mat[2][3] * FIXED_POINT_FACTOR)))), 0, 40);
    out.vertex1.z = (((mat[2][0] * in.vertex1.x) + (mat[2][1] * in.vertex1.y) + (mat[2][2] * in.vertex1.z) + (mat[2][3] * FIXED_POINT_FACTOR))) / w;

    w = ((mat[3][0] * in.vertex2.x) + (mat[3][1] * in.vertex2.y) + (mat[3][2] * in.vertex2.z) + (mat[3][3] * FIXED_POINT_FACTOR)) / FIXED_POINT_FACTOR;
    if (w < 1) {
        w = 1;
    }
    out.vertex2.x = (((mat[0][0] * in.vertex2.x) + (mat[0][1] * in.vertex2.y) + (mat[0][2] * in.vertex2.z) + (mat[0][3] * FIXED_POINT_FACTOR))) / w;
    out.vertex2.y = (((mat[1][0] * in.vertex2.x) + (mat[1][1] * in.vertex2.y) + (mat[1][2] * in.vertex2.z) + (mat[1][3] * FIXED_POINT_FACTOR))) / w;
    out.vertex2.z = (((mat[2][0] * in.vertex2.x) + (mat[2][1] * in.vertex2.y) + (mat[2][2] * in.vertex2.z) + (mat[2][3] * FIXED_POINT_FACTOR))) / w;

    w = ((mat[3][0] * in.vertex3.x) + (mat[3][1] * in.vertex3.y) + (mat[3][2] * in.vertex3.z) + (mat[3][3] * FIXED_POINT_FACTOR)) / FIXED_POINT_FACTOR;
    if (w < 1) {
        w = 1;
    }
    out.vertex3.x = (((mat[0][0] * in.vertex3.x) + (mat[0][1] * in.vertex3.y) + (mat[0][2] * in.vertex3.z) + (mat[0][3] * FIXED_POINT_FACTOR))) / w;
    out.vertex3.y = (((mat[1][0] * in.vertex3.x) + (mat[1][1] * in.vertex3.y) + (mat[1][2] * in.vertex3.z) + (mat[1][3] * FIXED_POINT_FACTOR))) / w;
    out.vertex3.z = (((mat[2][0] * in.vertex3.x) + (mat[2][1] * in.vertex3.y) + (mat[2][2] * in.vertex3.z) + (mat[2][3] * FIXED_POINT_FACTOR))) / w;

}




//matrix rotation functions (floating point)
void mat_rotate_x(float angle, float out[4][4]) {
    float rotation_matrix[4][4] = {{1,          0,           0, 0},
                                   {0, cos(angle), -sin(angle), 0},
                                   {0, sin(angle),  cos(angle), 0},
                                   {0,          0,           0, 1}};
    float mat_out[4][4];
    mat_mul(mat_camera, rotation_matrix, mat_out);
    memcpy(mat_camera, mat_out, sizeof(mat_out));
}

void mat_rotate_y(float angle, float out[4][4]) {
    float rotation_matrix[4][4] = {{ cos(angle), 0, sin(angle), 0},
                                   {          0, 1,          0, 0},
                                   {-sin(angle), 0, cos(angle), 0},
                                   {          0, 0,          0, 1}};
    float mat_out[4][4];

    mat_mul(mat_camera, rotation_matrix, mat_out);
    memcpy(mat_camera, mat_out, sizeof(mat_out));

    mat_mul(mat_cam_rotate, rotation_matrix, mat_out);
    memcpy(mat_cam_rotate, mat_out, sizeof(mat_out));
}

void mat_rotate_z(float angle, float out[4][4]) {
    float rotation_matrix[4][4] = {{cos(angle), -sin(angle), 0, 0},
                                   {sin(angle),  cos(angle), 0, 0},
                                   {         0,           0, 1, 0},
                                   {         0,           0, 0, 1}};
    float mat_out[4][4];
    mat_mul(mat_camera, rotation_matrix, mat_out);
    memcpy(mat_camera, mat_out, sizeof(mat_out));
}

