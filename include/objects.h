#ifndef OBJECTS_H
#define OBJECTS_H
#include "header.h"
#include "colors.h"

/* === Start Object Definitions === */
enum direction
{
  UP,
  RIGHT,
  DOWN,
  LEFT
};

typedef struct {
  char name[WORD_SIZE];
  char bind[WORD_SIZE];
  char sdl_name;
}TBinds;

typedef struct {
  TBinds move_left,move_right, move_down, move_hold, rotate_left, rotate_right;
}TMovement;

typedef struct {
  TBinds text;
  char *icon_path;
}TIconText;

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


#endif