#ifndef HEADER_H
#define HEADER_H

/* === Start Include System Libraries === */
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
/* === End Include System Libraries === */

/* === Start Define Constants === */
#define FIELD_WIDTH 12
#define FIELD_HEIGHT 22
#define SQUARE_SIZE 40
#define SHADE_WIDTH 5
#define SPACING_WIDTH 5
#define TITLE_SIZE 64
#define TEXT_SIZE 32
#define OBJECT_MATRICE_SIZE 4
#define SETTINGS_ROW_SIZE 50
#define WORD_SIZE 20
#define ICON_SIZE 48
#define BINDS_NUM 6
#define BEST_SCORE_NUM 5
/* === End Define Constants === */

/* === Start Define asset paths === */
#define SCORE_FILE "../data/best_score.txt"
#define ARROW_DOWN "../public/icons/arrow_down.png"
#define ARROW_LEFT "../public/icons/arrow_left.png"
#define ARROW_RIGHT "../public/icons/arrow_right.png"
#define COG "../public/icons/cog.png"
#define HOLD "../public/icons/hold.png"
#define ROTATE_CC "../public/icons/rotate_cc.png"
#define ROTATE_CW "../public/icons/rotate_cw.png"
#define VOLUME_MUTE "../public/icons/volume_mute.png"
#define VOLUME_ON "../public/icons/volume_on.png"
#define PAUSE "../public/icons/pause.png"
#define PLAY "../public/icons/play.png"
#define FONT "../data/fonts/ProggyCleanNerdFontMono-Regular.ttf"
#define CFG_FILE "../data/cfg/user.cfg"
#define TMP_FILE "../data/cfg/tmp.cfg"
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
  SDL_Surface text_surface;
  SDL_Texture *text_texture;
} TText;

typedef struct {
  char *title;
  char *data;
}TDataText;

typedef struct {
  SDL_FRect button_pos;
  char *button_name;
}TConfigButton;
/* === End Define Common Structures === */


#endif