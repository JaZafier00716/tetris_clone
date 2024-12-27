## Project Structure

- **src/**: This is where all the source code (.c files) is located.
  - \`colors.c\`: This file contains all colors used in the project.
  - \`draw.c\`: This file is contains all drawing functions. 
  - \`window.c\`: This file contains functions for individual windows.
  - \`main.c\`: This is the main file of the project.
  - \`movement.c\`: This file containts functions for object movement
  - \`objects.c\`: This file contains all object definitions
  - \`images.c\`: This file contains all functions that work with images

- **include/**: This folder contains header files. It includes \`header.h\` which is used for connection of source files.

- **build/**: This is where the compiled project will be located. after compilation is finished just run \`./tetris\` to start the game. 

- **data/**: This folder is used data storage, like best scores or user settings.

- **public/**: This folder is used for audio and image files.

- **CMakeLists.txt**: This file configures CMake for the project and specifies the build process.


### Prerequisites

Ensure that you have the following tools installed:
- **SDL2**: You need the SDL2, SDL2_image, SDL2_TTF, SDL2_mixer and SDL2_sound libraries installed on your system. You can install it using your package manager.
  - For Ubuntu/Debian:
    \`sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-sound-dev\`
  - For Arch Linux:
    \`sudo pacman -S sdl2 sdl2_image sdl2_ttf sdl2_mixer sdl2_sound\`
  - For Fedora:
    \`sudo dnf install SDL2 SDL2_image SDL2_ttf SDL2_mixer SDL2_sound\`

- **CMake**: Make sure CMake is installed. You can install it via your package manager.
  - For Ubuntu/Debian:
    \`sudo apt install cmake\`
  - For Arch Linux:
    \`sudo pacman -S cmake\`
  - For Fedora:
    \`sudo dnf install cmake\`


### Step-by-Step Compilation Instructions

1. **Navigate to the project's build directory**:
    \`
    #cd tetris/build
    \`

2. **Run the build command**:
    \`
    #cmake .. && make && ./tetris
    \`

   This command will:
   - Run \`cmake ..\` to configure the build process.
   - Compile the project using \`make\`.
   - Run the generated executable file.
