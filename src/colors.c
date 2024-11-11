#include "header.h"

SDL_Color black = {
    0,  // Red
    0,  // Green
    0,  // Blue
    255 // Alpha
};
SDL_Color white = {
    255, // Red
    255, // Green
    255, // Blue
    255  // Alpha
};

TColor light = {
    .primary = {
        205, // Red
        205, // Green
        205, // Blue
        255  // Alpha
    },
    .secondary = {
        130, // Red
        130, // Green
        130, // Blue
        255  // Alpha
    }};

// Empty node color
TColor dark = {
    .primary = {
        100, // Red
        100, // Green
        100, // Blue
        255  // Alpha
    },
    .secondary = {
        50, // Red
        50, // Green
        50, // Blue
        255 // Alpha
    }};

TColor red = {
    .primary = {
        255, // Red
        70,  // Green
        35,  // Blue
        255  // Alpha
    },
    .secondary = {
        195, // Red
        45,  // Green
        20,  // Blue
        255  // Alpha
    }};

TColor orange = {
    .primary = {
        255, // Red
        155, // Green
        0,   // Blue
        255  // Alpha
    },

    .secondary = {
        180, // Red
        115, // Green
        0,   // Blue
        255  // Alpha
    }};

TColor yellow = {
    .primary = {
        255, // Red
        230, // Green
        0,   // Blue
        255  // Alpha
    },

    .secondary = {
        180, // Red
        160, // Green
        0,   // Blue
        255  // Alpha
    }};

TColor green = {
    .primary = {
        10,  // Red
        255, // Green
        0,   // Blue
        255  // Alpha
    },

    .secondary = {
        45,  // Red
        180, // Green
        0,   // Blue
        255  // Alpha
    }};

TColor cyan = {
    .primary = {
        0,   // Red
        255, // Green
        250, // Blue
        255  // Alpha
    },

    .secondary = {
        0,   // Red
        175, // Green
        180, // Blue
        255  // Alpha
    }};

TColor blue = {
    .primary = {
        0,   // Red
        130, // Green
        255, // Blue
        255  // Alpha
    },

    .secondary = {
        0,   // Red
        100, // Green
        180, // Blue
        255  // Alpha
    }};

TColor purple = {
    .primary = {
        150, // Red
        0,   // Green
        255, // Blue
        255  // Alpha
    },

    .secondary = {
        110, // Red
        0,   // Green
        180, // Blue
        255  // Alpha
    }};
