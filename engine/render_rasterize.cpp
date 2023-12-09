#include "render_globals.h"
#include "render_math.h"

#include "../chunk_data.h" // chunk_texture_list

//all the shaders
#include "shader_wireframe.cpp"

uint8_t shader_override = 0;

alignas(4) static int16_t zbuffer[SCREEN_WIDTH * SCREEN_HEIGHT] = { };

color_t sky;

int zbuffer_scale = FIXED_POINT_FACTOR / 16;

uint8_t animated_texture_offset = 0;
uint8_t animated_texture_counter = 0;

//The rasterizer along with shaders is relatively small, so it can run from scratch memory
//Note that if the rasterizer is run from flash instead, the integer divide function should also be put into flash
// --> Remove target_compile_definitions(pico3d PUBLIC PICO_DIVIDER_IN_RAM=1) in CMake file
//Performance may otherwise be significantly hampered due to the veneers translating function calls between RAM/flash
#ifdef RASTERIZER_IN_FLASH
#define RASTERIZE_SECTION
#else
#include "pico/platform.h"
#define RASTERIZE_SECTION __scratch_x("render_rasterize")
#endif

void RASTERIZE_SECTION render_rasterize(uint32_t num_triangle, color_t *fb) {

    //we clear the screen either to a default black color (Fastest at around 60us)
    //memset(fb, 0, SCREEN_WIDTH * SCREEN_HEIGHT * 2);

    //or a set color/gradient (basically a sky). This uses around 160us 
    //unless more advanced effects are used like a gradient or additional ground.
    /*
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        fb[i] = 0xCFD0; //GBAR
    }*/

    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        fb[i] = sky; //GBAR
    }
    


    //default Z-Buffer to 1.0 / Max Scaling factor
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        zbuffer[i] = FIXED_POINT_FACTOR;
    }

    
    //if desired, put white background in Z-Buffer output
    //memset(fb, 0xFF, SCREEN_WIDTH * SCREEN_HEIGHT * 2);



    //Go through all the triangles in the triangle list
    for (int current_triangle = 0; current_triangle < num_triangle; current_triangle++) {
  
        int16_t x1 = triangle_list_current[current_triangle].vertex1.x;
        int16_t y1 = triangle_list_current[current_triangle].vertex1.y;

        int16_t x2 = triangle_list_current[current_triangle].vertex2.x;
        int16_t y2 = triangle_list_current[current_triangle].vertex2.y;

        int16_t x3 = triangle_list_current[current_triangle].vertex3.x;
        int16_t y3 = triangle_list_current[current_triangle].vertex3.y;

        //Pull in shader id for the triangle
        uint8_t shader_id = triangle_list_current[current_triangle].shader_id;

        //optionally override the shader id for debugging
        if (shader_override != 0) {
            shader_id = shader_override;
        }

#ifdef DEBUG_SHADERS
        //once we have all 3 on screen vertex points, we can optionally choose to display a wireframe instead of rasterizing
        if (shader_id == 250) {
            render_line(fb, x1, y1, x2, y2);
            render_line(fb, x2, y2, x3, y3);
            render_line(fb, x3, y3, x1, y1);
            continue;
        }
#endif

        //Determine the extremes of the triangle on screen for rasterization bounding box
        int16_t x_large = x1;
        int16_t x_small = x1;
        int16_t y_large = y1;
        int16_t y_small = y1;


        if (x2 > x_large)
            x_large = x2;
        if (x3 > x_large)
            x_large = x3;
        if (x2 < x_small)
            x_small = x2;
        if (x3 < x_small)
            x_small = x3;

        if (y2 > y_large)
            y_large = y2;
        if (y3 > y_large)
            y_large = y3;
        if (y2 < y_small)
            y_small = y2;
        if (y3 < y_small)
            y_small = y3;

        //limit bounding box to screen
        if (x_large > SCREEN_WIDTH)
            x_large = SCREEN_WIDTH;
        if (x_small < 0)
            x_small = 0;

        if (y_large > SCREEN_HEIGHT)
            y_large = SCREEN_HEIGHT;
        if (y_small < 0)
            y_small = 0;


        uint8_t texture_id = triangle_list_current[current_triangle].texture_id;
        uint32_t image_size;
        const color_t *texture;

        uint32_t u1;
        uint32_t v1;
        uint32_t u2;
        uint32_t v2;
        uint32_t u3;
        uint32_t v3;

        //shader IDs >= 100 indicate the use of a texture shader, preload texture info
        if (shader_id >= 100 && shader_id < 200) {
            image_size = chunk_texture_list[texture_id].texture_size;
            texture = chunk_texture_list[texture_id].image;


            //simple UV coordinates for vertices
            u1 = triangle_list_current[current_triangle].vertex_parameter1.uv[0];
            v1 = triangle_list_current[current_triangle].vertex_parameter1.uv[1];
            u2 = triangle_list_current[current_triangle].vertex_parameter2.uv[0];
            v2 = triangle_list_current[current_triangle].vertex_parameter2.uv[1];
            u3 = triangle_list_current[current_triangle].vertex_parameter3.uv[0];
            v3 = triangle_list_current[current_triangle].vertex_parameter3.uv[1];
        }

        //precalculate area of triangle for later to find barycentric coordinates
        int32_t area = (x3 - x1) * (y2 - y1) - (y3 - y1) * (x2 - x1);

        if(area == 0)
            continue;

        //inverse Z coordinates
        int32_t zi1 = ((FIXED_POINT_FACTOR * FIXED_POINT_FACTOR) / triangle_list_current[current_triangle].vertex1.z);
        int32_t zi2 = ((FIXED_POINT_FACTOR * FIXED_POINT_FACTOR) / triangle_list_current[current_triangle].vertex2.z);
        int32_t zi3 = ((FIXED_POINT_FACTOR * FIXED_POINT_FACTOR) / triangle_list_current[current_triangle].vertex3.z);


        //once we have the limits of the bounding box, loop over that area of the screen
        for (int32_t y = y_small; y < y_large; y++) {

            int8_t skipline = 0;

            for (int32_t x = x_small; x < x_large; x++) {

                //Determine if the pixel is within the triangle using edge functions
                
                int32_t edge1 = (x - x2) * (y3 - y2) - (y - y2) * (x3 - x2);
                if (edge1 < 0) {
                    if (skipline == 1) {
                        x = x_large;
                    }
                    continue;
                }

                int32_t edge2 = (x - x3) * (y1 - y3) - (y - y3) * (x1 - x3);
                if (edge2 < 0) {
                    if (skipline == 1) {
                        x = x_large;
                    }
                    continue;
                }

                int32_t edge3 = (x - x1) * (y2 - y1) - (y - y1) * (x2 - x1);
                if (edge3 < 0) {
                    if (skipline == 1) {
                        x = x_large;
                    }
                    continue;
                }


                skipline = 1;


                int32_t w1 = (FIXED_POINT_FACTOR * edge1) / area;
                int32_t w2 = (FIXED_POINT_FACTOR * edge2) / area;
                int32_t w3 = FIXED_POINT_FACTOR - (w1 + w2);

                //interpolated Z coordinate
                int32_t z = ((FIXED_POINT_FACTOR * FIXED_POINT_FACTOR * FIXED_POINT_FACTOR) / (w1 * zi1 + w2 * zi2 + w3 * zi3));
                

                //check if a closer value exists in Z-Buffer
                if (zbuffer[y * SCREEN_WIDTH + x] < z) {
                    continue; //skip pixel if there is
                } else {
                    zbuffer[y * SCREEN_WIDTH + x] = z; //write new z buffer value
                }

                //we can now decide on a shader to use
                //flat shading using first vertex color as triangle color
                if (shader_id == 1) {
                    color_t triangle_color = triangle_list_current[current_triangle].vertex_parameter1.color;
                    fb[y * SCREEN_WIDTH + x] = triangle_color; 


                //linear color interpolation shader using separate vertex colors given by triangle
                } else if (shader_id == 2) {

                    color_t color = triangle_list_current[current_triangle].vertex_parameter1.color;
                    uint8_t c1r = color & 0x000F;
                    uint8_t c1b = (color >> 8) & 0x000F;
                    uint8_t c1g = (color >> 12) & 0x000F;

                    color = triangle_list_current[current_triangle].vertex_parameter2.color;
                    uint8_t c2r = color & 0x000F;
                    uint8_t c2b = (color >> 8) & 0x000F;
                    uint8_t c2g = (color >> 12) & 0x000F;
                    
                    color = triangle_list_current[current_triangle].vertex_parameter3.color;
                    uint8_t c3r = color & 0x000F;
                    uint8_t c3b = (color >> 8) & 0x000F;
                    uint8_t c3g = (color >> 12) & 0x000F;

                    uint32_t r = (w1 * c1r + w2 * c2r + w3 * c3r) / FIXED_POINT_FACTOR;
                    uint32_t g = (w1 * c1g + w2 * c2g + w3 * c3g) / FIXED_POINT_FACTOR;
                    uint32_t b = (w1 * c1b + w2 * c2b + w3 * c3b) / FIXED_POINT_FACTOR;

                    //we have to give a small positive bias
                    if (r < 15)
                        r++;
                    if (g < 15)
                        g++;
                    if (b < 15)
                        b++;

                    color = g;
                    color <<= 4;
                    color |= b;
                    color <<= 8;
                    color |= r;

                    fb[y * SCREEN_WIDTH + x] = color;


                //Textures shaders exported by the chunk export
                } else if (shader_id == 100) {


                    
                    uint32_t u = (w1 * u1 + w2 * u2 + w3 * u3);
                    uint32_t v = (w1 * v1 + w2 * v2 + w3 * v3);

                    //scale back to pixel position
                    u = u / FIXED_POINT_FACTOR;
                    v = v / FIXED_POINT_FACTOR;

                    //clamp uv coordinates
                    if (u > image_size - 1)
                        u = image_size - 1;
                    if (v > image_size - 1)
                        v = image_size - 1;

                    fb[y * SCREEN_WIDTH + x] = texture[u * image_size + v];


                //Scrolling Texture by chunk export
                } else if (shader_id == 101) {

                    
                    uint32_t u = (w1 * u1 + w2 * u2 + w3 * u3);
                    uint32_t v = (w1 * v1 + w2 * v2 + w3 * v3);


                    //scale back to pixel position
                    u = u / FIXED_POINT_FACTOR;
                    v = v / FIXED_POINT_FACTOR;

                    //clamp uv coordinates
                    if (u > image_size - 1)
                        u = image_size - 1;
                    if (v > image_size - 1)
                        v = image_size - 1;

                    u = (u + animated_texture_offset) % 32;

                    fb[y * SCREEN_WIDTH + x] = texture[u * image_size + v];

                
                #ifdef DEBUG_SHADERS

                //DEBUG SHADERS
                //the following shaders are additional debug shaders, starting at ID 250

                //Wireframe mode uses shader_id 250 already
                //} else if (shader_id == 250) {

                //flat shading using triangle color 
                } else if (shader_id == 251) {

                    //test color based on current triangle
                    color_t triangle_color;
                    if (current_triangle % 5 == 0) {
                        triangle_color = 0x0FA0;
                    } else if (current_triangle % 5 == 1) {
                        triangle_color = 0x00AF;
                    } else if (current_triangle % 5 == 2) {
                        triangle_color = 0xF0A0;
                    } else if (current_triangle % 5 == 3) {
                        triangle_color = 0xFFA0;
                    } else if (current_triangle % 5 == 4) {
                        triangle_color = 0xF0AF;
                    }

                    fb[y * SCREEN_WIDTH + x] = triangle_color; 

                //Z-Buffer output
                } else if (shader_id == 252) {
                    int32_t grayscale = z / zbuffer_scale;
                    switch (grayscale) {
                        case 0: fb[y * SCREEN_WIDTH + x] = 0x0000; break;
                        case 1: fb[y * SCREEN_WIDTH + x] = 0x1101; break;
                        case 2: fb[y * SCREEN_WIDTH + x] = 0x2202; break;
                        case 3: fb[y * SCREEN_WIDTH + x] = 0x3303; break;
                        case 4: fb[y * SCREEN_WIDTH + x] = 0x4404; break;
                        case 5: fb[y * SCREEN_WIDTH + x] = 0x5505; break;
                        case 6: fb[y * SCREEN_WIDTH + x] = 0x6606; break;
                        case 7: fb[y * SCREEN_WIDTH + x] = 0x7707; break;
                        case 8: fb[y * SCREEN_WIDTH + x] = 0x8808; break;
                        case 9: fb[y * SCREEN_WIDTH + x] = 0x9909; break;
                        case 10: fb[y * SCREEN_WIDTH + x] = 0xAA0A; break;
                        case 11: fb[y * SCREEN_WIDTH + x] = 0xBB0B; break;
                        case 12: fb[y * SCREEN_WIDTH + x] = 0xCC0C; break;
                        case 13: fb[y * SCREEN_WIDTH + x] = 0xDD0D; break;
                        case 14: fb[y * SCREEN_WIDTH + x] = 0xEE0E; break;
                        case 15: fb[y * SCREEN_WIDTH + x] = 0xFF0F; break;
                        default: fb[y * SCREEN_WIDTH + x] = 0x000F; break; //red if something is out of range
                    }

                //linear color interpolation shader using separate vertex colors rgb (debug)
                } else if (shader_id == 253) {
                    uint32_t r = ((w1 * 15) / FIXED_POINT_FACTOR);
                    uint32_t g = ((w2 * 15) / FIXED_POINT_FACTOR);
                    uint32_t b = ((w3 * 15) / FIXED_POINT_FACTOR);


                    uint16_t color = g;
                    color <<= 4;
                    color |= b;
                    color <<= 8;
                    color |= r;

                    fb[y * SCREEN_WIDTH + x] = color;

                //Test texture with linear interpolation
                } else if (shader_id == 254) {


                    int image_size = 16;

                    //simple UV coordinates for vertices
                    u1 = 0;
                    v1 = image_size;
                    u2 = image_size;
                    v2 = 0;
                    u3 = 0;
                    v3 = 0;
                    
                    //Plane model
                    if (current_triangle % 2 == 0) {
                        u1 = 0;
                        v1 = image_size;
                        u2 = image_size;
                        v2 = image_size;
                        u3 = image_size;
                        v3 = 0;
                    }
                    


                    uint32_t u = (w1 * u1 + w2 * u2 + w3 * u3);
                    uint32_t v = (w1 * v1 + w2 * v2 + w3 * v3);


                    /*

                    //Perspective interpolation (more cpu power needed!)
                    u1 = u1 * zi1;
                    v1 = v1 * zi1;
                    u2 = u2 * zi2;
                    v2 = v2 * zi2;
                    u3 = u3 * zi3;
                    v3 = v3 * zi3;


                    uint32_t u = (w1 * u1 + w2 * u2 + w3 * u3) / FIXED_POINT_FACTOR / FIXED_POINT_FACTOR;
                    uint32_t v = (w1 * v1 + w2 * v2 + w3 * v3) / FIXED_POINT_FACTOR / FIXED_POINT_FACTOR;

                    u = u * z;
                    v = v * z;

                    */

                    //scale back to pixel position
                    u = u / FIXED_POINT_FACTOR;
                    v = v / FIXED_POINT_FACTOR;

                    //clamp uv coordinates
                    if (u > image_size - 1)
                        u = image_size - 1;
                    if (v > image_size - 1)
                        v = image_size - 1;

                    fb[y * SCREEN_WIDTH + x] = test_texture[v * image_size + u];



                //if a known shader id is not specified, return red to alert dev
                } else {
                    fb[y * SCREEN_WIDTH + x] = 0x000F;
                #endif

                }

            }

        }

    }
}