cmake_minimum_required(VERSION 3.12)

# Pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico32 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

option(GAMESCOM "Build gamescom version" OFF)

# Initialize the Pico SDK
pico_sdk_init()

# Find the PicoSystem SDK
find_package(PICOSYSTEM REQUIRED)

set(GLOBAL_DEFINES)

if(GAMESCOM)
    list(APPEND GLOBAL_DEFINES GAMESCOM) #enables tweaks for a gamescom version with peace loving balloons instead of zombies
    set(ZOMBIE_SOURCES
        game/gamescom/logic_balloon.cpp
        game/gamescom/logic_shoot_balloon.cpp
    )
else()
    set(ZOMBIE_SOURCES
        game/logic_shoot.cpp
        game/logic_zombies.cpp
    )
endif()

#Comment out/in defines if needed (for debugging)
#list(APPEND GLOBAL_DEFINES SKIP_START) #skips the starting splash/menu and goes straight into normal gameplay (menu = 0)
#list(APPEND GLOBAL_DEFINES FREE_ROAM) #set to ignore chunk physics for player
#list(APPEND GLOBAL_DEFINES DEBUG_SHADERS) #debug shaders are those with shader_id >= 250
#list(APPEND GLOBAL_DEFINES NO_GLOBAL_OFFSET) #disables using a global offset to move triangles and camera closer to origin

#Defines for performance profiling
#list(APPEND GLOBAL_DEFINES DEBUG_INFO) #adds information on core times and triangle counts in the main menu
#list(APPEND GLOBAL_DEFINES NO_NPCS) #disable all npcs including Zombies
#list(APPEND GLOBAL_DEFINES RASTERIZER_IN_FLASH) #puts the render_rasterize function for core 1 into flash instead of scratch RAM
#list(APPEND GLOBAL_DEFINES FRAME_COUNTER) #tallies frametimes for performance analysis
#list(APPEND GLOBAL_DEFINES CONTENTION_COUNTER) #enables counters for RAM contention on the 4 main banks
#list(APPEND GLOBAL_DEFINES CPU_LED_LOAD) #CPU load on LED (Core1: Green-40fps, Yellow-20fps, Red-10fps), blue if core 0 overloaded (logic too slow)
#list(APPEND GLOBAL_DEFINES BENCHMARK) #starts a benchmark recording average frametime and rough fps counter (takes 3 minutes!)

picosystem_executable(
    pico3d
    main.cpp
    chunk_data.cpp

    engine/render_camera.cpp
    engine/render_chunk.cpp
    engine/render_clipping.cpp
    engine/render_culling.cpp
    engine/render_lighting.cpp
    engine/render_model.cpp
    engine/render_rasterize.cpp
    engine/render_triangle.cpp

    game/logic_day_night_cycle.cpp
    game/logic_demo.cpp
    game/logic_events.cpp
    game/logic_gate.cpp
    game/logic_grass.cpp
    game/logic_info_text.cpp
    game/logic_input.cpp
    game/logic_menu.cpp
    game/logic_npc.cpp
    game/logic_physics.cpp
    game/logic_quest_npcs.cpp
    game/logic_random.cpp
    ${ZOMBIE_SOURCES}
)


pixel_double(pico3d)
disable_startup_logo(pico3d)
no_spritesheet(pico3d)
target_compile_definitions(pico3d PUBLIC ${GLOBAL_DEFINES})
target_compile_definitions(pico3d PUBLIC PICO_DIVIDER_IN_RAM=1)
target_link_libraries(pico3d pico_multicore)