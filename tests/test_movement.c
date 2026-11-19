#include "movement.h"
#include "test.h"
#include <string.h>

typedef int Field[FIELD_HEIGHT][FIELD_WIDTH];

static int count_filled_inside(Field field)
{
  int count = 0;
  for (int i = 1; i < FIELD_HEIGHT - 1; i++)
  {
    for (int j = 1; j < FIELD_WIDTH - 1; j++)
    {
      count += field[i][j] ? 1 : 0;
    }
  }
  return count;
}

static void fill_row(Field field, int row)
{
  for (int j = 1; j < FIELD_WIDTH - 1; j++)
  {
    field[row][j] = 1;
  }
}

static bool border_is_intact(Field field)
{
  for (int i = 0; i < FIELD_HEIGHT; i++)
  {
    if (!field[i][0] || !field[i][FIELD_WIDTH - 1])
    {
      return false;
    }
  }
  for (int j = 0; j < FIELD_WIDTH; j++)
  {
    if (!field[0][j] || !field[FIELD_HEIGHT - 1][j])
    {
      return false;
    }
  }
  return true;
}

static void test_field_init(void)
{
  Field field;
  memset(field, 7, sizeof(field)); // garbage
  matrice_init(field);
  CHECK(border_is_intact(field));
  CHECK_EQ(count_filled_inside(field), 0);
}

static void test_collision_with_walls_and_floor(void)
{
  Field field;
  matrice_init(field);
  TObject object = object_create(PIECE_O); // squares in the 2x2 top-left of the matrice

  object.pos = (TPoint){1, 1};
  CHECK(!object_collision_detection(&object, field));
  object.pos = (TPoint){0, 1}; // left square on the left border
  CHECK(object_collision_detection(&object, field));
  object.pos = (TPoint){FIELD_WIDTH - 2, 1}; // right square on the right border
  CHECK(object_collision_detection(&object, field));
  object.pos = (TPoint){FIELD_WIDTH - 3, 1}; // last free place at the right wall
  CHECK(!object_collision_detection(&object, field));
  object.pos = (TPoint){1, FIELD_HEIGHT - 3}; // lowest free place
  CHECK(!object_collision_detection(&object, field));
  object.pos = (TPoint){1, FIELD_HEIGHT - 2}; // into the floor
  CHECK(object_collision_detection(&object, field));
}

static void test_squares_outside_the_field_collide(void)
{
  Field field;
  matrice_init(field);
  TObject object = object_create(PIECE_O);

  // Used to be ignored, or to read outside of the array
  TPoint outside[] = {{-5, 5}, {5, -5}, {FIELD_WIDTH + 3, 5}, {5, FIELD_HEIGHT}, {5, FIELD_HEIGHT + 4}, {-1, -1}};
  for (size_t i = 0; i < sizeof(outside) / sizeof(outside[0]); i++)
  {
    object.pos = outside[i];
    CHECK(object_collision_detection(&object, field));
  }
}

static void test_collision_with_locked_squares(void)
{
  Field field;
  matrice_init(field);
  field[5][5] = 1;
  TObject object = object_create(PIECE_O);
  object.pos = (TPoint){4, 4}; // covers (4..5, 4..5)
  CHECK(object_collision_detection(&object, field));
  object.pos = (TPoint){6, 4};
  CHECK(!object_collision_detection(&object, field));
}

static void test_empty_matrice_cells_do_not_collide(void)
{
  Field field;
  matrice_init(field);
  TObject object = object_create(PIECE_T);
  object.pos = (TPoint){1, 1};
  field[1][1] = 1; // under the empty top-left cell of the T matrice
  CHECK(!object_collision_detection(&object, field));
}

static void test_try_move(void)
{
  Field field;
  matrice_init(field);
  TObject object = object_create(PIECE_O);
  object.pos = (TPoint){1, 1};

  CHECK(!try_move_object(&object, field, (TPoint){0, 1})); // into the wall
  CHECK_EQ(object.pos.x, 1);
  CHECK(try_move_object(&object, field, (TPoint){2, 1}));
  CHECK_EQ(object.pos.x, 2);
  CHECK(!try_move_object(&object, field, (TPoint){2, FIELD_HEIGHT - 2}));
  CHECK_EQ(object.pos.y, 1);
}

static void test_rotation_in_open_space(void)
{
  Field field;
  matrice_init(field);
  TObject object = object_create(PIECE_T);
  object.pos = (TPoint){4, 5};

  CHECK(try_rotate_object(&object, field, 1));
  CHECK_EQ(object.pos.x, 4);
  CHECK_EQ(object.pos.y, 5);
  CHECK(try_rotate_object(&object, field, -1));
  TObject spawn = object_create(PIECE_T);
  CHECK(memcmp(object.matrice, spawn.matrice, sizeof(spawn.matrice)) == 0);
}

static void test_rotation_kicks_off_the_wall(void)
{
  Field field;
  matrice_init(field);
  TObject object = object_create(PIECE_I);
  rotate_CW(&object);          // vertical bar in matrice column 2
  object.pos = (TPoint){-1, 5}; // so the bar is in field column 1, next to the wall

  // Horizontal again it would stick out of the wall, so it has to be moved sideways
  CHECK(try_rotate_object(&object, field, 1));
  CHECK(!object_collision_detection(&object, field));
  CHECK_EQ(object.pos.x, 1);
}

static void test_rotation_kicks_off_the_right_wall(void)
{
  Field field;
  matrice_init(field);
  TObject object = object_create(PIECE_I);
  rotate_CW(&object);
  object.pos = (TPoint){FIELD_WIDTH - 4, 5}; // bar in column FIELD_WIDTH - 2, next to the right wall

  CHECK(try_rotate_object(&object, field, 1));
  CHECK(!object_collision_detection(&object, field));
}

static void test_rotation_fails_when_boxed_in(void)
{
  Field field;
  matrice_init(field);
  for (int i = 1; i < FIELD_HEIGHT - 1; i++)
  {
    fill_row(field, i);
    field[i][5] = 0; // a well of width one
  }
  TObject object = object_create(PIECE_I);
  rotate_CW(&object);
  object.pos = (TPoint){3, 5}; // bar in column 5
  CHECK(!object_collision_detection(&object, field));

  TObject before = object;
  CHECK(!try_rotate_object(&object, field, 1));
  CHECK(memcmp(&object, &before, sizeof(object)) == 0);
}

static void test_solidify(void)
{
  Field field;
  matrice_init(field);
  TObject object = object_create(PIECE_T);
  object.pos = (TPoint){3, 18};
  solidify(&object, field);
  CHECK_EQ(count_filled_inside(field), 4);
  CHECK_EQ(field[18][4], 1);
  CHECK_EQ(field[19][3], 1);
  CHECK_EQ(field[19][4], 1);
  CHECK_EQ(field[19][5], 1);
  CHECK(border_is_intact(field));
}

static void test_solidify_ignores_squares_outside_the_field(void)
{
  Field field;
  matrice_init(field);
  TObject object = object_create(PIECE_O);
  object.pos = (TPoint){FIELD_WIDTH - 1, FIELD_HEIGHT - 1}; // mostly outside
  solidify(&object, field);
  CHECK(border_is_intact(field));
}

static void test_remove_nothing(void)
{
  Field field;
  matrice_init(field);
  fill_row(field, 20);
  field[20][4] = 0;
  Field copy;
  memcpy(copy, field, sizeof(field));
  CHECK_EQ(remove_full_rows(field), 0);
  CHECK(memcmp(copy, field, sizeof(field)) == 0);
}

static void test_remove_one_row(void)
{
  Field field;
  matrice_init(field);
  fill_row(field, 20);
  field[19][3] = 1; // lies on the full row and has to drop
  CHECK_EQ(remove_full_rows(field), 1);
  CHECK_EQ(field[20][3], 1);
  CHECK_EQ(count_filled_inside(field), 1);
  CHECK(border_is_intact(field));
}

static void test_remove_several_rows_with_gaps_between(void)
{
  Field field;
  matrice_init(field);
  fill_row(field, 20);
  fill_row(field, 19);
  field[18][2] = 1; // partial row in between
  fill_row(field, 17);
  field[16][6] = 1;
  CHECK_EQ(remove_full_rows(field), 3);
  // The two partial rows are now at the bottom, in the same order
  CHECK_EQ(field[20][2], 1);
  CHECK_EQ(field[19][6], 1);
  CHECK_EQ(count_filled_inside(field), 2);
  CHECK(border_is_intact(field));
}

static void test_remove_four_rows(void)
{
  Field field;
  matrice_init(field);
  for (int i = 17; i <= 20; i++)
  {
    fill_row(field, i);
  }
  CHECK_EQ(remove_full_rows(field), 4);
  CHECK_EQ(count_filled_inside(field), 0);
  CHECK(border_is_intact(field));
}

static void test_remove_top_row_and_all_rows(void)
{
  Field field;
  matrice_init(field);
  fill_row(field, 1);
  CHECK_EQ(remove_full_rows(field), 1);
  CHECK_EQ(count_filled_inside(field), 0);

  for (int i = 1; i < FIELD_HEIGHT - 1; i++)
  {
    fill_row(field, i);
  }
  CHECK_EQ(remove_full_rows(field), FIELD_HEIGHT - 2);
  CHECK_EQ(count_filled_inside(field), 0);
  CHECK(border_is_intact(field));
}

int main(void)
{
  test_field_init();
  test_collision_with_walls_and_floor();
  test_squares_outside_the_field_collide();
  test_collision_with_locked_squares();
  test_empty_matrice_cells_do_not_collide();
  test_try_move();
  test_rotation_in_open_space();
  test_rotation_kicks_off_the_wall();
  test_rotation_kicks_off_the_right_wall();
  test_rotation_fails_when_boxed_in();
  test_solidify();
  test_solidify_ignores_squares_outside_the_field();
  test_remove_nothing();
  test_remove_one_row();
  test_remove_several_rows_with_gaps_between();
  test_remove_four_rows();
  test_remove_top_row_and_all_rows();
  return test_result("test_movement");
}
