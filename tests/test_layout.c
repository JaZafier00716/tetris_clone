#include "layout.h"
#include "test.h"

static void check_fit(int dw, int dh, int aw, int ah)
{
  int w, h;
  fit_window_size(dw, dh, aw, ah, &w, &h);
  CHECK(w >= 1 && h >= 1);
  CHECK(w <= dw && h <= dh);                       // never bigger than the design
  CHECK(w <= aw * 9 / 10 + 0 || aw * 9 / 10 < 1);  // fits the desktop (with margin)
  CHECK(h <= ah * 9 / 10 + 0 || ah * 9 / 10 < 1);
  // same aspect ratio, up to integer rounding
  long cross = (long)w * dh - (long)h * dw;
  CHECK(cross < dh && cross > -dh);
}

static void test_big_desktop_keeps_design_size(void)
{
  int w, h;
  fit_window_size(1920, 1080, 3840, 2160, &w, &h);
  CHECK_EQ(w, 1920);
  CHECK_EQ(h, 1080);
}

static void test_small_desktop_shrinks(void)
{
  int w, h;
  fit_window_size(1920, 1080, 1280, 720, &w, &h);
  CHECK_EQ(w, 1152); // 90 % of 1280
  CHECK_EQ(h, 648);
  fit_window_size(1920, 1080, 1366, 768, &w, &h);
  CHECK(w <= 1229 && h <= 691);
}

static void test_height_can_be_the_limit(void)
{
  int w, h;
  fit_window_size(800, 600, 3000, 500, &w, &h);
  CHECK_EQ(h, 450);
  CHECK_EQ(w, 600);
}

static void test_many_desktops(void)
{
  int sizes[][2] = {{1280, 720}, {1366, 768}, {1920, 1080}, {2560, 1440}, {3840, 2160},
                    {1024, 768}, {800, 1200}, {320, 240}, {1, 1}, {0, 0}};
  for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++)
  {
    check_fit(GAME_DESIGN_WIDTH, GAME_DESIGN_HEIGHT, sizes[i][0], sizes[i][1]);
    check_fit(MENU_DESIGN_WIDTH, MENU_DESIGN_HEIGHT, sizes[i][0], sizes[i][1]);
  }
}

int main(void)
{
  test_big_desktop_keeps_design_size();
  test_small_desktop_shrinks();
  test_height_can_be_the_limit();
  test_many_desktops();
  return test_result("test_layout");
}
