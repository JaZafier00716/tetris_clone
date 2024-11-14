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
/* === End Define Constants === */

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
/* === End Define Common Structures === */

#endif