# Unit tests. Plain C, no framework: each test is a program that returns 0 on success.
# The game logic tests do not need SDL.
foreach(name test_objects test_movement test_game test_scores)
  add_executable(${name} ${name}.c)
  target_link_libraries(${name} PRIVATE tetris_logic)
  add_test(NAME ${name} COMMAND ${name})
endforeach()

# Key bindings (needs the SDL headers, not a window)
add_executable(test_config test_config.c)
target_link_libraries(test_config PRIVATE tetris_config)
add_test(NAME test_config COMMAND test_config)
