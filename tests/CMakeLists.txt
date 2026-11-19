# Unit tests. Plain C, no framework: each test is a program that returns 0 on success.
# The game logic tests do not need SDL.
foreach(name test_objects test_movement test_game test_scores test_layout)
  add_executable(${name} ${name}.c)
  target_link_libraries(${name} PRIVATE tetris_logic)
  add_test(NAME ${name} COMMAND ${name})
endforeach()

# Key bindings (needs the SDL headers, not a window)
add_executable(test_config test_config.c)
target_link_libraries(test_config PRIVATE tetris_config)
add_test(NAME test_config COMMAND test_config)

# Scaling of the design resolution to different window sizes: dummy video driver, no real window
add_executable(test_scaling test_scaling.c)
target_link_libraries(test_scaling PRIVATE tetris_ui m)
target_compile_definitions(test_scaling PRIVATE ASSET_DIR="${CMAKE_SOURCE_DIR}")
add_test(NAME test_scaling COMMAND test_scaling)
set_tests_properties(test_scaling PROPERTIES ENVIRONMENT "SDL_VIDEODRIVER=dummy;SDL_AUDIODRIVER=dummy")
