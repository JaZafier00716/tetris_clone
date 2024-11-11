#include "header.h"

void transpose(int matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE])
{
  int new_matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE];
  for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
  {
    for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
    {
      new_matrice[i][j] = matrice[j][i];
    }
  }
  memcpy(matrice, new_matrice, sizeof(new_matrice));
}

void reverse(int matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE])
{
  int new_matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE];
  for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
  {
    for (int j = 0, jj = OBJECT_MATRICE_SIZE - 1; j < OBJECT_MATRICE_SIZE; j++, jj--)
    {
      new_matrice[i][j] = matrice[i][jj];
    }
  }
  memcpy(matrice, new_matrice, sizeof(new_matrice));
}

int object_collision_detection(TObject object, TNode matrice[FIELD_HEIGHT][FIELD_WIDTH])
{
  for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
  {
    for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
    {
      if (object.pos.y + i > FIELD_HEIGHT || object.pos.y + i < 0 || object.pos.x + j > FIELD_WIDTH || object.pos.x + j < 0)
      {
        continue;
      }
      if (object.matrice[i][j] == 1 && matrice[i + object.pos.y][j + object.pos.x].exists) // +1 because of borders
      {
        //   printf("i:%d + y:%d : j:%d + x:%d\n", i, object.pos.y, j, object.pos.x);
        return 1;
      }
    }
  }
  return 0;
}

int try_move_object(TObject *object, TNode matrice[FIELD_HEIGHT][FIELD_WIDTH], SDL_Point new_pos)
{
  TObject temp_object = *object;
  temp_object.pos = new_pos;

  if (!object_collision_detection(temp_object, matrice))
  {
    *object = temp_object;
    return 1;
  }
  return 0;
}

int try_rotate_object(TObject *object, TNode matrice[FIELD_HEIGHT][FIELD_WIDTH], int rotation)
{
  TObject temp_object = *object;
  int moved_matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE] = {0};
  int is_empty = true;
  int num_empty = 0;

  if (rotation == 1)
  {
    rotate_CW(&temp_object);
  }
  else
  {
    rotate_CC(&temp_object);
  }

  // Try original position
  if (!object_collision_detection(temp_object, matrice))
  {
    *object = temp_object;
    return 1; // Successfully rotated
  }

  // Try moving left or right
  for (int dx = -1; dx <= 1; dx += 2)
  {
    temp_object.pos.x += dx;
    if (!object_collision_detection(temp_object, matrice))
    {
      *object = temp_object;
      return 1;
    }
  }

  // Try moving down
  temp_object.pos.y += 1;
  if (!object_collision_detection(temp_object, matrice))
  {
    *object = temp_object;
    return 1;
  }

  return 0; // Rotation failed, revert
}

void solidify(TObject object, TNode matrice[FIELD_HEIGHT][FIELD_WIDTH])
{
  TNode new_matrice[FIELD_HEIGHT][FIELD_WIDTH];

  memcpy(new_matrice, matrice, sizeof(new_matrice));

  for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
  {
    for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
    {
      if (object.pos.y + i > FIELD_HEIGHT || object.pos.y + i < 0 || object.pos.x + j > FIELD_WIDTH || object.pos.x + j < 0)
      {
        continue;
      }
      if (object.matrice[i][j] == 1)
      {
        new_matrice[object.pos.y + i][object.pos.x + j].exists = true;
        new_matrice[object.pos.y + i][object.pos.x + j].moving = false;
        new_matrice[object.pos.y + i][object.pos.x + j].color = light;
      }
    }
  }

  memcpy(matrice, new_matrice, sizeof(new_matrice));
}

int remove_full_rows(TNode matrice[FIELD_HEIGHT][FIELD_WIDTH])
{
  TNode new_matrice[FIELD_HEIGHT][FIELD_WIDTH];
  memcpy(new_matrice, matrice, sizeof(new_matrice));
  bool full_row;
  int num_full = 0;

  for (int i = FIELD_HEIGHT - 2; i > 0; i--)
  {
    full_row = true;
    for (int j = 1; j < FIELD_WIDTH - 1; j++)
    {
      if (!new_matrice[i][j].exists) // +1 because of borders
      {
        full_row = false;
        break;
      }
    }
    if (full_row)
    {
      for (int k = i; k > 0; k--)
      {
        if (k == 1)
        {
          for (int j = 1; j < FIELD_WIDTH - 1; j++)
          {
            new_matrice[k][j].exists = false;
            new_matrice[k][j].moving = false;
          }
        }
        else
        {
          for (int j = 1; j < FIELD_WIDTH - 1; j++)
          {
            new_matrice[k][j] = new_matrice[k - 1][j];
          }
        }
      }
      num_full++;
      i++;
    }
  }
  memcpy(matrice, new_matrice, sizeof(new_matrice));
  return num_full;
}

void rotate_CC(TObject *object)
{

  TObject temp_object = *object;
  int moved_matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE] = {0};
  int is_empty = true;
  int num_empty = 0;

  reverse(temp_object.matrice);
  transpose(temp_object.matrice);

  // Counts number of empty rows before object
  for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
  {
    is_empty = true;
    for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
    {
      if (temp_object.matrice[i][j] == 1)
      {
        is_empty = false;
        break;
      }
    }
    if (is_empty)
    {
      num_empty++;
    }
    else
    {
      break;
    }
  }
  if (num_empty > 0) // If there are any empty rows, remove them
  {
    // remove empty rows before object
    for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
    {
      for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
      {
        if (i + num_empty < OBJECT_MATRICE_SIZE)
        {
          moved_matrice[i][j] = temp_object.matrice[i + num_empty][j];
        }
        else
        {
          moved_matrice[i][j] = 0;
        }
      }
    }
  }

  if (num_empty > 0)
  {
    memcpy(temp_object.matrice, moved_matrice, sizeof(moved_matrice));
  }
  *object = temp_object;
}

void rotate_CW(TObject *object)
{

  TObject temp_object = *object;
  int moved_matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE] = {0};
  int is_empty = true;
  int num_empty = 0;

  transpose(temp_object.matrice);
  reverse(temp_object.matrice);

  // Counts number of empty cols before object
  for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
  {
    is_empty = true;
    for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
    {
      if (temp_object.matrice[j][i] == 1)
      {
        is_empty = false;
        break;
      }
    }
    if (is_empty)
    {
      num_empty++;
    }
    else
    {
      break;
    }
  }
  if (num_empty > 0) // If there are any empty cols, remove them
  {
    // removes empty cols before object
    for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
    {
      for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
      {
        if (j + num_empty < OBJECT_MATRICE_SIZE)
        {
          moved_matrice[i][j] = temp_object.matrice[i][j + num_empty];
        }
        else
        {
          moved_matrice[i][j] = 0;
        }
      }
    }
  }

  if (num_empty > 0)
  {
    memcpy(temp_object.matrice, moved_matrice, sizeof(moved_matrice));
  }
  *object = temp_object;
}

int rotate_UP(TObject *object)
{
  TObject temp_object = *object;

  for (int i = object->rotation; i > 0; i--)
  {
    rotate_CC(&temp_object);
  }

  *object = temp_object;
  return 0;
}