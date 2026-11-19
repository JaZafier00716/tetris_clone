#ifndef DRAW_H
#define DRAW_H
#include "header.h"

/* === Start draw.c Functions === */
void draw_background(SDL_Renderer *renderer, SDL_Color color);
void draw_rectangle(SDL_Renderer *renderer, SDL_FPoint move, TColor color, int width, int height);
void draw_playing_field(SDL_Renderer *renderer, int matrice[FIELD_HEIGHT][FIELD_WIDTH], SDL_FPoint pos);
void draw_object_matrice(SDL_Renderer *renderer, SDL_FPoint offset, const TObject *object);
void draw_object_box(SDL_Renderer *renderer, SDL_Color color, const SDL_FRect box, const TObject *object, const char *text, TTF_Font *font); // object may be NULL for an empty box
void draw_text_box(SDL_Renderer *renderer, TTF_Font *title_font, TTF_Font *data_font, TDataText text[], int text_num, SDL_FRect rect);
SDL_Texture *load_icon(SDL_Renderer *renderer, const char *file_path);                                 // NULL on failure; free with SDL_DestroyTexture
int draw_icon(SDL_Renderer *renderer, SDL_FRect rect, SDL_Texture *icon);                              // returns 0 when there is no icon
int draw_text(SDL_Renderer *renderer, SDL_FRect rect, const char *text, TTF_Font *font, SDL_Color color, bool center); // renders text either on left or on center
void draw_icon_text(SDL_Renderer *renderer, SDL_FRect rect, TIconText text, TTF_Font *font, TTF_Font *long_text_font, SDL_Color color);
void draw_icon_text_block(SDL_Renderer *renderer, SDL_FRect rect, TIconText texts[], int texts_num, TTF_Font *font, TTF_Font *long_text_font, SDL_Color color);
SDL_FRect draw_button(SDL_Renderer *renderer, SDL_FPoint pos, TColor button_color, SDL_Color text_color, TTF_Font *font, const char *button_text); // returns the button rect (all zero when it could not be drawn)
void draw_title_texts(SDL_Renderer *renderer, SDL_FRect rect, TTF_Font *title_font, TTF_Font *texts_font, int texts_num, const char *title, const char *const *texts);
void draw_title_config_box(SDL_Renderer *renderer, SDL_FRect rect, TTF_Font *title_font, TTF_Font *texts_font, const char *title, const char *const *texts, TConfigButton *config_buttons, int text_num);
void draw_edit_block(SDL_Renderer *renderer, SDL_FRect rect, TTF_Font *data_font, const char *title_text, const char *pressed_key, SDL_FRect *buttons);
/* === End draw.c Functions === */

#endif
