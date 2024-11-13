#include "objects.h"

TObject empty_object = {
  .matrice = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
  },
  .w = 0,
  .h = 0,
  .color = &dark,
  .rotation = UP,
  .pos = {
    .x = 0,
    .y = 0
  }
};

TObject object_I = {
  .matrice = {
    0,0,0,0,
    1,1,1,1,
    0,0,0,0,
    0,0,0,0,
  },
  .w = 4,
  .h = 1,
  .color = &cyan,
  .rotation = UP,
  .pos = {
    .x = 1,
    .y = 1
  }
};

TObject object_J = {
  .matrice = {
    1,0,0,0,
    1,1,1,0,
    0,0,0,0,
    0,0,0,0,
  },
  .w = 3,
  .h = 2,
  .color =&blue,
  .rotation = UP,
  .pos = {
    .x = 1,
    .y = 1
  }
};

TObject object_L = {
  .matrice = {
    0,0,1,0,
    1,1,1,0,
    0,0,0,0,
    0,0,0,0,
  },
  .w = 3,
  .h = 2,
  .color = &orange,
  .rotation = UP,
  .pos = {
    .x = 1,
    .y = 1
  }
};

TObject object_O = {
  .matrice = {
    1,1,0,0,
    1,1,0,0,
    0,0,0,0,
    0,0,0,0,
  },
  .w = 2,
  .h = 2,
  .color = &yellow,
  .rotation = UP,
  .pos = {
    .x = 1,
    .y = 1
  }
};

TObject object_T = {
  .matrice = {
    0,1,0,0,
    1,1,1,0,
    0,0,0,0,
    0,0,0,0,
  },
  .w = 3,
  .h = 2,
  .color = &purple,
  .rotation = UP,
  .pos = {
    .x = 1,
    .y = 1
  }
};

TObject object_S = {
  .matrice = {
    0,1,1,0,
    1,1,0,0,
    0,0,0,0,
    0,0,0,0,
  },
  .w = 3,
  .h = 2,
  .color = &green,
  .rotation = UP,
  .pos = {
    .x = 1,
    .y = 1
  }
};

TObject object_Z = {
  .matrice = {
    1,1,0,0,
    0,1,1,0,
    0,0,0,0,
    0,0,0,0,
  },
  .w = 3,
  .h = 2,
  .color = &red,
  .rotation = UP,
  .pos = {
    .x = 1,
    .y = 1
  }
};