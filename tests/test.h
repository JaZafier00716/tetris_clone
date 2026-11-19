#ifndef TEST_H
#define TEST_H
/* Tiny test harness: CHECK reports a failure and carries on, test_result() gives the exit code. */
#include <stdio.h>

static int test_failures = 0;
static int test_checks = 0;

#define CHECK(condition)                                                          \
  do                                                                              \
  {                                                                               \
    test_checks++;                                                                \
    if (!(condition))                                                             \
    {                                                                             \
      test_failures++;                                                            \
      printf("%s:%d: check failed: %s\n", __FILE__, __LINE__, #condition);        \
    }                                                                             \
  } while (0)

#define CHECK_EQ(actual, expected) CHECK((long)(actual) == (long)(expected))

static int test_result(const char *name)
{
  printf("%s: %d checks, %d failed\n", name, test_checks, test_failures);
  return test_failures == 0 ? 0 : 1;
}

#endif
