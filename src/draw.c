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
    const SDL_FRect inner = {
        .x = box.x + SPACING_WIDTH,
        .y = box.y + SPACING_WIDTH,
        .w = box.w - SPACING_WIDTH * 2,
        .h = box.h - SPACING_WIDTH * 2};

    const SDL_FRect text_rect = {
        .x = inner.x,
        .y = inner.y + SPACING_WIDTH,
        .w = inner.w,
        .h = TITLE_SIZE * 0.8125};

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
    draw_text(renderer, text_rect, text, font, white, true);

    SDL_FPoint pos;

    for (int i = 0; i < OBJECT_MATRICE_SIZE; i++)
    {
        pos.y = i * (SQUARE_SIZE + SPACING_WIDTH) + 0.5 * ((inner.h + inner.y - object.h));
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

void draw_text_box(SDL_Renderer *renderer, TTF_Font *title_font, TTF_Font *data_font, TDataText text[], int text_num, SDL_FRect rect)
{
    SDL_FRect outer = {
        .h = rect.h * text_num,
        .w = rect.w,
        .x = rect.x,
        .y = rect.y - (TITLE_SIZE + TEXT_SIZE) * text_num - SPACING_WIDTH};
    SDL_FRect inner = {
        .x = outer.x + SPACING_WIDTH,
        .y = outer.y + SPACING_WIDTH,
        .h = outer.h - 2 * SPACING_WIDTH,
        .w = outer.w - 2 * SPACING_WIDTH};
    int title_ypos = inner.y;
    int data_ypos = 0;

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
        const SDL_FRect title_pos = {
            .x = inner.x,
            .y = title_ypos,
            .h = TITLE_SIZE * 0.8125,
            .w = inner.w};
        data_ypos = title_pos.y + title_pos.h;

        const SDL_FRect data_pos = {
            .x = inner.x,
            .y = data_ypos,
            .h = TEXT_SIZE * 0.8125,
            .w = inner.w};

        title_ypos = data_pos.y + data_pos.h;

        const SDL_FRect data_bg_rect = {
            .x = inner.x,
            .y = title_pos.y + title_pos.h,
            .w = inner.w,
            .h = data_pos.h};
        SDL_SetRenderDrawColor(
            renderer,
            dark.secondary.r,
            dark.secondary.g,
            dark.secondary.b,
            dark.secondary.a);
        SDL_RenderFillRectF(renderer, &data_bg_rect);

        draw_text(renderer, title_pos, text[i].title, title_font, white, true);
        draw_text(renderer, data_pos, text[i].data, data_font, light.primary, false);
    }
}

int draw_icon(SDL_Renderer *renderer, SDL_FRect rect, char *file_path)
{
    SDL_Surface *surface = IMG_Load(file_path);
    if (!surface)
    {
        fprintf(stderr, "TTF_RenderText_Solid Error: %s\n", TTF_GetError());
        return 0;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", IMG_GetError());
        return 0;
    }

    SDL_RenderCopyF(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    return 1;
}

int draw_text(SDL_Renderer *renderer, SDL_FRect rect, char *text, TTF_Font *font, SDL_Color color, bool center)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface)
    {
        fprintf(stderr, "TTF_RenderText_Solid Error: %s\n", TTF_GetError());
        return 0;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", TTF_GetError());
        return 0;
    }
    SDL_FRect text_rect = {
        .x = rect.x + SPACING_WIDTH,
        .y = rect.y,
        .w = surface->w,
        .h = surface->h};

    if (center)
    {
        text_rect.x = rect.x + 0.5 * (rect.w - surface->w);
    }

    SDL_RenderCopyF(renderer, texture, NULL, &text_rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    return 1;
}

void draw_icon_text(SDL_Renderer *renderer, SDL_FRect rect, TIconText text, TTF_Font *font, TTF_Font *long_text_font, SDL_Color color)
{
    char *alt_text;
    SDL_FRect icon_rect = {
        .x = rect.x,
        .y = rect.y,
        .h = ICON_SIZE,
        .w = ICON_SIZE};
    SDL_FRect text_rect = {
        .x = icon_rect.x + icon_rect.w,
        .y = icon_rect.y + 0.5 * (ICON_SIZE - TITLE_SIZE * 0.8125),
        .w = rect.w - icon_rect.w - SPACING_WIDTH,
        .h = ICON_SIZE};


    if (!draw_icon(renderer, icon_rect, text.icon_path))
    {
        text_rect.x = rect.x;
        text_rect.w = rect.w;
        alt_text = (char *)malloc(sizeof(text.text) + 2 * sizeof(char));
        sprintf(alt_text, "%s: %c", text.text.name, text.text.bind);
    }
    else
    {
        alt_text = (char *)malloc(sizeof(text.text.bind) + 3 * sizeof(char));
        sprintf(alt_text, "%s", text.text.bind);
    }
    if (strlen(alt_text) > 6)
    {
        text_rect.y = icon_rect.y + 0.5 * (ICON_SIZE - TEXT_SIZE * 0.8125);
        draw_text(renderer, text_rect, alt_text, long_text_font, color, false);
    }
    else
    {
        draw_text(renderer, text_rect, alt_text, font, color, false);
    }
}

void draw_icon_text_block(SDL_Renderer *renderer, SDL_FRect rect, TIconText texts[], int texts_num, TTF_Font *font, TTF_Font *long_text_font, SDL_Color color)
{
    int ypos = rect.y;

    for (int i = 0; i < texts_num; i++)
    {
        SDL_FRect bind_rect = {
            .x = rect.x,
            .h = ICON_SIZE,
            .w = rect.w,
            .y = ypos};
        draw_icon_text(renderer, bind_rect, texts[i], font, long_text_font, color);
        ypos += bind_rect.h + SPACING_WIDTH;
    }
}