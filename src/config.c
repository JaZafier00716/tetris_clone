#include "config.h"
#include <ctype.h>

static const char *action_names[ACTION_COUNT] = {
    [ACTION_MOVE_DOWN] = "move_down",
    [ACTION_MOVE_LEFT] = "move_left",
    [ACTION_MOVE_RIGHT] = "move_right",
    [ACTION_MOVE_HOLD] = "move_hold",
    [ACTION_ROTATE_RIGHT] = "rotate_right",
    [ACTION_ROTATE_LEFT] = "rotate_left",
};

const char *action_name(Action action)
{
  return action_names[action];
}

void keymap_defaults(TKeyMap *keymap)
{
  keymap->keys[ACTION_MOVE_DOWN] = SDLK_s;
  keymap->keys[ACTION_MOVE_LEFT] = SDLK_a;
  keymap->keys[ACTION_MOVE_RIGHT] = SDLK_d;
  keymap->keys[ACTION_MOVE_HOLD] = SDLK_c;
  keymap->keys[ACTION_ROTATE_RIGHT] = SDLK_RIGHT;
  keymap->keys[ACTION_ROTATE_LEFT] = SDLK_LEFT;
}

// Removes spaces around the text in place and returns the start of it
static char *trim(char *text)
{
  while (isspace((unsigned char)*text))
  {
    text++;
  }
  size_t len = strlen(text);
  while (len > 0 && isspace((unsigned char)text[len - 1]))
  {
    text[--len] = '\0';
  }
  return text;
}

bool keymap_load(TKeyMap *keymap, const char *path)
{
  keymap_defaults(keymap);

  FILE *f = fopen(path, "r");
  if (!f)
  {
    return false;
  }
  char row[SETTINGS_ROW_SIZE];
  while (fgets(row, sizeof(row), f))
  {
    char *colon = strchr(row, ':');
    if (!colon)
    {
      continue;
    }
    *colon = '\0';
    char *name = trim(row);
    char *value = trim(colon + 1);
    size_t len = strlen(value);
    if (len > 0 && value[len - 1] == ';') // the key itself may be ';', so remove just one
    {
      value[len - 1] = '\0';
    }
    value = trim(value);

    SDL_Keycode key = SDL_GetKeyFromName(value);
    for (int i = 0; i < ACTION_COUNT; i++)
    {
      if (strcmp(name, action_names[i]) == 0 && key != SDLK_UNKNOWN)
      {
        keymap->keys[i] = key;
      }
    }
  }
  fclose(f);
  return true;
}

bool keymap_save(const TKeyMap *keymap, const char *path, const char *tmp_path)
{
  // Write a temporary file first, so a failure never destroys the existing config
  FILE *f = fopen(tmp_path, "w");
  if (!f)
  {
    return false;
  }
  for (int i = 0; i < ACTION_COUNT; i++)
  {
    fprintf(f, "%s: %s;\n", action_names[i], SDL_GetKeyName(keymap->keys[i]));
  }
  if (fclose(f) != 0)
  {
    return false;
  }
  return rename(tmp_path, path) == 0;
}

Action keymap_action(const TKeyMap *keymap, SDL_Keycode key)
{
  for (int i = 0; i < ACTION_COUNT; i++)
  {
    if (keymap->keys[i] == key)
    {
      return (Action)i;
    }
  }
  return ACTION_NONE;
}
