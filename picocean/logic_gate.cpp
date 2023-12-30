#include "logic_globals.h"
#include "../engine/render_globals.h"
#include "../engine/render_math.h"

#include "gate.h"

#define GATE_DRAW_DISTANCE (FIXED_POINT_FACTOR * 35)

void render_gate() {
    

    int32_t daylight = (global_time % (QUARTER_DAY * 4)) / QUARTER_DAY;
    int32_t progress = global_time % QUARTER_DAY;

    int32_t offset_x = 0;
    int32_t offset_y;
    //right gate
    //day
    if (daylight == 0) {
        offset_y = -FIXED_POINT_FACTOR * 2.5;

    //dusk
    } else if (daylight == 1) {
        offset_y = (-FIXED_POINT_FACTOR * 2.5) + ((((FIXED_POINT_FACTOR * 2.5) - (-FIXED_POINT_FACTOR * 2.5)) / QUARTER_DAY) * progress);

    //night
    } else if (daylight == 2) {
        offset_y = FIXED_POINT_FACTOR * 2.5;

    //dawn
    } else {
        offset_y = (FIXED_POINT_FACTOR * 2.5) + ((((-FIXED_POINT_FACTOR * 2.5) - (FIXED_POINT_FACTOR * 2.5)) / QUARTER_DAY) * progress);
    }

    //only render gate if within distance
    int32_t distancex = offset_x - camera_position_fixed_point[0];
    if (distancex < 0) {
        distancex *= -1;
    }

    int32_t distancey = offset_y - camera_position_fixed_point[2];
    if (distancey < 0) {
        distancey *= -1;
    }

    if (distancex < GATE_DRAW_DISTANCE && distancey < GATE_DRAW_DISTANCE) {
        for (int k = 0; k < GATE_MODEL; k++) {
            struct triangle_32 new_triangle;

            new_triangle.vertex1.x = gate_model[k].vertex1.x + offset_x;
            new_triangle.vertex1.y = gate_model[k].vertex1.y;
            new_triangle.vertex1.z = gate_model[k].vertex1.z + offset_y;
            
            new_triangle.vertex2.x = gate_model[k].vertex2.x + offset_x;
            new_triangle.vertex2.y = gate_model[k].vertex2.y;
            new_triangle.vertex2.z = gate_model[k].vertex2.z + offset_y;

            new_triangle.vertex3.x = gate_model[k].vertex3.x + offset_x;
            new_triangle.vertex3.y = gate_model[k].vertex3.y;
            new_triangle.vertex3.z = gate_model[k].vertex3.z + offset_y;

            new_triangle.shader_id = gate_model[k].shader_id;
            new_triangle.vertex_parameter1.color = gate_model[k].vertex_parameter1.color;
            new_triangle.vertex_parameter2.color = gate_model[k].vertex_parameter2.color;
            new_triangle.vertex_parameter3.color = gate_model[k].vertex_parameter3.color;

            render_triangle(new_triangle);
        }
    }




    //left gate
    //day
    if (daylight == 0) {
        offset_y = FIXED_POINT_FACTOR * 12.5;

    //dusk
    } else if (daylight == 1) {
        offset_y = (FIXED_POINT_FACTOR * 12.5) + ((((FIXED_POINT_FACTOR * 7.5) - (FIXED_POINT_FACTOR * 12.5)) / QUARTER_DAY) * progress);

    //night
    } else if (daylight == 2) {
        offset_y = FIXED_POINT_FACTOR * 7.5;

    //dawn
    } else {
        offset_y = (FIXED_POINT_FACTOR * 7.5) + ((((FIXED_POINT_FACTOR * 12.5) - (FIXED_POINT_FACTOR * 7.5)) / QUARTER_DAY) * progress);
    }

    //only render gate if within distance
    distancex = offset_x - camera_position_fixed_point[0];
    if (distancex < 0) {
        distancex *= -1;
    }

    distancey = offset_y - camera_position_fixed_point[2];
    if (distancey < 0) {
        distancey *= -1;
    }

    if (distancex < GATE_DRAW_DISTANCE && distancey < GATE_DRAW_DISTANCE) {
        for (int k = 0; k < GATE_MODEL; k++) {
            struct triangle_32 new_triangle;

            new_triangle.vertex1.x = gate_model[k].vertex1.x + offset_x;
            new_triangle.vertex1.y = gate_model[k].vertex1.y;
            new_triangle.vertex1.z = gate_model[k].vertex1.z + offset_y;
            
            new_triangle.vertex2.x = gate_model[k].vertex2.x + offset_x;
            new_triangle.vertex2.y = gate_model[k].vertex2.y;
            new_triangle.vertex2.z = gate_model[k].vertex2.z + offset_y;

            new_triangle.vertex3.x = gate_model[k].vertex3.x + offset_x;
            new_triangle.vertex3.y = gate_model[k].vertex3.y;
            new_triangle.vertex3.z = gate_model[k].vertex3.z + offset_y;

            new_triangle.shader_id = gate_model[k].shader_id;
            new_triangle.vertex_parameter1.color = gate_model[k].vertex_parameter1.color;
            new_triangle.vertex_parameter2.color = gate_model[k].vertex_parameter2.color;
            new_triangle.vertex_parameter3.color = gate_model[k].vertex_parameter3.color;

            render_triangle(new_triangle);
        }
    }


    return;
}