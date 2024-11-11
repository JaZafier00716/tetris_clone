// #include "header.h"

// void init_default_hold(TObject *object, enum direction rotation) {
//   int default_matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE] = {
//       0, 0, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0};
//   object->color = dark;
//   object->rotation = rotation;
//   object->pos.x = 0;
//   object->pos.y = 0;
// }

// void init_object_I(TObject *object, enum direction rotation, SDL_Point new_pos)
// {
//   int default_matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE] = {
//       0, 0, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0};
//   object->color = cyan;
//   object->rotation = rotation;
//   object->pos.x = new_pos.x;
//   object->pos.y = new_pos.y;

//   switch (object->rotation)
//   {
//   case UP:
//     default_matrice[1][0] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[1][2] = 1;
//     default_matrice[1][3] = 1;
//     object->h = 1;
//     object->w = 4;
//     break;
//   case RIGHT:
//     default_matrice[0][2] = 1;
//     default_matrice[1][2] = 1;
//     default_matrice[2][2] = 1;
//     default_matrice[3][2] = 1;
//     object->h = 4;
//     object->w = 1;
//     break;
//   case DOWN:
//     default_matrice[2][0] = 1;
//     default_matrice[2][1] = 1;
//     default_matrice[2][2] = 1;
//     default_matrice[2][3] = 1;
//     object->h = 1;
//     object->w = 4;
//     break;
//   case LEFT:
//     default_matrice[0][1] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[2][1] = 1;
//     default_matrice[3][1] = 1;
//     object->h = 4;
//     object->w = 1;
//     break;
//   }
//   memcpy(object->matrice, default_matrice, OBJECT_MATRICE_SIZE * OBJECT_MATRICE_SIZE * sizeof(int));
// }

// void init_object_J(TObject *object, enum direction rotation, SDL_Point new_pos)
// {
//   int default_matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE] = {
//       0, 0, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0};
//   object->color = blue;
//   object->rotation = rotation;
//   object->pos.x = new_pos.x;
//   object->pos.y = new_pos.y;

//   switch (object->rotation)
//   {
//   case UP:
//     default_matrice[0][0] = 1;
//     default_matrice[1][0] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[1][2] = 1;
//     object->h = 2;
//     object->w = 3;
//     break;
//   case RIGHT:
//     default_matrice[0][1] = 1;
//     default_matrice[0][2] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[2][1] = 1;
//     object->h = 3;
//     object->w = 2;
//     break;
//   case DOWN:
//     default_matrice[1][0] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[1][2] = 1;
//     default_matrice[2][2] = 1;
//     object->h = 2;
//     object->w = 3;
//     break;
//   case LEFT:
//     default_matrice[0][1] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[2][0] = 1;
//     default_matrice[2][1] = 1;
//     object->h = 3;
//     object->w = 2;
//     break;
//   }
//   memcpy(object->matrice, default_matrice, OBJECT_MATRICE_SIZE * OBJECT_MATRICE_SIZE * sizeof(int));
// }

// void init_object_L(TObject *object, enum direction rotation, SDL_Point new_pos) {
//   int default_matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE] = {
//       0, 0, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0};
//   object->color = blue;
//   object->rotation = rotation;
//   object->pos.x = new_pos.x;
//   object->pos.y = new_pos.y;

//   switch (object->rotation)
//   {
//   case UP:
//     default_matrice[0][2] = 1;
//     default_matrice[1][0] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[1][2] = 1;
//     object->h = 2;
//     object->w = 3;
//     break;
//   case RIGHT:
//     default_matrice[0][1] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[2][1] = 1;
//     default_matrice[2][2] = 1;
//     object->h = 3;
//     object->w = 2;
//     break;
//   case DOWN:
//     default_matrice[1][0] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[1][2] = 1;
//     default_matrice[2][0] = 1;
//     object->h = 2;
//     object->w = 3;
//     break;
//   case LEFT:
//     default_matrice[0][0] = 1;
//     default_matrice[0][1] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[2][1] = 1;
//     object->h = 3;
//     object->w = 2;
//     break;
//   }
//   memcpy(object->matrice, default_matrice, OBJECT_MATRICE_SIZE * OBJECT_MATRICE_SIZE * sizeof(int));
// }

// void init_object_O(TObject *object, enum direction rotation, SDL_Point new_pos) {
//   int default_matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE] = {
//       1, 1, 0, 0,
//       1, 1, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0};
//   object->color = blue;
//   object->rotation = rotation;
//   object->pos.x = new_pos.x;
//   object->pos.y = new_pos.y;
//   object->h = 2;
//   object->w = 2;
//   memcpy(object->matrice, default_matrice, OBJECT_MATRICE_SIZE * OBJECT_MATRICE_SIZE * sizeof(int));
// }

// void init_object_T(TObject *object, enum direction rotation, SDL_Point new_pos)
// {
//     int default_matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE] = {
//       0, 0, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0};
//   object->color = blue;
//   object->rotation = rotation;
//   object->pos.x = new_pos.x;
//   object->pos.y = new_pos.y;

//   switch (object->rotation)
//   {
//   case UP:
//     default_matrice[0][1] = 1;
//     default_matrice[1][0] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[1][2] = 1;
//     object->h = 2;
//     object->w = 3;
//     break;
//   case RIGHT:
//     default_matrice[0][1] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[1][2] = 1;
//     default_matrice[2][1] = 1;
//     object->h = 3;
//     object->w = 2;
//     break;
//   case DOWN:
//     default_matrice[1][0] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[1][2] = 1;
//     default_matrice[2][1] = 1;
//     object->h = 2;
//     object->w = 3;
//     break;
//   case LEFT:
//     default_matrice[0][1] = 1;
//     default_matrice[1][0] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[2][1] = 1;
//     object->h = 3;
//     object->w = 2;
//     break;
//   }
//   memcpy(object->matrice, default_matrice, OBJECT_MATRICE_SIZE * OBJECT_MATRICE_SIZE * sizeof(int));
// }

// void init_object_S(TObject *object, enum direction rotation, SDL_Point new_pos)
// {
//   int default_matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE] = {
//       0, 0, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0};
//   object->color = green;
//   object->rotation = rotation;
//   object->pos.x = new_pos.x;
//   object->pos.y = new_pos.y;

//   switch (object->rotation)
//   {
//   case UP:
//     default_matrice[0][1] = 1;
//     default_matrice[0][2] = 1;
//     default_matrice[1][0] = 1;
//     default_matrice[1][1] = 1;
//     object->h = 2;
//     object->w = 3;
//     break;
//   case RIGHT:
//     default_matrice[0][1] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[1][2] = 1;
//     default_matrice[2][2] = 1;
//     object->h = 3;
//     object->w = 2;
//     break;
//   case DOWN:
//     default_matrice[1][1] = 1;
//     default_matrice[1][2] = 1;
//     default_matrice[2][0] = 1;
//     default_matrice[2][1] = 1;
//     object->h = 2;
//     object->w = 3;
//     break;
//   case LEFT:
//     default_matrice[0][0] = 1;
//     default_matrice[1][0] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[2][1] = 1;
//     object->h = 3;
//     object->w = 2;
//     break;
//   }
//   memcpy(object->matrice, default_matrice, OBJECT_MATRICE_SIZE * OBJECT_MATRICE_SIZE * sizeof(int));
// }

// void init_object_Z(TObject *object, enum direction rotation, SDL_Point new_pos)
// {
//   int default_matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE] = {
//       0, 0, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0,
//       0, 0, 0, 0};
//   object->color = blue;
//   object->rotation = rotation;
//   object->pos.x = new_pos.x;
//   object->pos.y = new_pos.y;

//   switch (object->rotation)
//   {
//   case UP:
//     default_matrice[0][0] = 1;
//     default_matrice[0][1] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[1][2] = 1;
//     object->h = 2;
//     object->w = 3;
//     break;
//   case RIGHT:
//     default_matrice[0][2] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[1][2] = 1;
//     default_matrice[2][1] = 1;
//     object->h = 3;
//     object->w = 2;
//     break;
//   case DOWN:
//     default_matrice[1][0] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[2][1] = 1;
//     default_matrice[2][2] = 1;
//     object->h = 2;
//     object->w = 3;
//     break;
//   case LEFT:
//     default_matrice[0][1] = 1;
//     default_matrice[1][0] = 1;
//     default_matrice[1][1] = 1;
//     default_matrice[2][0] = 1;
//     object->h = 3;
//     object->w = 2;
//     break;
//   }
//   memcpy(object->matrice, default_matrice, OBJECT_MATRICE_SIZE * OBJECT_MATRICE_SIZE * sizeof(int));
// }