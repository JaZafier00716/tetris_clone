# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jan/Documents/School/UPR/v2_tetris

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jan/Documents/School/UPR/v2_tetris/build

# Include any dependencies generated for this target.
include CMakeFiles/tetris.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/tetris.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/tetris.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tetris.dir/flags.make

CMakeFiles/tetris.dir/codegen:
.PHONY : CMakeFiles/tetris.dir/codegen

CMakeFiles/tetris.dir/src/main.c.o: CMakeFiles/tetris.dir/flags.make
CMakeFiles/tetris.dir/src/main.c.o: /home/jan/Documents/School/UPR/v2_tetris/src/main.c
CMakeFiles/tetris.dir/src/main.c.o: CMakeFiles/tetris.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jan/Documents/School/UPR/v2_tetris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/tetris.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/tetris.dir/src/main.c.o -MF CMakeFiles/tetris.dir/src/main.c.o.d -o CMakeFiles/tetris.dir/src/main.c.o -c /home/jan/Documents/School/UPR/v2_tetris/src/main.c

CMakeFiles/tetris.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/tetris.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jan/Documents/School/UPR/v2_tetris/src/main.c > CMakeFiles/tetris.dir/src/main.c.i

CMakeFiles/tetris.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/tetris.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jan/Documents/School/UPR/v2_tetris/src/main.c -o CMakeFiles/tetris.dir/src/main.c.s

CMakeFiles/tetris.dir/src/window.c.o: CMakeFiles/tetris.dir/flags.make
CMakeFiles/tetris.dir/src/window.c.o: /home/jan/Documents/School/UPR/v2_tetris/src/window.c
CMakeFiles/tetris.dir/src/window.c.o: CMakeFiles/tetris.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jan/Documents/School/UPR/v2_tetris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/tetris.dir/src/window.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/tetris.dir/src/window.c.o -MF CMakeFiles/tetris.dir/src/window.c.o.d -o CMakeFiles/tetris.dir/src/window.c.o -c /home/jan/Documents/School/UPR/v2_tetris/src/window.c

CMakeFiles/tetris.dir/src/window.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/tetris.dir/src/window.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jan/Documents/School/UPR/v2_tetris/src/window.c > CMakeFiles/tetris.dir/src/window.c.i

CMakeFiles/tetris.dir/src/window.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/tetris.dir/src/window.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jan/Documents/School/UPR/v2_tetris/src/window.c -o CMakeFiles/tetris.dir/src/window.c.s

CMakeFiles/tetris.dir/src/draw.c.o: CMakeFiles/tetris.dir/flags.make
CMakeFiles/tetris.dir/src/draw.c.o: /home/jan/Documents/School/UPR/v2_tetris/src/draw.c
CMakeFiles/tetris.dir/src/draw.c.o: CMakeFiles/tetris.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jan/Documents/School/UPR/v2_tetris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/tetris.dir/src/draw.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/tetris.dir/src/draw.c.o -MF CMakeFiles/tetris.dir/src/draw.c.o.d -o CMakeFiles/tetris.dir/src/draw.c.o -c /home/jan/Documents/School/UPR/v2_tetris/src/draw.c

CMakeFiles/tetris.dir/src/draw.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/tetris.dir/src/draw.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jan/Documents/School/UPR/v2_tetris/src/draw.c > CMakeFiles/tetris.dir/src/draw.c.i

CMakeFiles/tetris.dir/src/draw.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/tetris.dir/src/draw.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jan/Documents/School/UPR/v2_tetris/src/draw.c -o CMakeFiles/tetris.dir/src/draw.c.s

CMakeFiles/tetris.dir/src/colors.c.o: CMakeFiles/tetris.dir/flags.make
CMakeFiles/tetris.dir/src/colors.c.o: /home/jan/Documents/School/UPR/v2_tetris/src/colors.c
CMakeFiles/tetris.dir/src/colors.c.o: CMakeFiles/tetris.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jan/Documents/School/UPR/v2_tetris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/tetris.dir/src/colors.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/tetris.dir/src/colors.c.o -MF CMakeFiles/tetris.dir/src/colors.c.o.d -o CMakeFiles/tetris.dir/src/colors.c.o -c /home/jan/Documents/School/UPR/v2_tetris/src/colors.c

CMakeFiles/tetris.dir/src/colors.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/tetris.dir/src/colors.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jan/Documents/School/UPR/v2_tetris/src/colors.c > CMakeFiles/tetris.dir/src/colors.c.i

CMakeFiles/tetris.dir/src/colors.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/tetris.dir/src/colors.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jan/Documents/School/UPR/v2_tetris/src/colors.c -o CMakeFiles/tetris.dir/src/colors.c.s

CMakeFiles/tetris.dir/src/objects.c.o: CMakeFiles/tetris.dir/flags.make
CMakeFiles/tetris.dir/src/objects.c.o: /home/jan/Documents/School/UPR/v2_tetris/src/objects.c
CMakeFiles/tetris.dir/src/objects.c.o: CMakeFiles/tetris.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jan/Documents/School/UPR/v2_tetris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/tetris.dir/src/objects.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/tetris.dir/src/objects.c.o -MF CMakeFiles/tetris.dir/src/objects.c.o.d -o CMakeFiles/tetris.dir/src/objects.c.o -c /home/jan/Documents/School/UPR/v2_tetris/src/objects.c

CMakeFiles/tetris.dir/src/objects.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/tetris.dir/src/objects.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jan/Documents/School/UPR/v2_tetris/src/objects.c > CMakeFiles/tetris.dir/src/objects.c.i

CMakeFiles/tetris.dir/src/objects.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/tetris.dir/src/objects.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jan/Documents/School/UPR/v2_tetris/src/objects.c -o CMakeFiles/tetris.dir/src/objects.c.s

CMakeFiles/tetris.dir/src/movement.c.o: CMakeFiles/tetris.dir/flags.make
CMakeFiles/tetris.dir/src/movement.c.o: /home/jan/Documents/School/UPR/v2_tetris/src/movement.c
CMakeFiles/tetris.dir/src/movement.c.o: CMakeFiles/tetris.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jan/Documents/School/UPR/v2_tetris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/tetris.dir/src/movement.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/tetris.dir/src/movement.c.o -MF CMakeFiles/tetris.dir/src/movement.c.o.d -o CMakeFiles/tetris.dir/src/movement.c.o -c /home/jan/Documents/School/UPR/v2_tetris/src/movement.c

CMakeFiles/tetris.dir/src/movement.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/tetris.dir/src/movement.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jan/Documents/School/UPR/v2_tetris/src/movement.c > CMakeFiles/tetris.dir/src/movement.c.i

CMakeFiles/tetris.dir/src/movement.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/tetris.dir/src/movement.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jan/Documents/School/UPR/v2_tetris/src/movement.c -o CMakeFiles/tetris.dir/src/movement.c.s

CMakeFiles/tetris.dir/src/images.c.o: CMakeFiles/tetris.dir/flags.make
CMakeFiles/tetris.dir/src/images.c.o: /home/jan/Documents/School/UPR/v2_tetris/src/images.c
CMakeFiles/tetris.dir/src/images.c.o: CMakeFiles/tetris.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jan/Documents/School/UPR/v2_tetris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/tetris.dir/src/images.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/tetris.dir/src/images.c.o -MF CMakeFiles/tetris.dir/src/images.c.o.d -o CMakeFiles/tetris.dir/src/images.c.o -c /home/jan/Documents/School/UPR/v2_tetris/src/images.c

CMakeFiles/tetris.dir/src/images.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/tetris.dir/src/images.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jan/Documents/School/UPR/v2_tetris/src/images.c > CMakeFiles/tetris.dir/src/images.c.i

CMakeFiles/tetris.dir/src/images.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/tetris.dir/src/images.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jan/Documents/School/UPR/v2_tetris/src/images.c -o CMakeFiles/tetris.dir/src/images.c.s

# Object files for target tetris
tetris_OBJECTS = \
"CMakeFiles/tetris.dir/src/main.c.o" \
"CMakeFiles/tetris.dir/src/window.c.o" \
"CMakeFiles/tetris.dir/src/draw.c.o" \
"CMakeFiles/tetris.dir/src/colors.c.o" \
"CMakeFiles/tetris.dir/src/objects.c.o" \
"CMakeFiles/tetris.dir/src/movement.c.o" \
"CMakeFiles/tetris.dir/src/images.c.o"

# External object files for target tetris
tetris_EXTERNAL_OBJECTS =

tetris: CMakeFiles/tetris.dir/src/main.c.o
tetris: CMakeFiles/tetris.dir/src/window.c.o
tetris: CMakeFiles/tetris.dir/src/draw.c.o
tetris: CMakeFiles/tetris.dir/src/colors.c.o
tetris: CMakeFiles/tetris.dir/src/objects.c.o
tetris: CMakeFiles/tetris.dir/src/movement.c.o
tetris: CMakeFiles/tetris.dir/src/images.c.o
tetris: CMakeFiles/tetris.dir/build.make
tetris: CMakeFiles/tetris.dir/compiler_depend.ts
tetris: /usr/lib/libSDL2-2.0.so.0.3000.9
tetris: /usr/lib/libSDL2_ttf.so
tetris: CMakeFiles/tetris.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/jan/Documents/School/UPR/v2_tetris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C executable tetris"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tetris.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tetris.dir/build: tetris
.PHONY : CMakeFiles/tetris.dir/build

CMakeFiles/tetris.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tetris.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tetris.dir/clean

CMakeFiles/tetris.dir/depend:
	cd /home/jan/Documents/School/UPR/v2_tetris/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jan/Documents/School/UPR/v2_tetris /home/jan/Documents/School/UPR/v2_tetris /home/jan/Documents/School/UPR/v2_tetris/build /home/jan/Documents/School/UPR/v2_tetris/build /home/jan/Documents/School/UPR/v2_tetris/build/CMakeFiles/tetris.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/tetris.dir/depend

