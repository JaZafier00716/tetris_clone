#ifndef CONFIG_H
#define CONFIG_H
#include "header.h"

/* Key bindings, stored in CFG_FILE as lines "action_name: Key Name;" (names as SDL_GetKeyName writes them). */

typedef enum
{
  ACTION_MOVE_DOWN,
  ACTION_MOVE_LEFT,
  ACTION_MOVE_RIGHT,
  ACTION_MOVE_HOLD,
  ACTION_ROTATE_RIGHT,
  ACTION_ROTATE_LEFT,
  ACTION_COUNT,
  ACTION_NONE = -1
} Action;

typedef struct
{
  SDL_Keycode keys[ACTION_COUNT];
} TKeyMap;

/* === Start config.c Functions === */
const char *action_name(Action action);                       // name used in the config file
void keymap_defaults(TKeyMap *keymap);
bool keymap_load(TKeyMap *keymap, const char *path);          // starts from defaults; false when the file cannot be read
bool keymap_save(const TKeyMap *keymap, const char *path, const char *tmp_path);
Action keymap_action(const TKeyMap *keymap, SDL_Keycode key); // ACTION_NONE when the key is not bound
/* === End config.c Functions === */

#endif
