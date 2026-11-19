#include "game.h"

// Points for 1..4 cleared rows, multiplied by (level + 1). Original Nintendo scoring.
static const int line_scores[5] = {0, 40, 100, 300, 1200};

// xorshift32: small deterministic generator, so a game can be replayed from its seed
static int random_below(TGame *game, int bound)
{
  unsigned int x = game->rng;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  game->rng = x;
  return (int)(x % (unsigned int)bound);
}

// Puts a new piece at the top of the field at a random column. Ends the game if it does not fit.
static void spawn_piece(TGame *game, PieceType type)
{
  game->current = object_create(type);
  // Box of the piece is 4 wide at most, leave the border column on both sides
  int box = type == PIECE_I ? 4 : (type == PIECE_O ? 2 : 3);
  game->current.pos.x = 1 + random_below(game, FIELD_WIDTH - 1 - box);
  game->current.pos.y = 1;
  game->grounded = false;

  if (object_collision_detection(&game->current, game->field))
  {
    game->game_over = true;
  }
}

static void spawn_next_piece(TGame *game)
{
  spawn_piece(game, game->next);
  game->next = (PieceType)random_below(game, PIECE_COUNT);
}

void game_init(TGame *game, unsigned int seed)
{
  *game = (TGame){0};
  game->rng = seed ? seed : 1; // xorshift must not start at 0
  matrice_init(game->field);
  game->next = (PieceType)random_below(game, PIECE_COUNT);
  spawn_next_piece(game);
}

static void lock_piece(TGame *game)
{
  solidify(&game->current, game->field);

  int cleared = remove_full_rows(game->field);
  game->score += line_scores[cleared] * (game->level + 1); // scored with the level the rows were cleared on
  game->lines += cleared;
  game->level = game->lines / LINES_PER_LEVEL;

  game->hold_used = false;
  spawn_next_piece(game);
}

static void step_down(TGame *game, bool by_player)
{
  TPoint new_pos = game->current.pos;
  new_pos.y += 1;

  if (try_move_object(&game->current, game->field, new_pos))
  {
    game->grounded = false;
    if (by_player)
    {
      game->score += 1;
    }
  }
  else if (game->grounded)
  {
    lock_piece(game); // second failure in a row: the piece stays where it is
  }
  else
  {
    game->grounded = true; // grace period, the player can still slide the piece
  }
}

void game_move(TGame *game, int dx)
{
  if (game->game_over)
  {
    return;
  }
  TPoint new_pos = game->current.pos;
  new_pos.x += dx;
  try_move_object(&game->current, game->field, new_pos);
}

void game_rotate(TGame *game, int rotation)
{
  if (game->game_over)
  {
    return;
  }
  try_rotate_object(&game->current, game->field, rotation);
}

void game_soft_drop(TGame *game)
{
  if (!game->game_over)
  {
    step_down(game, true);
  }
}

void game_gravity(TGame *game)
{
  if (!game->game_over)
  {
    step_down(game, false);
  }
}

void game_hold(TGame *game)
{
  if (game->game_over || game->hold_used)
  {
    return;
  }
  PieceType current = game->current.type;

  if (game->has_held)
  {
    spawn_piece(game, game->held);
  }
  else
  {
    game->has_held = true;
    spawn_next_piece(game);
  }
  game->held = current;
  game->hold_used = true;
}

int game_gravity_delay_ms(const TGame *game)
{
  // Frames (at 60 FPS) between drops: 48 at level 0, 5 fewer per level down to 3, 1 from level 19
  int frames = 48 - 5 * (game->level < 9 ? game->level : 9);
  if (game->level > 18)
  {
    frames = 1;
  }
  return frames * 1000 / 60;
}
