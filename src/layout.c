#include "layout.h"

void fit_window_size(int design_width, int design_height, int avail_width, int avail_height, int *width, int *height)
{
  // Work in integers: shrink by the more limiting of the two ratios
  long max_w = (long)avail_width * 9 / 10;
  long max_h = (long)avail_height * 9 / 10;
  long w = design_width;
  long h = design_height;

  if (max_w < 1 || max_h < 1)
  {
    max_w = max_h = 1;
  }
  if (w > max_w)
  {
    h = h * max_w / w;
    w = max_w;
  }
  if (h > max_h)
  {
    w = w * max_h / h;
    h = max_h;
  }
  *width = w < 1 ? 1 : (int)w;
  *height = h < 1 ? 1 : (int)h;
}
