#include "draw.h"

void draw_background(SDL_Renderer *renderer, SDL_Color color)
{
    SDL_SetRenderDrawColor(
        renderer,
        color.r,
        color.g,
        color.b,
        color.a);

    SDL_RenderClear(renderer);
}

void draw_object_box(SDL_Renderer *renderer, SDL_Color color, const SDL_FRect box, TObject object, char *text, TTF_Font *font)
{
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, white);
    if (!text_surface)
    {
        fprintf(stderr, "TTF_RenderText_Solid Error: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text_texture)
    {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", TTF_GetError());
        return;
    }

    const SDL_FRect inner = {
        .x = box.x + SPACING_WIDTH,
        .y = box.y + SPACING_WIDTH,
        .w = box.w - SPACING_WIDTH * 2,
        .h = box.h - SPACING_WIDTH * 2};

    const SDL_Rect text_rect = {
        .x = box.x + SPACING_WIDTH + (box.w - SPACING_WIDTH * 2 - text_surface->w) / 2,
        .y = box.y + SPACING_WIDTH * 2,
        .w = text_surface->w,
        .h = text_surface->h};

    const SDL_FRect rect = {
        .x = (0.5 * inner.w + inner.x) - 0.5 * (object.w * SQUARE_SIZE + (object.w - 1) * SPACING_WIDTH),
        .y = 0.5 * ((inner.h + inner.y + 3 * SPACING_WIDTH)) - 0.5 * text_rect.h,
        .w = box.w - SPACING_WIDTH * 2,
        .h = 2 * (SQUARE_SIZE) + SPACING_WIDTH};

    SDL_SetRenderDrawColor(
        renderer,
        color.r,
        color.g,
        color.b,
        color.a);
    SDL_RenderFillRectF(renderer, &box);

    SDL_SetRenderDrawColor(
        renderer,
        black.r,
        black.g,
        black.b,
        black.a);
    SDL_RenderFillRectF(renderer, &inner);
    // SDL_SetRenderDrawColor(
    //     renderer,
    //     orange.primary.r,
    //     orange.primary.g,
    //     orange.primary.b,
    //     orange.primary.a);
    // SDL_RenderDrawRectF(renderer, &rect);

    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

    SDL_FPoint pos;

    for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
    {
        pos.y = i * (SQUARE_SIZE + SPACING_WIDTH) + 0.5 * ((inner.h + inner.y + 3 * SPACING_WIDTH)) - 0.5 * text_rect.h;
        if (object.h == 1)
        {
            pos.y -= (SQUARE_SIZE + SPACING_WIDTH) / 2;
        }
        for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
        {
            pos.x = j * (SQUARE_SIZE + SPACING_WIDTH) + (0.5 * inner.w + inner.x) - 0.5 * (object.w * SQUARE_SIZE + (object.w - 1) * SPACING_WIDTH);
            if (object.matrice[i][j] == 1)
            {
                draw_rectangle(renderer, pos, *object.color);
            }
        }
    }

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
}

void draw_rectangle(SDL_Renderer *renderer, SDL_FPoint move, TColor color)
{
    SDL_SetRenderDrawColor(
        renderer,
        color.secondary.r,
        color.secondary.g,
        color.secondary.b,
        color.secondary.a);

    const SDL_FRect square1_1 = {move.x, move.y, SQUARE_SIZE, SQUARE_SIZE};
    SDL_RenderFillRectF(
        renderer,
        &square1_1);

    SDL_SetRenderDrawColor(
        renderer,
        color.primary.r,
        color.primary.g,
        color.primary.b,
        color.primary.a);

    const SDL_FRect square1_2 = {move.x, move.y, SQUARE_SIZE - SHADE_WIDTH, SQUARE_SIZE - SHADE_WIDTH};
    SDL_RenderFillRectF(
        renderer,
        &square1_2);
}

void draw_playing_field(SDL_Renderer *renderer, int matrice[FIELD_HEIGHT][FIELD_WIDTH], SDL_FPoint pos)
{
    SDL_FPoint square_pos;
    for (int i = 0; i < FIELD_HEIGHT; i++)
    {
        square_pos.y = i * (SQUARE_SIZE + SPACING_WIDTH) + pos.y;
        for (int j = 0; j < FIELD_WIDTH; j++)
        {
            square_pos.x = j * (SQUARE_SIZE + SPACING_WIDTH) + pos.x;
            if (matrice[i][j])
            {
                if (j == 0 || j == FIELD_WIDTH - 1 || i == 0 || i == FIELD_HEIGHT - 1)
                {

                    draw_rectangle(renderer, square_pos, dark);
                }
                else
                {
                    draw_rectangle(renderer, square_pos, light);
                }
            }
        }
    }
}

void draw_object_matrice(SDL_Renderer *renderer, SDL_FPoint offset, TObject object)
{
    SDL_FPoint square_pos;
    for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
    {
        square_pos.y = offset.y + (i + object.pos.y) * (SQUARE_SIZE + SPACING_WIDTH);
        for (int j = 0; j < OBJECT_MATRICE_SIZE; j++)
        {
            if (object.pos.y + i > FIELD_HEIGHT || object.pos.y + i < 0 || object.pos.x + j > FIELD_WIDTH || object.pos.x + j < 0)
            {
                continue;
            }
            square_pos.x = offset.x + (j + object.pos.x) * (SQUARE_SIZE + SPACING_WIDTH);
            if (object.matrice[i][j])
            {
                draw_rectangle(renderer, square_pos, *object.color);
            }
        }
    }
}

// void draw_color_palette(SDL_Renderer *renderer)
// {
//     int k = 0, j = 1, i = 1;
//     TColor color[] = {
//         light,
//         dark,
//         red,
//         yellow,
//         green,
//         cyan,
//         blue,
//         purple,
//         orange};

//     while (k < 27)
//     {
//         if (k < 9)
//         {
//             draw_rectangle(renderer, j, i, color[k]);
//         }
//         else
//         {
//             if (k < 18)
//             {
//                 SDL_SetRenderDrawColor( // light red
//                     renderer,
//                     color[k - 9].primary.r,
//                     color[k - 9].primary.g,
//                     color[k - 9].primary.b,
//                     color[k - 9].primary.a);
//             }
//             else
//             {
//                 SDL_SetRenderDrawColor( // light red
//                     renderer,
//                     color[k - 18].secondary.r,
//                     color[k - 18].secondary.g,
//                     color[k - 18].secondary.b,
//                     color[k - 18].secondary.a);
//             }
//             const SDL_Rect square1_2 = {SQUARE_SIZE * j + SPACING_WIDTH * j, SQUARE_SIZE * i + SPACING_WIDTH * i, SQUARE_SIZE - SHADE_WIDTH, SQUARE_SIZE - SHADE_WIDTH};

//             SDL_RenderFillRect(
//                 renderer,
//                 &square1_2);
//         }
//         j++;
//         k++;
//         if (k == 9 || k == 18)
//         {
//             i++;
//             j = 1;
//         }
//         else
//         {
//             if (j > FIELD_WIDTH)
//             {
//                 j = 1;
//                 i++;
//             }
//         }
//     }
// }

void draw_text_box(SDL_Renderer *renderer, TTF_Font *title_font, TTF_Font *data_font, TDataText text[], int text_num, SDL_FRect rect)
{
    SDL_FRect outer = {
        .h = rect.h * text_num,
        .w = rect.w,
        .x = rect.x,
        .y = rect.y - (TEXT_SIZE + TEXT_SIZE / 2) * text_num - SPACING_WIDTH};
    SDL_FRect inner = {
        .x = outer.x + SPACING_WIDTH,
        .y = outer.y + SPACING_WIDTH,
        .h = outer.h - 2 * SPACING_WIDTH,
        .w = outer.w - 2 * SPACING_WIDTH};
    int title_ypos = inner.y;
    int data_ypos = 0;

    SDL_Rect box = {
        .x = inner.x,
        .y = title_ypos,
        .w = inner.w,
        .h = TEXT_SIZE,
    };

    SDL_SetRenderDrawColor(
        renderer,
        light.secondary.r,
        light.secondary.g,
        light.secondary.b,
        light.secondary.a);
    SDL_RenderFillRectF(renderer, &outer);
    SDL_SetRenderDrawColor(
        renderer,
        black.r,
        black.g,
        black.b,
        black.a);
    SDL_RenderFillRectF(renderer, &inner);

    for (int i = 0; i < text_num; i++)
    {
        SDL_Surface *title_surface = TTF_RenderText_Solid(title_font, text[i].title, white);
        if (!title_surface)
        {
            fprintf(stderr, "TTF_RenderText_Solid Error: %s\n", TTF_GetError());
            return;
        }
        SDL_Texture *title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
        if (!title_texture)
        {
            fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", TTF_GetError());
            return;
        }
        SDL_Surface *data_surface = TTF_RenderText_Solid(data_font, text[i].data, light.primary);
        if (!data_surface)
        {
            fprintf(stderr, "TTF_RenderText_Solid Error: %s\n", TTF_GetError());
            return;
        }
        SDL_Texture *data_texture = SDL_CreateTextureFromSurface(renderer, data_surface);
        if (!data_texture)
        {
            fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", TTF_GetError());
            return;
        }

        const SDL_Rect title_rect = {
            .x = 0.5 * (inner.x + inner.w) - 0.5 * title_surface->w,
            .y = title_ypos,
            .w = title_surface->w,
            .h = title_surface->h};
        data_ypos = title_rect.y + title_rect.h;
        const SDL_Rect data_rect = {
            .x = inner.x + SPACING_WIDTH,
            .y = data_ypos,
            .w = data_surface->w,
            .h = data_surface->h};
        const SDL_FRect data_bg_rect = {
            .x = inner.x,
            .y = title_rect.y + title_rect.h,
            .w = inner.w,
            .h = data_surface->h};
        title_ypos = data_rect.y + data_rect.h;
        SDL_SetRenderDrawColor(
            renderer,
            dark.secondary.r,
            dark.secondary.g,
            dark.secondary.b,
            dark.secondary.a);
        SDL_RenderFillRectF(renderer, &data_bg_rect);

        SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
        SDL_RenderCopy(renderer, data_texture, NULL, &data_rect);

        SDL_DestroyTexture(title_texture);
        SDL_FreeSurface(title_surface);
        SDL_DestroyTexture(data_texture);
        SDL_FreeSurface(data_surface);
    }
}