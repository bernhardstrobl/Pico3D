#include "render_globals.h"
#include "chunk_globals.h"
#include "render_math.h"

float camera_position[3] = {0.0, 0.0, 0.0};
int32_t camera_position_fixed_point[3] = {0, 0, 0};
float pitch = 0;
float yaw = 0;

#ifndef NO_GLOBAL_OFFSET
    int32_t global_offset_x;
    int32_t global_offset_z;
#endif


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

//perspective projection matrix
float mat_projection[4][4] = {{ atanf((CAMERA_FOVX * PI / 180) * 0.5), 0.0, 0.0, 0.0},
                              { 0.0, atanf((CAMERA_FOVY * PI / 180) * 0.5), 0.0, 0.0},
                              { 0.0, 0.0, -((ZFAR + ZNEAR) / (ZFAR - ZNEAR)), -((2.0*ZFAR*ZNEAR) / (ZFAR - ZNEAR))},
                              { 0.0, 0.0, -1.0, 0.0}};

int32_t mat_vp[4][4] = {{ 0, 0, 0, 0},
                        { 0, 0, 0, 0},
                        { 0, 0, 0, 0},
                        { 0, 0, 0, 0}};

//camera functions
float dot_product3(float vec1[3], float vec2[3]) {
    return(vec1[0] * vec2[0] + vec1[1] * vec2[1] +vec1[2] * vec2[2]);
}



void update_camera() {

    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);
    float cosYaw = cos(yaw);
    float sinYaw = sin(yaw);


    float xaxis[3] = { cosYaw, 0, -sinYaw };
    float yaxis[3] = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
    float zaxis[3] = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };


    mat_camera[0][0] = xaxis[0];
    mat_camera[0][1] = xaxis[1];
    mat_camera[0][2] = xaxis[2];
    mat_camera[0][3] = -dot_product3( xaxis, camera_position );

    mat_camera[1][0] = yaxis[0];
    mat_camera[1][1] = yaxis[1];
    mat_camera[1][2] = yaxis[2];
    mat_camera[1][3] = -dot_product3( yaxis, camera_position );

    mat_camera[2][0] = zaxis[0];
    mat_camera[2][1] = zaxis[1];
    mat_camera[2][2] = zaxis[2];
    mat_camera[2][3] = -dot_product3( zaxis, camera_position );

    mat_camera[3][0] = 0;
    mat_camera[3][1] = 0;
    mat_camera[3][2] = 0;
    mat_camera[3][3] = 1;
    

    //create fixed point camera position
    camera_position_fixed_point[0] = float_to_int(camera_position[0]);
    camera_position_fixed_point[1] = float_to_int(camera_position[1]);
    camera_position_fixed_point[2] = float_to_int(camera_position[2]);
}

void move_camera(float move) {
    //we know the cameras yaw, we can create forward/backward movement

    float rotation_matrix[4][4] = {{ cos(yaw), 0, sin(yaw), 0},
                                   {      0, 1,          0, 0},
                                   {-sin(yaw), 0, cos(yaw), 0},
                                   {      0, 0,          0, 1}};
    float mat_out[4][4];

    float translate_matrix[4][4] = {{1, 0, 0, 0},
                                    {0, 1, 0, 0},
                                    {0, 0, 1, move},
                                    {0, 0, 0, 1}};
    
    mat_mul(rotation_matrix, translate_matrix, mat_out);

    camera_position[0] -= mat_out[0][3];
    camera_position[2] -= mat_out[2][3];
}

void render_view_projection() {
    //we prepare the matrix to convert objects from world to view/projection space
    #ifdef NO_GLOBAL_OFFSET
        float mat_vp_float[4][4];
        mat_mul(mat_projection, mat_camera, mat_vp_float);

        //convert the resulting matrix to integers
        mat_convert_float_fixed(mat_vp_float, mat_vp);

    //use global offsets for large worlds (a bit hacky!)
    #else
        float mat_vp_float[4][4];
        float mat_camera_global[4][4];

        float old_position_x = camera_position[0];
        float old_position_z = camera_position[2];
        
        //calculate and update needed global offsets
        global_offset_x = camera_position[0] / CHUNK_UNITS;
        global_offset_z = camera_position[2] / CHUNK_UNITS;

        //we update the camera position to the new one and move the camera
        camera_position[0] -= global_offset_x * CHUNK_UNITS;
        camera_position[2] -= global_offset_z * CHUNK_UNITS;

        update_camera();

        //restore old camera positions for future correct updates
        camera_position[0] = old_position_x;
        camera_position[2] = old_position_z;

        //create fixed point camera position
        camera_position_fixed_point[0] = float_to_int(camera_position[0]);
        camera_position_fixed_point[1] = float_to_int(camera_position[1]);
        camera_position_fixed_point[2] = float_to_int(camera_position[2]);

        mat_mul(mat_projection, mat_camera, mat_vp_float);

        //convert the resulting matrix to integers
        mat_convert_float_fixed(mat_vp_float, mat_vp);
    #endif

}
