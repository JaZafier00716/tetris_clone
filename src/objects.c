#include "objects.h"

static const int shapes[PIECE_COUNT][OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE] = {
    [PIECE_I] = {{0, 0, 0, 0},
                 {1, 1, 1, 1},
                 {0, 0, 0, 0},
                 {0, 0, 0, 0}},
    [PIECE_J] = {{1, 0, 0, 0},
                 {1, 1, 1, 0},
                 {0, 0, 0, 0},
                 {0, 0, 0, 0}},
    [PIECE_L] = {{0, 0, 1, 0},
                 {1, 1, 1, 0},
                 {0, 0, 0, 0},
                 {0, 0, 0, 0}},
    [PIECE_O] = {{1, 1, 0, 0},
                 {1, 1, 0, 0},
                 {0, 0, 0, 0},
                 {0, 0, 0, 0}},
    [PIECE_S] = {{0, 1, 1, 0},
                 {1, 1, 0, 0},
                 {0, 0, 0, 0},
                 {0, 0, 0, 0}},
    [PIECE_T] = {{0, 1, 0, 0},
                 {1, 1, 1, 0},
                 {0, 0, 0, 0},
                 {0, 0, 0, 0}},
    [PIECE_Z] = {{1, 1, 0, 0},
                 {0, 1, 1, 0},
                 {0, 0, 0, 0},
                 {0, 0, 0, 0}},
};

// Side of the square box the piece rotates in
static const int box_sizes[PIECE_COUNT] = {4, 3, 3, 2, 3, 3, 3};

TObject object_create(PieceType type)
{
  TObject object = {.type = type};
  for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
  {
    for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
    {
      object.matrice[i][j] = shapes[type][i][j];
    }
  }
  return object;
}

void rotate_CW(TObject *object)
{
  const int n = box_sizes[object->type];
  int rotated[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE] = {{0}};

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      rotated[j][n - 1 - i] = object->matrice[i][j];
    }
  }
  for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
  {
    for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
    {
      object->matrice[i][j] = rotated[i][j];
    }
  }
}

void rotate_CC(TObject *object)
{
  // Three clockwise quarter turns are one counter clockwise turn
  rotate_CW(object);
  rotate_CW(object);
  rotate_CW(object);
}

TBounds object_bounds(const TObject *object)
{
  TBounds bounds = {OBJECT_MATRICE_SIZE, -1, OBJECT_MATRICE_SIZE, -1};
  for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
  {
    for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
    {
      if (!object->matrice[i][j])
      {
        continue;
      }
      if (i < bounds.row_min) bounds.row_min = i;
      if (i > bounds.row_max) bounds.row_max = i;
      if (j < bounds.col_min) bounds.col_min = j;
      if (j > bounds.col_max) bounds.col_max = j;
    }
  }
  return bounds;
}
