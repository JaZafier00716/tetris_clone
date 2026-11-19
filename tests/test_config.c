// Key binding file: needs the SDL headers and library (key names), but no window.
#include "config.h"
#include "test.h"
#include <stdio.h>

#define TEST_FILE "test_config.tmp"
#define TEST_TMP "test_config.tmp.new"

static void write_file(const char *text)
{
  FILE *f = fopen(TEST_FILE, "w");
  if (f)
  {
    fputs(text, f);
    fclose(f);
  }
}

static void test_defaults(void)
{
  TKeyMap keymap;
  keymap_defaults(&keymap);
  CHECK_EQ(keymap_action(&keymap, SDLK_a), ACTION_MOVE_LEFT);
  CHECK_EQ(keymap_action(&keymap, SDLK_d), ACTION_MOVE_RIGHT);
  CHECK_EQ(keymap_action(&keymap, SDLK_s), ACTION_MOVE_DOWN);
  CHECK_EQ(keymap_action(&keymap, SDLK_c), ACTION_MOVE_HOLD);
  CHECK_EQ(keymap_action(&keymap, SDLK_RIGHT), ACTION_ROTATE_RIGHT);
  CHECK_EQ(keymap_action(&keymap, SDLK_LEFT), ACTION_ROTATE_LEFT);
  CHECK_EQ(keymap_action(&keymap, SDLK_x), ACTION_NONE);
  CHECK_EQ(keymap_action(&keymap, SDLK_UNKNOWN), ACTION_NONE);
}

static void test_save_and_load_round_trip(void)
{
  TKeyMap saved;
  keymap_defaults(&saved);
  saved.keys[ACTION_MOVE_LEFT] = SDLK_SPACE;
  saved.keys[ACTION_MOVE_HOLD] = SDLK_SEMICOLON; // special characters of the file format
  saved.keys[ACTION_MOVE_DOWN] = SDLK_COLON;
  saved.keys[ACTION_ROTATE_LEFT] = SDLK_UP;
  CHECK(keymap_save(&saved, TEST_FILE, TEST_TMP));

  TKeyMap loaded;
  CHECK(keymap_load(&loaded, TEST_FILE));
  for (int i = 0; i < ACTION_COUNT; i++)
  {
    CHECK_EQ(loaded.keys[i], saved.keys[i]);
  }
  remove(TEST_FILE);
}

static void test_load_keeps_defaults_for_bad_lines(void)
{
  write_file("move_left: Z;\nnonsense\nmove_right: NoSuchKey;\nunknown_action: Q;\n\nmove_hold:\n");
  TKeyMap keymap, defaults;
  keymap_defaults(&defaults);
  CHECK(keymap_load(&keymap, TEST_FILE));
  CHECK_EQ(keymap.keys[ACTION_MOVE_LEFT], SDLK_z);
  CHECK_EQ(keymap.keys[ACTION_MOVE_RIGHT], defaults.keys[ACTION_MOVE_RIGHT]);
  CHECK_EQ(keymap.keys[ACTION_MOVE_HOLD], defaults.keys[ACTION_MOVE_HOLD]);
  CHECK_EQ(keymap.keys[ACTION_MOVE_DOWN], defaults.keys[ACTION_MOVE_DOWN]);
  remove(TEST_FILE);
}

static void test_missing_file_gives_defaults(void)
{
  remove(TEST_FILE);
  TKeyMap keymap, defaults;
  keymap_defaults(&defaults);
  CHECK(!keymap_load(&keymap, TEST_FILE));
  for (int i = 0; i < ACTION_COUNT; i++)
  {
    CHECK_EQ(keymap.keys[i], defaults.keys[i]);
  }
}

static void test_failed_save_keeps_the_old_file(void)
{
  write_file("move_left: Z;\n");
  TKeyMap keymap;
  keymap_defaults(&keymap);
  CHECK(!keymap_save(&keymap, TEST_FILE, "/nonexistent-dir/tmp.cfg"));
  TKeyMap loaded;
  CHECK(keymap_load(&loaded, TEST_FILE));
  CHECK_EQ(loaded.keys[ACTION_MOVE_LEFT], SDLK_z);
  remove(TEST_FILE);
}

int main(void)
{
  test_defaults();
  test_save_and_load_round_trip();
  test_load_keeps_defaults_for_bad_lines();
  test_missing_file_gives_defaults();
  test_failed_save_keeps_the_old_file();
  return test_result("test_config");
}
