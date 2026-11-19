#include <stddef.h>
#include "movement.h"

static bool in_field(int x, int y)
{
  return x >= 0 && x < FIELD_WIDTH && y >= 0 && y < FIELD_HEIGHT;
}

void matrice_init(int matrice[FIELD_HEIGHT][FIELD_WIDTH])
{
  for (int i = 0; i < FIELD_HEIGHT; i++)
  {
    for (int j = 0; j < FIELD_WIDTH; j++)
    {
      bool border = i == 0 || i == FIELD_HEIGHT - 1 || j == 0 || j == FIELD_WIDTH - 1;
      matrice[i][j] = border ? 1 : 0;
    }
  }
}

bool object_collision_detection(const TObject *object, int matrice[FIELD_HEIGHT][FIELD_WIDTH])
{
  for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
  {
    for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
    {
      if (!object->matrice[i][j])
      {
        continue;
      }
      int x = object->pos.x + j;
      int y = object->pos.y + i;
      if (!in_field(x, y) || matrice[y][x])
      {
        return true;
      }
    }
  }
  return false;
}

bool try_move_object(TObject *object, int matrice[FIELD_HEIGHT][FIELD_WIDTH], TPoint new_pos)
{
  TObject moved = *object;
  moved.pos = new_pos;

  if (object_collision_detection(&moved, matrice))
  {
    return false;
  }
  *object = moved;
  return true;
}

bool try_rotate_object(TObject *object, int matrice[FIELD_HEIGHT][FIELD_WIDTH], int rotation)
{
  // Offsets tried in order when the rotated piece does not fit: in place, sideways, then up (off the floor)
  static const TPoint kicks[] = {{0, 0}, {-1, 0}, {1, 0}, {-2, 0}, {2, 0}, {0, -1}};
  TObject rotated = *object;

  if (rotation > 0)
  {
    rotate_CW(&rotated);
  }
  else
  {
    rotate_CC(&rotated);
  }

  for (size_t i = 0; i < sizeof(kicks) / sizeof(kicks[0]); i++)
  {
    TObject kicked = rotated;
    kicked.pos.x += kicks[i].x;
    kicked.pos.y += kicks[i].y;
    if (!object_collision_detection(&kicked, matrice))
    {
      *object = kicked;
      return true;
    }
  }
  return false;
}

void solidify(const TObject *object, int matrice[FIELD_HEIGHT][FIELD_WIDTH])
{
  for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
  {
    for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
    {
      int x = object->pos.x + j;
      int y = object->pos.y + i;
      if (object->matrice[i][j] && in_field(x, y))
      {
        matrice[y][x] = 1;
      }
    }
  }
}

static bool row_is_full(int matrice[FIELD_HEIGHT][FIELD_WIDTH], int row)
{
  for (int j = 1; j < FIELD_WIDTH - 1; j++)
  {
    if (!matrice[row][j])
    {
      return false;
    }
  }
  return true;
}

int remove_full_rows(int matrice[FIELD_HEIGHT][FIELD_WIDTH])
{
  int removed = 0;
  int dst = FIELD_HEIGHT - 2; // row that receives the next kept row

  // Walk the inside rows bottom-up, keeping the rows that are not full
  for (int src = FIELD_HEIGHT - 2; src >= 1; src--)
  {
    if (row_is_full(matrice, src))
    {
      removed++;
      continue;
    }
    if (dst != src)
    {
      for (int j = 0; j < FIELD_WIDTH; j++)
      {
        matrice[dst][j] = matrice[src][j];
      }
    }
    dst--;
  }
  // Rows freed at the top become empty
  for (int i = dst; i >= 1; i--)
  {
    for (int j = 1; j < FIELD_WIDTH - 1; j++)
    {
      matrice[i][j] = 0;
    }
  }
  return removed;
}
