#include "header.h"
#include "logo_512.c"

static void SetSDLIcon(SDL_Window *window)
{

  Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  int shift = (my_icon.bytes_per_pixel == 3) ? 8 : 0;
  rmask = 0xff000000 >> shift;
  gmask = 0x00ff0000 >> shift;
  bmask = 0x0000ff00 >> shift;
  amask = 0x000000ff >> shift;
#else // little endian, like x86
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = (tetris_logo.bytes_per_pixel == 3) ? 0 : 0xff000000;
#endif
  SDL_Surface *icon = SDL_CreateRGBSurfaceFrom((void *)tetris_logo.pixel_data,
                                               tetris_logo.width, tetris_logo.height, tetris_logo.bytes_per_pixel * 8,
                                               tetris_logo.bytes_per_pixel * tetris_logo.width, rmask, gmask, bmask, amask);
  SDL_SetWindowIcon(window, icon);

  SDL_FreeSurface(icon);
}