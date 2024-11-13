#ifndef DRAW_H
#define DRAW_H
#include "header.h"
#include "objects.h"
#include "colors.h"

/* === Start draw.c Functions === */
void draw_background(SDL_Renderer *renderer, SDL_Color color);
void draw_rectangle(SDL_Renderer *renderer, SDL_FPoint move, TColor color);
void draw_playing_field(SDL_Renderer *renderer, int matrice[FIELD_HEIGHT][FIELD_WIDTH], SDL_FPoint pos);
void draw_object_matrice(SDL_Renderer *renderer, SDL_FPoint offset, TObject object);
void draw_color_palette(SDL_Renderer *renderer);
void draw_object_box(SDL_Renderer *renderer, SDL_Color color, const SDL_FRect box, TObject object, char *text, TTF_Font *font);
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect);
void draw_text_box(SDL_Renderer *renderer, TTF_Font *title_font, TTF_Font *data_font, TDataText text[], int text_num, SDL_FRect rect);
/* === End draw.c Functions === */


#endif