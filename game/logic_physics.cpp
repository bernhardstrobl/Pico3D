#include "logic_globals.h"
#include "../engine/chunk_globals.h"
#include "../engine/render_math.h"

//calling this "physics" is very much a stretch but it will do for the time being
//chunks 1 are for city npcs, 2 for zombies. Anything above 0 can be entered by the player (except when gate is closed)
uint8_t chunk_physics[12][12] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {0, 1, 1, 1, 0, 0, 2, 2, 2, 2, 0, 0},
                                 {0, 1, 0, 1, 0, 0, 2, 2, 2, 2, 5, 0},
                                 {0, 1, 0, 1, 0, 0, 2, 2, 2, 2, 2, 0},
                                 {0, 1, 0, 1, 0, 0, 2, 2, 2, 2, 2, 0},
                                 {0, 1, 1, 1, 0, 0, 2, 2, 2, 2, 2, 0},
                                 {0, 1, 1, 1, 1, 3, 2, 2, 2, 2, 2, 0},
                                 {0, 1, 1, 1, 0, 0, 2, 2, 2, 2, 2, 0},
                                 {0, 0, 1, 0, 0, 0, 2, 2, 2, 2, 2, 0},
                                 {0, 0, 1, 0, 0, 0, 2, 2, 2, 2, 2, 0},
                                 {0, 0, 1, 0, 0, 0, 2, 2, 2, 2, 2, 0},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                };

//check if this location can be walked on by npcs or the player
//currently only does complete chunk granularity

uint8_t chunk_traversable(int32_t x, int32_t y, uint8_t character_type) {

    int32_t chunk_x;
    int32_t chunk_y;
    //check which chunk would be affected by the location
    //make sure there is a decent gap
    chunk_locate(x + CHUNK_BORDER, y + CHUNK_BORDER, chunk_x, chunk_y);
    uint8_t c1 = chunk_physics[chunk_x][chunk_y];

    chunk_locate(x - CHUNK_BORDER, y + CHUNK_BORDER, chunk_x, chunk_y);
    uint8_t c2 = chunk_physics[chunk_x][chunk_y];

    chunk_locate(x + CHUNK_BORDER, y - CHUNK_BORDER, chunk_x, chunk_y);
    uint8_t c3 = chunk_physics[chunk_x][chunk_y];

    chunk_locate(x - CHUNK_BORDER, y - CHUNK_BORDER, chunk_x, chunk_y);
    uint8_t c4 = chunk_physics[chunk_x][chunk_y];


    //character type defines the boundaries for the different characters:
    //depending on the character we make a decision

    //0 - player (confined to Zone 1/3 or 2 at night)
    if (character_type == 0) {
        
        int32_t daylight = (global_time % (QUARTER_DAY * 4)) / QUARTER_DAY;

        if (daylight == 2) {
            if (player_area == AREA_OUTSKIRTS || player_area == AREA_OUTSKIRT_STABLES) {
                if ((c1 == 2 || c1 == 5) && (c2 == 2 || c2 == 5) && (c3 == 2 || c3 == 5) && (c4 == 2 || c4 == 5)) {
                    return 1;
                } else {
                    return 0;
                }
            } else {
                if (c1 == 2 || c2 == 2 || c3 == 2 || c4 == 2) {
                    return 0;
                } else if (c1 == 0 || c2 == 0 || c3 == 0 || c4 == 0) {
                    return 0;
                } else {
                    return 1;
                }
            }
        } else {
            if (c1 == 0 || c2 == 0 || c3 == 0 || c4 == 0) {
                return 0;
            } else {
                return 1;
            }
        }
        
    //1 - city resident (confined to zone 1)
    } else if (character_type == 1) {
        if (c1 != 1 || c2 != 1 || c3 != 1 || c4 != 1) {
            return 0;
        } else {
            return 1;
        }

    //2 - zombie (confined to zone 2)
    } else if (character_type == 2) {
        if (c1 != 2 || c2 != 2 || c3 != 2 || c4 != 2) {
            return 0;
        } else {
            return 1;
        }
    }

    return 0;

}