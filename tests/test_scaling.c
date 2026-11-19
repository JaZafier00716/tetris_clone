// Scaling of the fixed design resolution to windows of any size, checked with SDL's dummy video
// driver and a software renderer: nothing may be drawn in the black bars, and a button drawn in
// design coordinates has to be hit by a click at the matching window pixel.
#include "colors.h"
#include "draw.h"
#include "layout.h"
#include "test.h"
#include <math.h>

#ifndef ASSET_DIR
#define ASSET_DIR "."
#endif

static const int window_sizes[][2] = {
    {1280, 720}, {1366, 768}, {1920, 1080}, {2560, 1440}, {3840, 2160}, {800, 1000}, {1000, 300}, {400, 225}};
static const int design_sizes[][2] = {
    {GAME_DESIGN_WIDTH, GAME_DESIGN_HEIGHT}, {MENU_DESIGN_WIDTH, MENU_DESIGN_HEIGHT}};

static SDL_Window *window;
static SDL_Renderer *renderer;

static bool open_window(int w, int h, int design_w, int design_h)
{
  window = SDL_CreateWindow("scaling test", 0, 0, w, h, SDL_WINDOW_RESIZABLE);
  if (!window)
  {
    return false;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  if (!renderer)
  {
    SDL_DestroyWindow(window);
    return false;
  }
  SDL_RenderSetLogicalSize(renderer, design_w, design_h);
  SDL_RenderSetIntegerScale(renderer, SDL_FALSE);
  return true;
}

static void close_window(void)
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

static Uint32 pixel_at(const Uint32 *pixels, int w, int x, int y)
{
  return pixels[y * w + x] & 0x00FFFFFF;
}

static void test_nothing_is_drawn_outside_the_design_area(int w, int h, int dw, int dh)
{
  CHECK(open_window(w, h, dw, dh));
  int real_w, real_h;
  SDL_GetWindowSize(window, &real_w, &real_h);
  CHECK_EQ(real_w, w);
  CHECK_EQ(real_h, h);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_FRect huge = {-5000, -5000, 10000, 10000}; // far bigger than the design area
  SDL_RenderFillRectF(renderer, &huge);

  // Read the real window pixels
  SDL_RenderSetLogicalSize(renderer, 0, 0);
  Uint32 *pixels = malloc((size_t)w * h * sizeof(Uint32));
  CHECK(pixels != NULL);
  if (pixels)
  {
    CHECK_EQ(SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, pixels, w * (int)sizeof(Uint32)), 0);

    double scale = fmin((double)w / dw, (double)h / dh);
    double left = (w - dw * scale) / 2, top = (h - dh * scale) / 2;
    double right = left + dw * scale, bottom = top + dh * scale;
    const Uint32 red = 0xFF0000;
    int bad_outside = 0, bad_inside = 0, bars = 0;
    for (int y = 0; y < h; y++)
    {
      for (int x = 0; x < w; x++)
      {
        Uint32 p = pixel_at(pixels, w, x, y);
        // 2 pixels of tolerance around the edge for rounding
        bool clearly_outside = x < left - 2 || x > right + 2 || y < top - 2 || y > bottom + 2;
        bool clearly_inside = x > left + 2 && x < right - 2 && y > top + 2 && y < bottom - 2;
        if (clearly_outside)
        {
          bars++;
          bad_outside += p != 0;
        }
        else if (clearly_inside)
        {
          bad_inside += p != red;
        }
      }
    }
    CHECK_EQ(bad_outside, 0);
    CHECK_EQ(bad_inside, 0);
    if (fabs(left) > 4 || fabs(top) > 4)
    {
      CHECK(bars > 0); // the sizes with bars really have bars
    }
    free(pixels);
  }
  close_window();
}

static bool inside(SDL_FRect r, float x, float y)
{
  return x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h;
}

static SDL_Event click_at(int window_x, int window_y)
{
  SDL_Event e = {0};
  e.type = SDL_MOUSEBUTTONDOWN;
  e.button.windowID = SDL_GetWindowID(window);
  e.button.button = SDL_BUTTON_LEFT;
  e.button.x = window_x;
  e.button.y = window_y;
  SDL_PumpEvents();
  SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT); // nothing left over from other windows or clicks
  SDL_PushEvent(&e); // SDL maps window pixels to design coordinates on its way into the queue
  SDL_Event out = {0};
  SDL_PeepEvents(&out, 1, SDL_GETEVENT, SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONDOWN);
  return out;
}

static void test_button_hit_testing(int w, int h, int dw, int dh, TTF_Font *font)
{
  CHECK(open_window(w, h, dw, dh));
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_FPoint where = {.x = dw, .y = dh / 2.0f}; // x is the width to center in, like the game does
  SDL_FRect button = draw_button(renderer, where, green, white, font, START);
  CHECK(button.w > 0 && button.h > 0);
  CHECK(button.x >= 0 && button.y >= 0 && button.x + button.w <= dw && button.y + button.h <= dh);

  int wx, wy;
  SDL_RenderLogicalToWindow(renderer, button.x + button.w / 2, button.y + button.h / 2, &wx, &wy);
  SDL_Event hit = click_at(wx, wy);
  CHECK_EQ(hit.type, SDL_MOUSEBUTTONDOWN);
  CHECK(inside(button, hit.button.x, hit.button.y));

  // a point well left of the button (a quarter of the design width away) must miss
  SDL_RenderLogicalToWindow(renderer, button.x - dw / 4.0f, button.y + button.h / 2, &wx, &wy);
  SDL_Event miss = click_at(wx, wy);
  CHECK(!inside(button, miss.button.x, miss.button.y));

  // the window corners map to the design corners, or outside of the design area where there are bars
  SDL_Event corner = click_at(0, 0);
  // one window pixel covers 1 / scale design pixels, so that much slack is allowed
  const int slack = (int)ceil(1.0 / fmin((double)w / dw, (double)h / dh)) + 1;
  CHECK(corner.button.x <= slack && corner.button.y <= slack);
  corner = click_at(w - 1, h - 1);
  CHECK(corner.button.x >= dw - 1 - slack && corner.button.y >= dh - 1 - slack);
  close_window();
}

int main(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0)
  {
    printf("cannot initialise SDL: %s\n", SDL_GetError());
    return 1;
  }
  TTF_Font *font = TTF_OpenFont(ASSET_DIR "/data/fonts/ProggyCleanNerdFontMono-Regular.ttf", TITLE_SIZE);
  CHECK(font != NULL);

  for (size_t d = 0; d < sizeof(design_sizes) / sizeof(design_sizes[0]); d++)
  {
    for (size_t i = 0; i < sizeof(window_sizes) / sizeof(window_sizes[0]); i++)
    {
      int w = window_sizes[i][0], h = window_sizes[i][1];
      int dw = design_sizes[d][0], dh = design_sizes[d][1];
      test_nothing_is_drawn_outside_the_design_area(w, h, dw, dh);
      if (font)
      {
        test_button_hit_testing(w, h, dw, dh, font);
      }
    }
  }
  if (font)
  {
    TTF_CloseFont(font);
  }
  TTF_Quit();
  SDL_Quit();
  return test_result("test_scaling");
}
