#include "scores.h"
#include "test.h"
#include <stdio.h>
#include <stdlib.h>

#define TEST_FILE "test_scores.tmp"

static void write_file(const char *text)
{
  FILE *f = fopen(TEST_FILE, "w");
  if (f)
  {
    fputs(text, f);
    fclose(f);
  }
}

static void test_insert_into_empty_list(void)
{
  int scores[BEST_SCORE_NUM];
  CHECK_EQ(scores_insert(scores, 0, 100), 1);
  CHECK_EQ(scores[0], 100);
}

static void test_insert_keeps_the_list_sorted(void)
{
  int scores[BEST_SCORE_NUM];
  int count = 0;
  count = scores_insert(scores, count, 50);
  count = scores_insert(scores, count, 300);
  count = scores_insert(scores, count, 100);
  CHECK_EQ(count, 3);
  CHECK_EQ(scores[0], 300);
  CHECK_EQ(scores[1], 100);
  CHECK_EQ(scores[2], 50);
}

static void test_low_score_is_added_when_there_is_room(void)
{
  // Used to be lost when it was lower than every stored score
  int scores[BEST_SCORE_NUM] = {100, 50};
  int count = scores_insert(scores, 2, 10);
  CHECK_EQ(count, 3);
  CHECK_EQ(scores[2], 10);
}

static void test_full_list_drops_the_lowest(void)
{
  int scores[BEST_SCORE_NUM] = {500, 400, 300, 200, 100};
  int count = scores_insert(scores, BEST_SCORE_NUM, 250);
  CHECK_EQ(count, BEST_SCORE_NUM);
  CHECK_EQ(scores[0], 500);
  CHECK_EQ(scores[3], 250);
  CHECK_EQ(scores[4], 200);
}

static void test_full_list_rejects_a_worse_score(void)
{
  int scores[BEST_SCORE_NUM] = {500, 400, 300, 200, 100};
  CHECK_EQ(scores_insert(scores, BEST_SCORE_NUM, 50), BEST_SCORE_NUM);
  CHECK_EQ(scores[4], 100);
  CHECK_EQ(scores_insert(scores, BEST_SCORE_NUM, 100), BEST_SCORE_NUM); // a tie does not beat an older score
  CHECK_EQ(scores[4], 100);
}

static void test_best_score_goes_first(void)
{
  int scores[BEST_SCORE_NUM] = {500, 400, 300, 200, 100};
  scores_insert(scores, BEST_SCORE_NUM, 900);
  CHECK_EQ(scores[0], 900);
  CHECK_EQ(scores[1], 500);
  CHECK_EQ(scores[4], 200);
}

static void test_missing_file(void)
{
  remove(TEST_FILE);
  int scores[BEST_SCORE_NUM];
  CHECK_EQ(scores_load(TEST_FILE, scores), 0);
}

static void test_save_and_load(void)
{
  int scores[BEST_SCORE_NUM] = {900, 500, 20};
  CHECK(scores_save(TEST_FILE, scores, 3));
  int loaded[BEST_SCORE_NUM];
  CHECK_EQ(scores_load(TEST_FILE, loaded), 3);
  CHECK_EQ(loaded[0], 900);
  CHECK_EQ(loaded[1], 500);
  CHECK_EQ(loaded[2], 20);
  remove(TEST_FILE);
}

static void test_load_survives_a_damaged_file(void)
{
  // blank lines, text, negative and absurd numbers, no final newline, more lines than fit
  write_file("100\n\nabc\n-5\n99999999999999\n300\n  250\n7\n8\n9\n10\n11");
  int scores[BEST_SCORE_NUM];
  int count = scores_load(TEST_FILE, scores);
  CHECK_EQ(count, BEST_SCORE_NUM);
  CHECK_EQ(scores[0], 300);
  CHECK_EQ(scores[1], 250);
  CHECK_EQ(scores[2], 100);
  CHECK_EQ(scores[3], 11);
  CHECK_EQ(scores[4], 10);
  remove(TEST_FILE);
}

static void test_record(void)
{
  remove(TEST_FILE);
  CHECK(scores_record(TEST_FILE, 0)); // nothing worth saving
  int scores[BEST_SCORE_NUM];
  CHECK_EQ(scores_load(TEST_FILE, scores), 0);

  CHECK(scores_record(TEST_FILE, 120));
  CHECK(scores_record(TEST_FILE, 40));
  CHECK(scores_record(TEST_FILE, 300));
  CHECK_EQ(scores_load(TEST_FILE, scores), 3);
  CHECK_EQ(scores[0], 300);
  CHECK_EQ(scores[1], 120);
  CHECK_EQ(scores[2], 40);

  for (int i = 1; i <= 10; i++)
  {
    scores_record(TEST_FILE, i * 1000);
  }
  CHECK_EQ(scores_load(TEST_FILE, scores), BEST_SCORE_NUM);
  CHECK_EQ(scores[0], 10000);
  CHECK_EQ(scores[4], 6000);
  remove(TEST_FILE);
}

static void test_record_reports_unwritable_file(void)
{
  CHECK(!scores_record("/nonexistent-dir/scores.txt", 10));
}

int main(void)
{
  test_insert_into_empty_list();
  test_insert_keeps_the_list_sorted();
  test_low_score_is_added_when_there_is_room();
  test_full_list_drops_the_lowest();
  test_full_list_rejects_a_worse_score();
  test_best_score_goes_first();
  test_missing_file();
  test_save_and_load();
  test_load_survives_a_damaged_file();
  test_record();
  test_record_reports_unwritable_file();
  return test_result("test_scores");
}
