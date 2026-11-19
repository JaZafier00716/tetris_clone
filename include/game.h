#ifndef GAME_H
#define GAME_H
#include <stdbool.h>
#include "movement.h"
#include "objects.h"

/* Game rules and state. Pure logic, no SDL: the window code only feeds it input and draws it. */

#define LINES_PER_LEVEL 10

typedef struct
{
  int field[FIELD_HEIGHT][FIELD_WIDTH];
  TObject current;
  PieceType next;
  PieceType held;
  bool has_held;      // false until the first hold
  bool hold_used;     // a piece can be held only once per drop
  bool grounded;      // the piece failed to move down once, the next failure locks it
  bool game_over;
  int score;
  int lines;
  int level;
  unsigned int rng;   // random generator state
} TGame;

/* === Start game.c Functions === */
void game_init(TGame *game, unsigned int seed);
void game_move(TGame *game, int dx);                // dx = -1 left, 1 right
void game_rotate(TGame *game, int rotation);        // 1 = clockwise, -1 = counter clockwise
void game_soft_drop(TGame *game);                   // player pressed down: moves down, awards 1 point
void game_gravity(TGame *game);                     // timer tick: moves down
void game_hold(TGame *game);
int game_gravity_delay_ms(const TGame *game);       // time between gravity ticks at the current level
/* === End game.c Functions === */

#endif
