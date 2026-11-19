#ifndef OBJECTS_H
#define OBJECTS_H

/* Tetrominoes: shapes and rotation. Pure logic, no SDL. */

#define OBJECT_MATRICE_SIZE 4

typedef enum
{
  PIECE_I,
  PIECE_J,
  PIECE_L,
  PIECE_O,
  PIECE_S,
  PIECE_T,
  PIECE_Z,
  PIECE_COUNT
} PieceType;

typedef struct
{
  int x;
  int y;
} TPoint;

typedef struct
{
  int row_min, row_max; // rows/columns of the object matrice that contain squares
  int col_min, col_max;
} TBounds;

typedef struct
{
  int matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE];
  PieceType type;
  TPoint pos; // position of the matrice's top-left corner in the playing field
} TObject;

/* Creates a piece in its spawn orientation, with pos {0, 0}. */
TObject object_create(PieceType type);
/* Rotates the piece 90 degrees. The piece is rotated inside its own square box
   (4x4 for I, 2x2 for O, 3x3 otherwise), so four rotations restore the original. */
void rotate_CW(TObject *object);
void rotate_CC(TObject *object);
/* Smallest rectangle of the matrice that contains all squares of the piece. */
TBounds object_bounds(const TObject *object);

#endif
