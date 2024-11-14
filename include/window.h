#ifndef WINDOW_H
#define WINDOW_H
#include "header.h"
#include "objects.h"
#include "colors.h"
#include "movement.h"
#include "draw.h"


/* === Start window.c Functions === */
int SDL_rand(int max); // returns random number  <0,max>
void game_infinite_loop(SDL_Renderer *renderer, int window_width, int window_height);
void matrice_init(int matrice[FIELD_HEIGHT][FIELD_WIDTH]);
int game_window(); // returns 1 on error
int get_settings(TMovement *binds); // Returns 0 on error
char get_sdl_name(char *name);
/* === End window.c Functions === */


#endif