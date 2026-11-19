#ifndef WINDOW_H
#define WINDOW_H
#include "header.h"

/* Values returned by the screen functions: which screen to show next */
enum
{
  SCREEN_ERROR = -2,
  SCREEN_QUIT = -1,
  MENU,
  GAME
};

/* === Start window.c Functions === */
int window_select(void);                                                             // runs the menu and the game; returns 0 on success, 1 on error
int main_menu(SDL_Renderer *renderer, int window_width, int window_height);          // size = design size (the renderer scales it to the window); returns the next screen
int game_infinite_loop(SDL_Renderer *renderer, int window_width, int window_height); // returns the next screen to display
/* === End window.c Functions === */

#endif
