#include "scores.h"
#include <stdio.h>
#include <stdlib.h>

int scores_insert(int scores[BEST_SCORE_NUM], int count, int score)
{
  // Find the first entry the new score beats (ties go below the older score)
  int pos = 0;
  while (pos < count && scores[pos] >= score)
  {
    pos++;
  }
  if (pos >= BEST_SCORE_NUM)
  {
    return count; // not good enough for the list
  }
  if (count < BEST_SCORE_NUM)
  {
    count++;
  }
  for (int i = count - 1; i > pos; i--)
  {
    scores[i] = scores[i - 1];
  }
  scores[pos] = score;
  return count;
}

int scores_load(const char *path, int scores[BEST_SCORE_NUM])
{
  FILE *f = fopen(path, "r");
  if (!f)
  {
    return 0;
  }
  int count = 0;
  char row[64];
  while (fgets(row, sizeof(row), f))
  {
    char *end;
    long value = strtol(row, &end, 10);
    if (end == row || value <= 0 || value > 999999999)
    {
      continue; // blank or damaged line
    }
    count = scores_insert(scores, count, (int)value); // also sorts a hand-edited file
  }
  fclose(f);
  return count;
}

bool scores_save(const char *path, const int scores[], int count)
{
  FILE *f = fopen(path, "w");
  if (!f)
  {
    return false;
  }
  for (int i = 0; i < count; i++)
  {
    fprintf(f, "%d\n", scores[i]);
  }
  return fclose(f) == 0;
}

bool scores_record(const char *path, int score)
{
  if (score <= 0)
  {
    return true;
  }
  int scores[BEST_SCORE_NUM];
  int count = scores_load(path, scores);
  count = scores_insert(scores, count, score);
  return scores_save(path, scores, count);
}
