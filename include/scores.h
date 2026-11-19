#ifndef SCORES_H
#define SCORES_H
#include <stdbool.h>

/* Best scores kept in a text file, one score per line, highest first. Pure logic, no SDL. */

#define BEST_SCORE_NUM 5

int scores_load(const char *path, int scores[BEST_SCORE_NUM]);               // returns number of scores read, 0 when the file is missing
bool scores_save(const char *path, const int scores[], int count);           // false when the file cannot be written
int scores_insert(int scores[BEST_SCORE_NUM], int count, int score);         // keeps the list sorted and at most BEST_SCORE_NUM long, returns new count
bool scores_record(const char *path, int score);                             // load + insert + save; scores <= 0 are ignored

#endif
