//handles npc logic for npcs that are important for the game storyline like quest givers and shop keepers

void init_quest_npcs() {

    //Guard at the gate
    npc_quest_list[0].status = 1;
    npc_quest_list[0].direction = 0;
    npc_quest_list[0].status = 1;
    npc_quest_list[0].shirt_color = 0x0F00; 
    npc_quest_list[0].dialogue = 0; 
    npc_quest_list[0].x = -5 * FIXED_POINT_FACTOR;
    npc_quest_list[0].y = -1 * FIXED_POINT_FACTOR;

}

void render_quest_npcs() {

    if (skip_frame == 1) {
        return;
    }

    for (int i = 0; i < MAX_QUEST_NPCS; i++) {
        if (npc_quest_list[i].status != -1) {

            //we only load npcs if they are close to the player
            int32_t distancex = npc_quest_list[i].x - camera_position_fixed_point[0];
            if (distancex < 0) {
                distancex *= -1;
            }

            int32_t distancey = npc_quest_list[i].y - camera_position_fixed_point[2];
            if (distancey < 0) {
                distancey *= -1;
            }

            //we also only load them in if their bounding box is visible
            if (render_view_frustum_culling(npc_quest_list[i].x, FIXED_POINT_FACTOR, npc_quest_list[i].y, FIXED_POINT_FACTOR / 2, FIXED_POINT_FACTOR, FIXED_POINT_FACTOR / 2) == 0) {
                continue;
            }

            if (distancex < NPC_VIEW_DISTANCE && distancey < NPC_VIEW_DISTANCE) {
                for (int j = 0; j < NPC; j++) {

                    struct triangle_32 new_triangle;

                    new_triangle.vertex1.x = npc[j].vertex1.x + npc_quest_list[i].x;
                    new_triangle.vertex1.y = npc[j].vertex1.y;
                    new_triangle.vertex1.z = npc[j].vertex1.z + npc_quest_list[i].y;
                    
                    new_triangle.vertex2.x = npc[j].vertex2.x + npc_quest_list[i].x;
                    new_triangle.vertex2.y = npc[j].vertex2.y;
                    new_triangle.vertex2.z = npc[j].vertex2.z + npc_quest_list[i].y;

                    new_triangle.vertex3.x = npc[j].vertex3.x + npc_quest_list[i].x;
                    new_triangle.vertex3.y = npc[j].vertex3.y;
                    new_triangle.vertex3.z = npc[j].vertex3.z + npc_quest_list[i].y;

                    new_triangle.shader_id = npc[j].shader_id;
                    if(npc[j].vertex_parameter1.color == 0xf000) {
                        new_triangle.vertex_parameter1.color = npc_quest_list[i].shirt_color;
                        new_triangle.vertex_parameter2.color = npc_quest_list[i].shirt_color;
                        new_triangle.vertex_parameter3.color = npc_quest_list[i].shirt_color;
                    } else {
                        new_triangle.vertex_parameter1.color = npc[j].vertex_parameter1.color;
                        new_triangle.vertex_parameter2.color = npc[j].vertex_parameter2.color;
                        new_triangle.vertex_parameter3.color = npc[j].vertex_parameter3.color;
                    }
                    render_triangle(new_triangle);
                    
                }
            }
        }
    }
}