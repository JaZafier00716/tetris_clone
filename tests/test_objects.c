#include "objects.h"
#include "test.h"
#include <stdbool.h>
#include <string.h>

static int count_squares(const TObject *object)
{
  int count = 0;
  for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
  {
    for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
    {
      count += object->matrice[i][j];
    }
  }
  return count;
}

static bool same_shape(const TObject *a, const TObject *b)
{
  return memcmp(a->matrice, b->matrice, sizeof(a->matrice)) == 0;
}

static void test_every_piece_has_four_squares(void)
{
  for (int type = 0; type < PIECE_COUNT; type++)
  {
    TObject object = object_create((PieceType)type);
    CHECK_EQ(count_squares(&object), 4);
    CHECK_EQ(object.type, type);
    CHECK_EQ(object.pos.x, 0);
    CHECK_EQ(object.pos.y, 0);
  }
}

static void test_four_rotations_restore_the_piece(void)
{
  for (int type = 0; type < PIECE_COUNT; type++)
  {
    TObject start = object_create((PieceType)type);
    TObject cw = start;
    TObject cc = start;
    for (int i = 0; i < 4; i++)
    {
      rotate_CW(&cw);
      rotate_CC(&cc);
      CHECK_EQ(count_squares(&cw), 4);
      CHECK_EQ(count_squares(&cc), 4);
    }
    CHECK(same_shape(&cw, &start));
    CHECK(same_shape(&cc, &start));
  }
}

static void test_cc_undoes_cw(void)
{
  for (int type = 0; type < PIECE_COUNT; type++)
  {
    TObject start = object_create((PieceType)type);
    TObject object = start;
    rotate_CW(&object);
    rotate_CC(&object);
    CHECK(same_shape(&object, &start));
  }
}

static void test_rotation_keeps_position_and_type(void)
{
  TObject object = object_create(PIECE_L);
  object.pos.x = 5;
  object.pos.y = 7;
  rotate_CW(&object);
  CHECK_EQ(object.pos.x, 5);
  CHECK_EQ(object.pos.y, 7);
  CHECK_EQ(object.type, PIECE_L);
}

static void test_o_piece_does_not_change(void)
{
  TObject start = object_create(PIECE_O);
  TObject object = start;
  rotate_CW(&object);
  CHECK(same_shape(&object, &start));
}

static void test_t_piece_rotates_clockwise(void)
{
  // Spawn:   .X.     CW:  .X.
  //          XXX          .XX
  //                       .X.
  TObject object = object_create(PIECE_T);
  rotate_CW(&object);
  const int expected[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE] = {
      {0, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0}};
  CHECK(memcmp(object.matrice, expected, sizeof(expected)) == 0);
}

static void test_i_piece_rotates_into_a_vertical_bar(void)
{
  TObject object = object_create(PIECE_I);
  rotate_CW(&object);
  for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
  {
    CHECK_EQ(object.matrice[i][2], 1);
  }
  TBounds bounds = object_bounds(&object);
  CHECK_EQ(bounds.col_min, 2);
  CHECK_EQ(bounds.col_max, 2);
  CHECK_EQ(bounds.row_min, 0);
  CHECK_EQ(bounds.row_max, 3);
}

static void test_bounds(void)
{
  TObject object = object_create(PIECE_I);
  TBounds bounds = object_bounds(&object);
  CHECK_EQ(bounds.row_min, 1);
  CHECK_EQ(bounds.row_max, 1);
  CHECK_EQ(bounds.col_min, 0);
  CHECK_EQ(bounds.col_max, 3);

  object = object_create(PIECE_T);
  bounds = object_bounds(&object);
  CHECK_EQ(bounds.row_min, 0);
  CHECK_EQ(bounds.row_max, 1);
  CHECK_EQ(bounds.col_min, 0);
  CHECK_EQ(bounds.col_max, 2);
}

int main(void)
{
  test_every_piece_has_four_squares();
  test_four_rotations_restore_the_piece();
  test_cc_undoes_cw();
  test_rotation_keeps_position_and_type();
  test_o_piece_does_not_change();
  test_t_piece_rotates_clockwise();
  test_i_piece_rotates_into_a_vertical_bar();
  test_bounds();
  return test_result("test_objects");
}
