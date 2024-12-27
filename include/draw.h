#ifndef DRAW_H
#define DRAW_H
#include "header.h"
#include "objects.h"
#include "colors.h"


// TODO: Create separate function for drawing text in draw text box
// TODO: Create a function that draws Icon and text next to each other

/* === Start draw.c Functions === */
void draw_background(SDL_Renderer *renderer, SDL_Color color);
void draw_rectangle(SDL_Renderer *renderer, SDL_FPoint move, TColor color, int width, int height);
void draw_playing_field(SDL_Renderer *renderer, int matrice[FIELD_HEIGHT][FIELD_WIDTH], SDL_FPoint pos);
void draw_object_matrice(SDL_Renderer *renderer, SDL_FPoint offset, TObject object);
void draw_color_palette(SDL_Renderer *renderer);
void draw_object_box(SDL_Renderer *renderer, SDL_Color color, const SDL_FRect box, TObject object, char *text, TTF_Font *font);
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect);
void draw_text_box(SDL_Renderer *renderer, TTF_Font *title_font, TTF_Font *data_font, TDataText text[], int text_num, SDL_FRect rect);
int draw_icon(SDL_Renderer *renderer, SDL_FRect rect, char *file_path);
int draw_text(SDL_Renderer *renderer, SDL_FRect rect, char *text, TTF_Font *font, SDL_Color color, bool center); // renders text either on left or on center
void draw_icon_text(SDL_Renderer *renderer, SDL_FRect rect, TIconText text, TTF_Font *font, TTF_Font *long_text_font, SDL_Color color);
void draw_icon_text_block(SDL_Renderer *renderer, SDL_FRect rect, TIconText texts[], int texts_num, TTF_Font *font, TTF_Font *long_text_font, SDL_Color color);
SDL_FRect draw_button(SDL_Renderer *renderer, SDL_FPoint pos, TColor button_color, SDL_Color text_color, TTF_Font *font, char *button_text); // returns whether a button had been clicked
void draw_title_texts(SDL_Renderer *renderer, SDL_FRect rect, TTF_Font *title_font, TTF_Font *texts_font, int texts_num, char *title, char **texts);
void draw_title_config_box(SDL_Renderer *renderer, SDL_FRect rect, TTF_Font *title_font, TTF_Font *texts_font, char *title, char **texts, int text_num);
/* === End draw.c Functions === */


#endif