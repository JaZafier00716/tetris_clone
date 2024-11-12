cmake_minimum_required(VERSION 3.10)

# Project name
project(Tetris)

# Specify the C++ standard (or C standard if using C)
set(CMAKE_C_STANDARD 99)

# Add the executable target
add_executable(tetris src/main.c src/window.c src/draw.c src/colors.c src/objects.c src/movement.c)

# Specify output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories if needed
include_directories(include)

# Link any libraries if needed (e.g., SDL2)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
target_link_libraries(tetris SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_image)

set(CMAKE_BUILD_TYPE Debug)           # Set the build type to Debug
set(CMAKE_C_FLAGS_DEBUG "-g -O0")     # Add debugging symbols and disable optimizations
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")   # Same for C++ if your project has C++ files

