cmake_minimum_required(VERSION 3.12)

# Pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico32 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Find the PicoSystem SDK
find_package(PICOSYSTEM REQUIRED)

picosystem_executable(
    pico3d
    main.cpp
)


pixel_double(pico3d)
disable_startup_logo(pico3d)
no_spritesheet(pico3d)
target_compile_definitions(pico3d PUBLIC PICO_DIVIDER_IN_RAM=1)
target_link_libraries(pico3d pico_multicore)