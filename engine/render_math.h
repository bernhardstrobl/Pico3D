//File with math functions mainly for rendering
//Several functions have both fixed and floating point implementations

//When converting to fixed points, a scaling factor power 2 allows
//the compiler to perform bit-shifts instead of more expensive divisions/multiplications
#define FIXED_POINT_FACTOR 1024





inline int float_to_int (float in) {
    int32_t out = in * FIXED_POINT_FACTOR;
    return out;
}

inline float int_to_float (int32_t in) {
    float out = in * 1.0 / FIXED_POINT_FACTOR;
    return out;
}

//4x4 floating point matrix multiply
inline void mat_mul(float mat1[4][4], float mat2[4][4], float out[4][4]) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++){
            out[y][x] = 0;
            for (int z = 0; z < 4; z++){
                out[y][x] += mat1[y][z] * mat2[z][x];
            }
        }
    }
}

/*
//4x4 matrix debug output
inline void mat_debug(float mat[4][4], int y_offset) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++){
            text(str(mat[y][x], 2), x * 30, y * 10 + y_offset);
        }
    }
}

inline void mat_debug_fixed_point(int32_t mat[4][4], int y_offset) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++){
            text(str(mat[y][x], 0), x * 30, y * 10 + y_offset);
        }
    }
}
*/

//Convert 4x4 Matrix to integer
inline void mat_convert_float_fixed(float mat_in[4][4], int32_t mat_out[4][4]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            mat_out[i][j] = float_to_int(mat_in[i][j]);
        }
    }
}
