#ifndef HEADER_H
#define HEADER_H

/* === Start Include System Libraries === */
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* === End Include System Libraries === */

#include "game.h"
#include "scores.h"

/* === Start Define Constants === */
#define SQUARE_SIZE 40
#define SHADE_WIDTH 5
#define SPACING_WIDTH 5
#define TITLE_SIZE 64
#define TEXT_SIZE 32
#define SETTINGS_ROW_SIZE 50
#define WORD_SIZE 20
#define ICON_SIZE 48
#define BINDS_NUM 6
/* === End Define Constants === */

/* === Start Define asset paths === */
// ASSET_DIR is the repository root, set by CMake. Without it paths are relative to the working directory.
#ifndef ASSET_DIR
#define ASSET_DIR "."
#endif
#define SCORE_FILE ASSET_DIR "/data/best_score.txt"
#define ARROW_DOWN ASSET_DIR "/public/icons/arrow_down.png"
#define ARROW_LEFT ASSET_DIR "/public/icons/arrow_left.png"
#define ARROW_RIGHT ASSET_DIR "/public/icons/arrow_right.png"
#define HOLD ASSET_DIR "/public/icons/hold.png"
#define ROTATE_CC ASSET_DIR "/public/icons/rotate_cc.png"
#define ROTATE_CW ASSET_DIR "/public/icons/rotate_cw.png"
#define PAUSE ASSET_DIR "/public/icons/pause.png"
#define PLAY ASSET_DIR "/public/icons/play.png"
#define FONT ASSET_DIR "/data/fonts/ProggyCleanNerdFontMono-Regular.ttf"
#define CFG_FILE ASSET_DIR "/data/cfg/user.cfg"
#define TMP_FILE ASSET_DIR "/data/cfg/tmp.cfg"
/* === End Define asset paths === */

/* === Start Define Texts ===*/
#define GAME_OVER_TEXT "GAME OVER"
#define NEXT_BOX_TEXT "NEXT"
#define HOLD_BOX_TEXT "HOLD"
#define SCORE_TEXT "SCORE"
#define LINES_TEXT "LINES"
#define LEVEL_TEXT "LEVEL"
#define MAIN_MENU "MAIN MENU"
#define RESTART "RESTART"
#define RESUME "RESUME"
#define NO_SCORE "NO BEST SCORES"
#define BEST_SCORES "BEST SCORES"
#define START "START"
#define CONFIG "SETTINGS"
#define EDIT "EDIT"
#define NEW_BIND "PRESS NEW KEY"
/* === End Define Texts ===*/

/* === Start Define Common Structures === */
typedef struct
{
  SDL_Color primary;
  SDL_Color secondary;
} TColor;

typedef struct
{
  const char *title;
  const char *data;
} TDataText;

typedef struct
{
  SDL_FRect button_pos;
  char button_name[WORD_SIZE]; // name of the action the button edits, e.g. "move_left"
} TConfigButton;

typedef struct
{
  const char *name;      // action name, shown when the icon is missing
  char key_name[WORD_SIZE];
  SDL_Texture *icon;     // may be NULL
} TIconText;
/* === End Define Common Structures === */

#endif
