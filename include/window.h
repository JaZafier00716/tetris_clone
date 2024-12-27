#ifndef WINDOW_H
#define WINDOW_H
#include "header.h"
#include "objects.h"
#include "colors.h"
#include "movement.h"
#include "draw.h"

enum
{
  MENU,
  GAME,
  SETTINGS
};

enum
{
  move_left = 'a',
  move_right = 'd',
  move_down = 's',
  move_hold = 'c',
  rotate_right = 'O',
  rotate_left = 'P'
};


/* === Start window.c Functions === */
int SDL_rand(int max);                                                               // returns random number  <0,max>
int main_menu(SDL_Renderer *renderer, int window_width, int window_height);          // returns Next window that should be displayed, otherwise returns -1
int game_infinite_loop(SDL_Renderer *renderer, int window_width, int window_height); // returns next widow that should be displayed, otherwise returns -1
void matrice_init(int matrice[FIELD_HEIGHT][FIELD_WIDTH]);
int window_select();                // returns 1 on error
int get_settings(TMovement *binds); // Returns 0 on error
void update_settings(char *new_bind, char *movement);
void get_normalized_name(char name, char *normalized_name);
char get_sdl_name(char *normalized_name);
int update_best_scores(int score);
/* === End window.c Functions === */

#endif