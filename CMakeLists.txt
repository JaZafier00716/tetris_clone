cmake_minimum_required(VERSION 3.10)
project(tetris C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# Run e.g. `cmake -S . -B build-asan -DTETRIS_SANITIZE=ON` to build with AddressSanitizer + UBSan
option(TETRIS_SANITIZE "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
if(TETRIS_SANITIZE)
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Game rules: no SDL, so it can be tested without a window
add_library(tetris_logic STATIC
  src/objects.c
  src/movement.c
  src/game.c
  src/scores.c)
target_include_directories(tetris_logic PUBLIC include)

# The game itself: window, drawing and input
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

# Key bindings: uses SDL key names, but needs no window
add_library(tetris_config STATIC src/config.c)
target_include_directories(tetris_config PUBLIC include)
target_link_libraries(tetris_config PUBLIC tetris_logic SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image)

add_executable(tetris
  src/main.c
  src/window.c
  src/draw.c
  src/colors.c)
target_link_libraries(tetris PRIVATE tetris_config)
# Assets (icons, font, config, scores) are found relative to the repository, whatever the working directory
target_compile_definitions(tetris PRIVATE ASSET_DIR="${CMAKE_SOURCE_DIR}")

enable_testing()
add_subdirectory(tests)
