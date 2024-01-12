cmake_minimum_required(VERSION 3.10)

project(ir_laser_game)

# Define the module sources
set(SRCS
    ir_laser_game.cpp
    )

# Generate the firmware module
px4_add_module(
    MODULE modules__ir_laser_game
    MAIN ir_laser_game
    STACK_MAIN 2000
    SRCS ${SRCS}
)

# Link the module with the required libraries
target_link_libraries(modules__ir_laser_game
    PUBLIC
    # Add any additional libraries if needed
)
