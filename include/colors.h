#ifndef COLORS_H
#define COLORS_H
#include "header.h"

/* === Start Color Definitions === */
extern const TColor light;
extern const TColor dark;
extern const TColor red;
extern const TColor orange;
extern const TColor yellow;
extern const TColor green;
extern const TColor cyan;
extern const TColor blue;
extern const TColor purple;

extern const SDL_Color black;
extern const SDL_Color white;

const TColor *piece_color(PieceType type);
/* === End Color Definitions === */

#endif
