#ifndef MOVEMENT_H
#define MOVEMENT_H
#include "header.h"
#include "objects.h"

/* === Start Define Structures for Playing Field Matrice === */
// typedef struct
// {
//   bool exists;  // whether node exists or if it is empty (background)
//   bool moving;  // Moving state of node - useful only if node exists
//   TColor color; // Color of node
// } TNode;
/* === End Define Structures for Playing Field Matrice === */

/* === Start movement.c Functions === */
void solidify(TObject object, int matrice[FIELD_HEIGHT][FIELD_WIDTH]);
int object_collision_detection(TObject object, int matrice[FIELD_HEIGHT][FIELD_WIDTH]);           // returns 1 on collision
int try_move_object(TObject *object, int matrice[FIELD_HEIGHT][FIELD_WIDTH], SDL_Point new_pos);  // returns 0 on fail
int try_rotate_object(TObject *object, int matrice[FIELD_HEIGHT][FIELD_WIDTH], int rotation);     // returns 0 on fail
void transpose(int matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE]);                              // Transposes object matrice
void reverse(int matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE]);                                // Reverses object matrice 
int remove_full_rows(int matrice[FIELD_HEIGHT][FIELD_WIDTH]);                                     // returns number of removed rows
void rotate_UP(TObject *object);                                                                    // Rotates object to face upwards
void rotate_CC(TObject *object);                                                                    // Rotates object Counter Clockwise
void rotate_CW(TObject *object);                                                                    // Rotates object Clockwise
/* === End movement.c Functions === */


#endif