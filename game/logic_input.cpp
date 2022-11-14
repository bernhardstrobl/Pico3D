//handle inputs of the console



void logic_input() {


    //input is processed depending on current game state
    //if the game is currently running normally
    if (menu == 0) {

        #ifdef GAMESCOM
        if (pressed(A) || pressed(B) || pressed(X) || pressed(Y) || pressed(UP) || pressed(DOWN) || pressed(LEFT) || pressed(RIGHT)
            || button(A) || button(B) || button(X) || button(Y) || button(UP) || button(DOWN) || button(LEFT) || button(RIGHT)) {
            input_idle_timer = 0;
        }
        #endif

        //process input to transform camera
        //turn left
        if (button(LEFT) == true) {
            yaw += INPUT_SENSITIVITY;
            update_camera();
        }

        //turn right
        if (button(RIGHT) == true) {
            yaw -= INPUT_SENSITIVITY;
            update_camera();
        }

        //Forward
        if (button(UP) == true) {
            move_camera(INPUT_SENSITIVITY);
            update_camera();
        }

        //Back
        if (button(DOWN) == true) {
            move_camera(-INPUT_SENSITIVITY);
            update_camera();
        }

        //Look up
        if (button(X) == true) {
            pitch += 0.1;
            update_camera();
        }

        //Look down
        if (button(B) == true) {
            pitch -= 0.1;
            update_camera();
        }   



        //Context sensitive A button
        if (pressed(A) == true) {
            //shoot at zombies
            if (player_area == AREA_OUTSKIRTS) {
                logic_shoot();
            }

            if (close_npc != -1) {
                talk_quest_npc();
            }
        }

        #ifndef GAMESCOM
        //open game menu
        if (pressed(Y) == true) {
            menu = MENU_MAIN;
        }
        #else
        //show battery life
        if (button(Y) == true) {
            show_battery = 1;
        } else {
            show_battery = 0;
        }
        #endif


    //if the player is in the main menu
    } else if (menu == MENU_MAIN) {

        //close menu
        if (pressed(Y) == true) {
            menu = 0;
        }

        //Increase screen brightness
        if (pressed(UP) == true) {
            brightness += BACKLIGHT_INCREMENT;
            if (brightness > 100) {
                brightness = 100;
            }
            backlight(brightness);
        }

        //Lower screen brightness
        if (pressed(DOWN) == true) {
            brightness -= BACKLIGHT_INCREMENT;
            if (brightness < 0) {
                brightness = 0;
            }
            backlight(brightness);
        }

        #ifdef FREE_ROAM
        //move up
        if (button(X) == true) {
            camera_position[1] += 0.1;
            update_camera();

        }

        //move down
        if (button(B) == true) {
            camera_position[1] -= 0.1;
            update_camera();
        }
        #endif

        //TODO: sound on/off
        if (pressed(A) == true) {

        }

        #ifdef DEBUG_SHADERS
        //cycle through debug shaders if the feature is on
        if (pressed(LEFT) == true) {
            //Lowest debug shader ID
            if (shader_override < 250) {
                shader_override = 250;
            //Highest shader ID
            } else if (shader_override >= 254) {
                shader_override = 0;
            } else if (shader_override >= 250) {
                shader_override++;
            }
        }
        #endif

    } else if (menu == MENU_START) {

        if (pressed(A) || pressed(B) || pressed(X) || pressed(Y) || pressed(UP) || pressed(DOWN) || pressed(LEFT) || pressed(RIGHT)) {
            logic_new_game();
            menu = 0;
        }

    } else if (menu == MENU_DEATH) {
        
    }



}