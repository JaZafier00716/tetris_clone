#ifndef LAYOUT_H
#define LAYOUT_H

/* Window sizing. Pure logic, no SDL.
   Screens are drawn in a fixed design resolution that SDL scales to the real window. */

#define MENU_DESIGN_WIDTH 800
#define MENU_DESIGN_HEIGHT 600
#define GAME_DESIGN_WIDTH 1920
#define GAME_DESIGN_HEIGHT 1080

/* Largest window with the design aspect ratio that fits into 90 % of the available area,
   never larger than the design size itself. */
void fit_window_size(int design_width, int design_height, int avail_width, int avail_height, int *width, int *height);

#endif
