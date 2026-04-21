### Project Structure

- **src/**: C source files.
  - `colors.c`: Color definitions used by rendering code.
  - `draw.c`: Drawing and rendering helpers.
  - `main.c`: Program entry point.
  - `movement.c`: Piece movement logic.
  - `objects.c`: Game object and tetromino definitions.
  - `window.c`: Window and UI flow logic.

- **include/**: Header files for declarations.
  - `colors.h`, `draw.h`, `header.h`, `movement.h`, `objects.h`, `window.h`

- **data/**: Runtime data/config files.
- **public/**: Image/icon assets.
- **CMakeLists.txt**: Build configuration.

### Prerequisites

Based on the current `CMakeLists.txt`, required libraries are:
- SDL2
- SDL2_image
- SDL2_ttf

Debian/Ubuntu install command:

`sudo apt update && sudo apt install -y build-essential cmake pkg-config libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev`

Optional packages (only if you later add code that uses them):
- `libsdl2-mixer-dev`
- `libsdl2-sound-dev`

### Build and Run (CMake)

From the repository root (`tetris_clone`):

1. Configure the project:

`cmake -S . -B build`

2. Build the executable:

`cmake --build build -j`

3. Run the game:

`./build/bin/tetris`

Note: the binary is placed in `build/bin/` because `CMAKE_RUNTIME_OUTPUT_DIRECTORY` is set in `CMakeLists.txt`.
