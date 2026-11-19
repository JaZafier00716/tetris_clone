#ifndef MOVEMENT_H
#define MOVEMENT_H
#include <stdbool.h>
#include "objects.h"

/* The playing field: a matrice where the outermost rows/columns are a solid border (1),
   the inside is empty (0) or filled by locked squares (1). Pure logic, no SDL. */
#define FIELD_WIDTH 12
#define FIELD_HEIGHT 22

/* === Start movement.c Functions === */
void matrice_init(int matrice[FIELD_HEIGHT][FIELD_WIDTH]);                                          // empty field with border
bool object_collision_detection(const TObject *object, int matrice[FIELD_HEIGHT][FIELD_WIDTH]);    // true on collision; squares outside the field collide
bool try_move_object(TObject *object, int matrice[FIELD_HEIGHT][FIELD_WIDTH], TPoint new_pos);     // false on fail
bool try_rotate_object(TObject *object, int matrice[FIELD_HEIGHT][FIELD_WIDTH], int rotation);      // rotation: 1 = clockwise, -1 = counter clockwise; tries wall kicks; false on fail
void solidify(const TObject *object, int matrice[FIELD_HEIGHT][FIELD_WIDTH]);                       // writes the object into the field
int remove_full_rows(int matrice[FIELD_HEIGHT][FIELD_WIDTH]);                                       // returns number of removed rows
/* === End movement.c Functions === */

#endif
