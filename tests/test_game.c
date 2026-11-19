#include "game.h"
#include "test.h"
#include <string.h>

static int count_filled_inside(const TGame *game)
{
  int count = 0;
  for (int i = 1; i < FIELD_HEIGHT - 1; i++)
  {
    for (int j = 1; j < FIELD_WIDTH - 1; j++)
    {
      count += game->field[i][j] ? 1 : 0;
    }
  }
  return count;
}

static void fill_row_except(TGame *game, int row, int first_gap, int last_gap)
{
  for (int j = 1; j < FIELD_WIDTH - 1; j++)
  {
    game->field[row][j] = (j >= first_gap && j <= last_gap) ? 0 : 1;
  }
}

// Lets the current piece drop and lock (the first failed step only grounds it)
static void drop_and_lock(TGame *game)
{
  for (int i = 0; i < FIELD_HEIGHT + 2 && !game->game_over; i++)
  {
    PieceType before = game->current.type;
    int filled = count_filled_inside(game);
    game_gravity(game);
    if (count_filled_inside(game) != filled || before != game->current.type)
    {
      return;
    }
  }
}

static void test_new_game(void)
{
  TGame game;
  game_init(&game, 42);
  CHECK(!game.game_over);
  CHECK_EQ(game.score, 0);
  CHECK_EQ(game.lines, 0);
  CHECK_EQ(game.level, 0);
  CHECK(!game.has_held);
  CHECK(!game.hold_used);
  CHECK_EQ(count_filled_inside(&game), 0);
  CHECK(!object_collision_detection(&game.current, game.field));
  CHECK_EQ(game.current.pos.y, 1);
  CHECK(game.next >= 0 && game.next < PIECE_COUNT);
  CHECK_EQ(game_gravity_delay_ms(&game), 800);
}

static void test_seed_zero_works(void)
{
  TGame game;
  game_init(&game, 0);
  CHECK(!game.game_over);
  CHECK(!object_collision_detection(&game.current, game.field));
}

static void test_same_seed_same_game(void)
{
  TGame a, b;
  game_init(&a, 1234);
  game_init(&b, 1234);
  for (int i = 0; i < 200; i++)
  {
    game_gravity(&a);
    game_gravity(&b);
  }
  CHECK(memcmp(&a, &b, sizeof(a)) == 0);
}

static void test_spawn_always_in_field(void)
{
  for (unsigned int seed = 1; seed < 300; seed++)
  {
    TGame game;
    game_init(&game, seed);
    CHECK(!game.game_over);
    CHECK(!object_collision_detection(&game.current, game.field));
    CHECK(game.current.pos.x >= 1);
  }
}

static void test_every_piece_type_is_dealt(void)
{
  TGame game;
  game_init(&game, 7);
  bool seen[PIECE_COUNT] = {false};
  for (int i = 0; i < 400 && !game.game_over; i++)
  {
    seen[game.next] = true;
    drop_and_lock(&game);
    matrice_init(game.field); // keep the game running
  }
  for (int type = 0; type < PIECE_COUNT; type++)
  {
    CHECK(seen[type]);
  }
}

static void test_move_stops_at_the_walls(void)
{
  TGame game;
  game_init(&game, 5);
  for (int i = 0; i < FIELD_WIDTH; i++)
  {
    game_move(&game, -1);
  }
  TBounds bounds = object_bounds(&game.current);
  CHECK_EQ(game.current.pos.x + bounds.col_min, 1);
  CHECK(!object_collision_detection(&game.current, game.field));

  for (int i = 0; i < FIELD_WIDTH; i++)
  {
    game_move(&game, 1);
  }
  bounds = object_bounds(&game.current);
  CHECK_EQ(game.current.pos.x + bounds.col_max, FIELD_WIDTH - 2);
  CHECK(!object_collision_detection(&game.current, game.field));
}

static void test_rotate_keeps_piece_valid(void)
{
  TGame game;
  game_init(&game, 9);
  for (int i = 0; i < 10; i++)
  {
    game_rotate(&game, i % 2 ? 1 : -1);
    CHECK(!object_collision_detection(&game.current, game.field));
  }
}

static void test_gravity_moves_down_then_locks_on_second_failure(void)
{
  TGame game;
  game_init(&game, 3);
  game.current = object_create(PIECE_O);
  game.current.pos = (TPoint){4, FIELD_HEIGHT - 4}; // rests on the floor after one step
  game.next = PIECE_T;

  game_gravity(&game);
  CHECK_EQ(game.current.pos.y, FIELD_HEIGHT - 3);
  CHECK_EQ(count_filled_inside(&game), 0);

  game_gravity(&game); // cannot move: grace period
  CHECK(game.grounded);
  CHECK_EQ(count_filled_inside(&game), 0);
  CHECK_EQ(game.current.type, PIECE_O);

  game_gravity(&game); // second failure locks it
  CHECK_EQ(count_filled_inside(&game), 4);
  CHECK(!game.grounded);
  CHECK_EQ(game.current.type, PIECE_T); // the next piece came in
  CHECK_EQ(game.current.pos.y, 1);
}

static void test_sliding_during_grace_period_can_resume_falling(void)
{
  TGame game;
  game_init(&game, 3);
  game.current = object_create(PIECE_O);
  game.current.pos = (TPoint){5, 10}; // covers columns 5..6, rows 10..11
  game.field[12][5] = 1;              // ledge right under it
  game.field[12][6] = 1;

  game_gravity(&game);
  CHECK_EQ(game.current.pos.y, 10); // blocked by the ledge
  CHECK(game.grounded);
  game_move(&game, 1);
  game_move(&game, 1);
  game_gravity(&game); // past the ledge it falls again, nothing was locked
  CHECK_EQ(game.current.pos.y, 11);
  CHECK(!game.grounded);
  CHECK_EQ(count_filled_inside(&game), 2);
}

static void test_soft_drop_scores_one_point_per_step(void)
{
  TGame game;
  game_init(&game, 3);
  game.current = object_create(PIECE_O);
  game.current.pos = (TPoint){4, 1};

  game_soft_drop(&game);
  game_soft_drop(&game);
  CHECK_EQ(game.score, 2);
  CHECK_EQ(game.current.pos.y, 3);

  game_gravity(&game); // falling by itself gives no points
  CHECK_EQ(game.score, 2);
  CHECK_EQ(game.current.pos.y, 4);
}

static void test_soft_drop_into_floor_gives_no_points(void)
{
  TGame game;
  game_init(&game, 3);
  game.current = object_create(PIECE_O);
  game.current.pos = (TPoint){4, FIELD_HEIGHT - 3};
  game_soft_drop(&game);
  CHECK_EQ(game.score, 0);
  CHECK(game.grounded);
}

static void test_one_line_scores_40_at_level_zero(void)
{
  TGame game;
  game_init(&game, 3);
  fill_row_except(&game, 20, 1, 4);
  game.current = object_create(PIECE_I);
  game.current.pos = (TPoint){1, 19}; // horizontal bar on row 20, columns 1..4
  game.next = PIECE_O;
  game.grounded = true;

  game_gravity(&game);
  CHECK_EQ(game.lines, 1);
  CHECK_EQ(game.score, 40);
  CHECK_EQ(game.level, 0);
  CHECK_EQ(count_filled_inside(&game), 0);
  CHECK_EQ(game.current.type, PIECE_O);
}

static void test_tetris_scores_1200(void)
{
  TGame game;
  game_init(&game, 3);
  for (int row = 17; row <= 20; row++)
  {
    fill_row_except(&game, row, 1, 1);
  }
  game.current = object_create(PIECE_I);
  rotate_CW(&game.current);
  game.current.pos = (TPoint){-1, 17}; // vertical bar in column 1, rows 17..20
  game.next = PIECE_O;
  game.grounded = true;

  game_gravity(&game);
  CHECK_EQ(game.lines, 4);
  CHECK_EQ(game.score, 1200);
  CHECK_EQ(count_filled_inside(&game), 0);
}

static void test_score_multiplies_with_level(void)
{
  TGame game;
  game_init(&game, 3);
  game.level = 2;
  game.lines = 20;
  fill_row_except(&game, 20, 1, 4);
  game.current = object_create(PIECE_I);
  game.current.pos = (TPoint){1, 19};
  game.next = PIECE_O;
  game.grounded = true;

  game_gravity(&game);
  CHECK_EQ(game.score, 40 * 3);
  CHECK_EQ(game.lines, 21);
  CHECK_EQ(game.level, 2);
}

static void test_level_goes_up_every_ten_lines(void)
{
  TGame game;
  game_init(&game, 3);
  game.lines = 9;
  fill_row_except(&game, 20, 1, 4);
  game.current = object_create(PIECE_I);
  game.current.pos = (TPoint){1, 19};
  game.next = PIECE_O;
  game.grounded = true;

  game_gravity(&game);
  CHECK_EQ(game.lines, 10);
  CHECK_EQ(game.level, 1);
  CHECK_EQ(game.score, 40); // scored on the level it was cleared on
  CHECK_EQ(game_gravity_delay_ms(&game), 716);
}

static void test_gravity_speeds_up_and_never_reaches_zero(void)
{
  TGame game;
  game_init(&game, 3);
  int previous = game_gravity_delay_ms(&game);
  for (game.level = 1; game.level < 60; game.level++)
  {
    int delay = game_gravity_delay_ms(&game);
    CHECK(delay > 0);
    CHECK(delay <= previous);
    previous = delay;
  }
  game.level = 9;
  CHECK_EQ(game_gravity_delay_ms(&game), 50);
  game.level = 18;
  CHECK_EQ(game_gravity_delay_ms(&game), 50);
  game.level = 19;
  CHECK_EQ(game_gravity_delay_ms(&game), 16);
}

static void test_hold(void)
{
  TGame game;
  game_init(&game, 11);
  PieceType first = game.current.type;
  PieceType second = game.next;

  game_hold(&game);
  CHECK(game.has_held);
  CHECK_EQ(game.held, first);
  CHECK_EQ(game.current.type, second); // the next piece takes over
  CHECK(game.hold_used);

  PieceType current = game.current.type;
  game_hold(&game); // not twice in one drop
  CHECK_EQ(game.held, first);
  CHECK_EQ(game.current.type, current);

  drop_and_lock(&game);
  CHECK(!game.hold_used);
  PieceType now = game.current.type;
  game_hold(&game); // swaps with the held piece
  CHECK_EQ(game.current.type, first);
  CHECK_EQ(game.held, now);
  CHECK(!object_collision_detection(&game.current, game.field));
  CHECK_EQ(game.current.pos.y, 1);
}

static void test_hold_returns_the_piece_in_spawn_orientation(void)
{
  TGame game;
  game_init(&game, 11);
  game.current = object_create(PIECE_T);
  game.current.pos = (TPoint){4, 6};
  rotate_CW(&game.current);
  game.next = PIECE_O;

  game_hold(&game); // holds T, plays O
  drop_and_lock(&game);
  game_hold(&game); // plays T again
  TObject spawn = object_create(PIECE_T);
  CHECK(memcmp(game.current.matrice, spawn.matrice, sizeof(spawn.matrice)) == 0);
}

static void test_game_over_when_spawn_is_blocked(void)
{
  TGame game;
  game_init(&game, 3);
  // The stack reaches the spawn area; every row has a gap, so nothing is cleared
  for (int row = 1; row <= 20; row++)
  {
    fill_row_except(&game, row, FIELD_WIDTH - 2, FIELD_WIDTH - 2); // gap in the last column only
  }
  game.current = object_create(PIECE_O);
  game.current.pos = (TPoint){4, 1}; // stuck in the stack
  game.grounded = true;

  game_gravity(&game); // locks and tries to spawn the next piece
  CHECK(game.game_over);
}

static void test_game_ends_by_stacking_and_then_ignores_input(void)
{
  TGame game;
  game_init(&game, 99);
  for (int i = 0; i < 5000 && !game.game_over; i++)
  {
    game_gravity(&game);
  }
  CHECK(game.game_over);

  TGame frozen = game;
  game_gravity(&game);
  game_soft_drop(&game);
  game_move(&game, 1);
  game_rotate(&game, 1);
  game_hold(&game);
  CHECK(memcmp(&game, &frozen, sizeof(game)) == 0);
}

static void test_field_border_survives_a_long_game(void)
{
  TGame game;
  game_init(&game, 2024);
  for (int i = 0; i < 3000 && !game.game_over; i++)
  {
    game_move(&game, i % 3 - 1);
    game_rotate(&game, i % 2 ? 1 : -1);
    if (i % 5 == 0)
    {
      game_hold(&game);
    }
    game_soft_drop(&game);
    game_gravity(&game);
  }
  for (int i = 0; i < FIELD_HEIGHT; i++)
  {
    CHECK(game.field[i][0] && game.field[i][FIELD_WIDTH - 1]);
  }
  for (int j = 0; j < FIELD_WIDTH; j++)
  {
    CHECK(game.field[0][j] && game.field[FIELD_HEIGHT - 1][j]);
  }
  CHECK(game.score >= 0);
}

int main(void)
{
  test_new_game();
  test_seed_zero_works();
  test_same_seed_same_game();
  test_spawn_always_in_field();
  test_every_piece_type_is_dealt();
  test_move_stops_at_the_walls();
  test_rotate_keeps_piece_valid();
  test_gravity_moves_down_then_locks_on_second_failure();
  test_sliding_during_grace_period_can_resume_falling();
  test_soft_drop_scores_one_point_per_step();
  test_soft_drop_into_floor_gives_no_points();
  test_one_line_scores_40_at_level_zero();
  test_tetris_scores_1200();
  test_score_multiplies_with_level();
  test_level_goes_up_every_ten_lines();
  test_gravity_speeds_up_and_never_reaches_zero();
  test_hold();
  test_hold_returns_the_piece_in_spawn_orientation();
  test_game_over_when_spawn_is_blocked();
  test_game_ends_by_stacking_and_then_ignores_input();
  test_field_border_survives_a_long_game();
  return test_result("test_game");
}
