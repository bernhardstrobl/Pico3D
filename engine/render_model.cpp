// functions to render a single model
#include "render_globals.h"

//when using reduced 16 bit fixed point vertices (models smaller than 32 meters in each direction from local origin)
void render_model_16bit(triangle_16 *model, int32_t triangle_count) {

    for (int k = 0; k < triangle_count; k++) {
        struct triangle_32 new_triangle;

        new_triangle.vertex1.x = model[k].vertex1.x;
        new_triangle.vertex1.y = model[k].vertex1.y;
        new_triangle.vertex1.z = model[k].vertex1.z;
        
        new_triangle.vertex2.x = model[k].vertex2.x;
        new_triangle.vertex2.y = model[k].vertex2.y;
        new_triangle.vertex2.z = model[k].vertex2.z;

        new_triangle.vertex3.x = model[k].vertex3.x;
        new_triangle.vertex3.y = model[k].vertex3.y;
        new_triangle.vertex3.z = model[k].vertex3.z;

        new_triangle.shader_id = model[k].shader_id;
        new_triangle.texture_id = model[k].texture_id;
        new_triangle.vertex_parameter1.color = model[k].vertex_parameter1.color;
        new_triangle.vertex_parameter2.color = model[k].vertex_parameter2.color;
        new_triangle.vertex_parameter3.color = model[k].vertex_parameter3.color;

        render_triangle(new_triangle);
    }

    return;
}


//when using 32 bit fixed point vertices for large models or if fractional precision has been made higher
void render_model_32bit(triangle_32 *model, int32_t triangle_count) {

    for (int k = 0; k < triangle_count; k++) {
        struct triangle_32 new_triangle;

        new_triangle.vertex1.x = model[k].vertex1.x;
        new_triangle.vertex1.y = model[k].vertex1.y;
        new_triangle.vertex1.z = model[k].vertex1.z;
        
        new_triangle.vertex2.x = model[k].vertex2.x;
        new_triangle.vertex2.y = model[k].vertex2.y;
        new_triangle.vertex2.z = model[k].vertex2.z;

        new_triangle.vertex3.x = model[k].vertex3.x;
        new_triangle.vertex3.y = model[k].vertex3.y;
        new_triangle.vertex3.z = model[k].vertex3.z;

        new_triangle.shader_id = model[k].shader_id;
        new_triangle.texture_id = model[k].texture_id;
        new_triangle.vertex_parameter1.color = model[k].vertex_parameter1.color;
        new_triangle.vertex_parameter2.color = model[k].vertex_parameter2.color;
        new_triangle.vertex_parameter3.color = model[k].vertex_parameter3.color;

        render_triangle(new_triangle);
    }

    return;
}


//These functions are variants designed to load models from flash (prefixed with const)

void render_model_16bit_flash(const triangle_16 *model, int32_t triangle_count) {

    for (int k = 0; k < triangle_count; k++) {
        struct triangle_32 new_triangle;

        new_triangle.vertex1.x = model[k].vertex1.x;
        new_triangle.vertex1.y = model[k].vertex1.y;
        new_triangle.vertex1.z = model[k].vertex1.z;
        
        new_triangle.vertex2.x = model[k].vertex2.x;
        new_triangle.vertex2.y = model[k].vertex2.y;
        new_triangle.vertex2.z = model[k].vertex2.z;

        new_triangle.vertex3.x = model[k].vertex3.x;
        new_triangle.vertex3.y = model[k].vertex3.y;
        new_triangle.vertex3.z = model[k].vertex3.z;

        new_triangle.shader_id = model[k].shader_id;
        new_triangle.texture_id = model[k].texture_id;
        new_triangle.vertex_parameter1.color = model[k].vertex_parameter1.color;
        new_triangle.vertex_parameter2.color = model[k].vertex_parameter2.color;
        new_triangle.vertex_parameter3.color = model[k].vertex_parameter3.color;

        render_triangle(new_triangle);
    }

    return;
}


void render_model_32bit_flash(const triangle_32 *model, int32_t triangle_count) {

    for (int k = 0; k < triangle_count; k++) {
        struct triangle_32 new_triangle;

        new_triangle.vertex1.x = model[k].vertex1.x;
        new_triangle.vertex1.y = model[k].vertex1.y;
        new_triangle.vertex1.z = model[k].vertex1.z;
        
        new_triangle.vertex2.x = model[k].vertex2.x;
        new_triangle.vertex2.y = model[k].vertex2.y;
        new_triangle.vertex2.z = model[k].vertex2.z;

        new_triangle.vertex3.x = model[k].vertex3.x;
        new_triangle.vertex3.y = model[k].vertex3.y;
        new_triangle.vertex3.z = model[k].vertex3.z;

        new_triangle.shader_id = model[k].shader_id;
        new_triangle.texture_id = model[k].texture_id;
        new_triangle.vertex_parameter1.color = model[k].vertex_parameter1.color;
        new_triangle.vertex_parameter2.color = model[k].vertex_parameter2.color;
        new_triangle.vertex_parameter3.color = model[k].vertex_parameter3.color;

        render_triangle(new_triangle);
    }

    return;
}