# Tetris clone

A small Tetris game written in C with SDL2 (SDL2_image and SDL2_ttf for icons and text).

## Prerequisites

A C compiler, CMake 3.10+ and the SDL2 development packages. On Debian/Ubuntu:

```sh
sudo apt update && sudo apt install -y build-essential cmake pkg-config \
    libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

## Build, run, test

```sh
cmake -S . -B build
cmake --build build -j
./build/bin/tetris          # works from any directory, assets are found via the repository path
cd build && ctest --output-on-failure
```

To hunt for memory errors, build with AddressSanitizer and UBSan in a separate directory:

```sh
cmake -S . -B build-asan -DTETRIS_SANITIZE=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-asan -j && (cd build-asan && ctest --output-on-failure)
```

## Controls

Default keys (change them in the SETTINGS box of the main menu, they are stored in `data/cfg/user.cfg`):

| Key | Action |
| --- | --- |
| A / D | move left / right |
| S | soft drop (1 point per row) |
| Left / Right arrow | rotate counter clockwise / clockwise |
| C | hold the piece (once per piece) |
| Esc or the pause icon | pause |

Scoring is the original Nintendo one (40 / 100 / 300 / 1200 points for 1 / 2 / 3 / 4 rows, times level + 1).
The level goes up every 10 rows. The five best scores are kept in `data/best_score.txt`.

## Project structure

- `src/` and `include/`
  - Game logic, independent of SDL (and unit tested):
    - `objects.c`: the seven pieces and their rotation.
    - `movement.c`: the playing field, collisions, wall kicks and clearing rows.
    - `game.c`: game state and rules (spawning, hold, gravity, locking, score, level).
    - `scores.c`: best score list stored in a text file.
  - SDL part:
    - `window.c`: windows, main menu, game screen and input handling.
    - `draw.c`, `colors.c`: rendering helpers and colors.
    - `config.c`: key bindings and their config file.
    - `main.c`: entry point.
- `tests/`: unit tests of the logic and of the key binding file, run by CTest (tiny harness in `tests/test.h`, no framework).
- `data/`: font and runtime config/score files. `public/`: icons.
