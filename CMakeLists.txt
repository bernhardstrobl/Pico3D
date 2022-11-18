picosystem_executable(
    pico3d
    main.cpp
)


pixel_double(pico3d)
disable_startup_logo(pico3d)
no_spritesheet(pico3d)
target_compile_definitions(pico3d PUBLIC PICO_DIVIDER_IN_RAM=1)