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
#define TEXT_SIZE 64
#define OBJECT_MATRICE_SIZE 4
#define SETTINGS_ROW_SIZE 50
/* === End Define Constants === */

/* === Start Color Definitions === */
typedef struct
{
  SDL_Color primary;
  SDL_Color secondary;
} TColor;

extern TColor light;
extern TColor dark;
extern TColor red;
extern TColor orange;
extern TColor yellow;
extern TColor green;
extern TColor cyan;
extern TColor blue;
extern TColor purple;

extern SDL_Color black;
extern SDL_Color white;
/* === End Color Definitions === */

/* === Start Object Definitions === */
enum direction
{
  UP,
  RIGHT,
  DOWN,
  LEFT
};

typedef struct
{
  SDL_Surface text_surface;
  SDL_Texture *text_texture;
}TText;
typedef struct {
  char *title;
  char *data;
}TDataText;


typedef struct {
  TDataText move_left,move_right, move_down, move_hold, rotate_left, rotate_right;
}TMovement;

typedef struct
{
  int matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE];
  TColor *color;
  SDL_Point pos;
  enum direction rotation;
  int w;
  int h;
} TObject;

extern TObject empty_object;
extern TObject object_I;
extern TObject object_J;
extern TObject object_L;
extern TObject object_O;
extern TObject object_S;
extern TObject object_T;
extern TObject object_Z;
/* === End Object Definitions === */

/* === Start Define Structures for Playing Field Matrice === */
typedef struct
{
  bool exists;  // whether node exists or if it is empty (background)
  bool moving;  // Moving state of node - useful only if node exists
  TColor color; // Color of node
} TNode;
/* === End Define Structures for Playing Field Matrice === */


/* === Start Init User Functions === */

/* === Start images.c Functions === */
static void SetSDLIcon(SDL_Window* window);
/* === End images.c Functions === */


/* === Start movement.c Functions === */
void solidify(TObject object, TNode matrice[FIELD_HEIGHT][FIELD_WIDTH]);
int object_collision_detection(TObject object, TNode matrice[FIELD_HEIGHT][FIELD_WIDTH]);           // returns 1 on collision
int try_move_object(TObject *object, TNode matrice[FIELD_HEIGHT][FIELD_WIDTH], SDL_Point new_pos);  // returns 0 on fail
int try_rotate_object(TObject *object, TNode matrice[FIELD_HEIGHT][FIELD_WIDTH], int rotation);     // returns 0 on fail
void transpose(int matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE]);                              // Transposes object matrice
void reverse(int matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE]);                                // Reverses object matrice 
int remove_full_rows(TNode matrice[FIELD_HEIGHT][FIELD_WIDTH]);                                     // returns number of removed rows
void rotate_UP(TObject *object);                                                                    // Rotates object to face upwards
void rotate_CC(TObject *object);                                                                    // Rotates object Counter Clockwise
void rotate_CW(TObject *object);                                                                    // Rotates object Clockwise
/* === End movement.c Functions === */

/* === Start window.c Functions === */
int SDL_rand(int max); // returns random number  <0,max>
void game_infinite_loop(SDL_Renderer *renderer, int window_width, int window_height, SDL_FRect hold_box, SDL_FRect next_box, SDL_FRect matrice_box, SDL_FRect score_box);
void matrice_init(TNode matrice[FIELD_HEIGHT][FIELD_WIDTH]);
int game_window(); // returns 1 on error
int get_settings(TMovement *binds); // Returns 0 on error
/* === End window.c Functions === */

/* === Start draw.c Functions === */
void draw_background(SDL_Renderer *renderer, SDL_Color color);
void draw_rectangle(SDL_Renderer *renderer, SDL_FPoint move, TColor color);
void draw_playing_field(SDL_Renderer *renderer, TNode matrice[FIELD_HEIGHT][FIELD_WIDTH], SDL_FPoint pos);
void draw_object_matrice(SDL_Renderer *renderer, SDL_FPoint offset, TObject object);
void draw_color_palette(SDL_Renderer *renderer);
void draw_object_box(SDL_Renderer *renderer, SDL_Color color, const SDL_FRect box, TObject object, char *text, TTF_Font *font);
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect);
void draw_text_box(SDL_Renderer *renderer, TTF_Font *title_font, TTF_Font *data_font, TDataText text[], int text_num, SDL_FRect rect);
/* === End draw.c Functions === */
/* === End User Functions === */

#endif