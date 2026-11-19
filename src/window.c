#include "window.h"
#include <time.h>
#include "colors.h"
#include "config.h"
#include "draw.h"
#include "layout.h"

static bool point_in_rect(float x, float y, SDL_FRect rect)
{
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
}

// Size of the area of the desktop a window can use
static void desktop_size(int *width, int *height)
{
    SDL_Rect area;
    if (SDL_GetDisplayUsableBounds(0, &area) == 0 && area.w > 0 && area.h > 0)
    {
        *width = area.w;
        *height = area.h;
        return;
    }
    *width = 1280; // unknown desktop: assume something small
    *height = 720;
}

// Creates a resizable window with a renderer. Everything is drawn in the design size
// (design_width x design_height) and SDL scales it to the window, keeping the aspect ratio
// (black bars) and mapping mouse positions back to design coordinates.
// Prefers a vsynced hardware renderer, falls back to software.
static bool create_window(const char *title, int design_width, int design_height, Uint32 extra_flags, SDL_Window **window, SDL_Renderer **renderer)
{
    int width, height, avail_width, avail_height;
    desktop_size(&avail_width, &avail_height);
    fit_window_size(design_width, design_height, avail_width, avail_height, &width, &height);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); // smooth text when scaled down
    *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                               SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | extra_flags);
    if (!*window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetWindowMinimumSize(*window, design_width / 4, design_height / 4);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!*renderer)
    {
        *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_SOFTWARE);
    }
    if (!*renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        *window = NULL;
        return false;
    }
    SDL_RenderSetLogicalSize(*renderer, design_width, design_height);
    SDL_RenderSetIntegerScale(*renderer, SDL_FALSE);
    return true;
}

// F11: switch between fullscreen and a window that fits the desktop
static void toggle_fullscreen(SDL_Renderer *renderer)
{
    SDL_Window *window = SDL_RenderGetWindow(renderer);
    bool fullscreen = (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
    SDL_SetWindowFullscreen(window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
}

int window_select(void)
{
    int status = 0;

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init())
    {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        fprintf(stderr, "IMG_Init Error: %s\n", IMG_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    int select_window = MENU;
    while (select_window == MENU || select_window == GAME)
    {
        const bool menu = select_window == MENU;
        const int design_width = menu ? MENU_DESIGN_WIDTH : GAME_DESIGN_WIDTH;
        const int design_height = menu ? MENU_DESIGN_HEIGHT : GAME_DESIGN_HEIGHT;
        SDL_Window *window;
        SDL_Renderer *renderer;
        if (!create_window(menu ? "ZAM0074 - Tetris/menu_window" : "ZAM0074 - Tetris/game_window",
                           design_width, design_height,
                           menu ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP,
                           &window, &renderer))
        {
            status = 1;
            break;
        }

        if (menu)
        {
            select_window = main_menu(renderer, design_width, design_height);
        }
        else
        {
            do // a restart plays again in the same window
            {
                select_window = game_infinite_loop(renderer, design_width, design_height);
            } while (select_window == GAME);
        }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
    if (select_window == SCREEN_ERROR)
    {
        status = 1;
    }

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return status;
}

// Writes one "action: key" row for every action, in the order they are listed in the menu
static void format_bind_rows(char rows[BINDS_NUM][SETTINGS_ROW_SIZE], const TKeyMap *keymap)
{
    for (int i = 0; i < BINDS_NUM; i++)
    {
        snprintf(rows[i], SETTINGS_ROW_SIZE, "%s: %s", action_name((Action)i), SDL_GetKeyName(keymap->keys[i]));
    }
}

int main_menu(SDL_Renderer *renderer, int window_width, int window_height)
{
    SDL_FRect best_scores = {
        .x = 2 * SPACING_WIDTH,
        .y = 2 * SPACING_WIDTH,
        .w = (window_width - 6 * SPACING_WIDTH) / 3,
        .h = window_height - 4 * SPACING_WIDTH};
    SDL_FPoint start_button = {
        .x = window_width,
        .y = (window_height - TITLE_SIZE - 4 * SPACING_WIDTH) / 2};
    SDL_FRect config_box = {
        .x = (window_width - 6 * SPACING_WIDTH) * 2 / 3,
        .y = 2 * SPACING_WIDTH,
        .w = window_width - 2 * SPACING_WIDTH - (window_width - 6 * SPACING_WIDTH) * 2 / 3,
        .h = window_height - 4 * SPACING_WIDTH};
    SDL_FRect edit_block = {
        .x = (window_width - 10 * SQUARE_SIZE) / 2,
        .y = (window_height - (2 * TITLE_SIZE + SQUARE_SIZE)) / 2,
        .h = 2 * TEXT_SIZE + SQUARE_SIZE,
        .w = 10 * SQUARE_SIZE};

    TTF_Font *title_font = TTF_OpenFont(FONT, 48);
    TTF_Font *data_font = TTF_OpenFont(FONT, TEXT_SIZE);
    if (!title_font || !data_font)
    {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        if (title_font)
        {
            TTF_CloseFont(title_font);
        }
        if (data_font)
        {
            TTF_CloseFont(data_font);
        }
        return SCREEN_ERROR;
    }

    TKeyMap keymap;
    if (!keymap_load(&keymap, CFG_FILE))
    {
        fprintf(stderr, "Could not read %s, using default keys\n", CFG_FILE);
    }
    char binds_rows[BINDS_NUM][SETTINGS_ROW_SIZE];
    const char *binds_texts[BINDS_NUM];
    format_bind_rows(binds_rows, &keymap);
    for (int i = 0; i < BINDS_NUM; i++)
    {
        binds_texts[i] = binds_rows[i];
    }

    int scores[BEST_SCORE_NUM];
    char score_rows[BEST_SCORE_NUM][SETTINGS_ROW_SIZE];
    const char *best_scores_text[BEST_SCORE_NUM];
    int text_num = scores_load(SCORE_FILE, scores);
    if (text_num == 0)
    {
        snprintf(score_rows[0], SETTINGS_ROW_SIZE, "%s", NO_SCORE);
        text_num = 1;
    }
    else
    {
        for (int i = 0; i < text_num; i++)
        {
            snprintf(score_rows[i], SETTINGS_ROW_SIZE, "%d", scores[i]);
        }
    }
    for (int i = 0; i < text_num; i++)
    {
        best_scores_text[i] = score_rows[i];
    }

    TConfigButton config_buttons[BINDS_NUM] = {0};
    SDL_FRect edit_buttons[2] = {0};
    SDL_FRect start_button_pos = {0};

    SDL_Event e;
    bool editing = false;
    Action edited_action = ACTION_NONE;
    SDL_Keycode new_key = SDLK_UNKNOWN;
    int next_screen = SCREEN_QUIT;
    bool running = true;
    while (running)
    {
        draw_background(renderer, black);
        draw_title_texts(renderer, best_scores, title_font, data_font, text_num, BEST_SCORES, best_scores_text);
        start_button_pos = draw_button(renderer, start_button, green, white, title_font, START);
        draw_title_config_box(renderer, config_box, title_font, data_font, CONFIG, binds_texts, config_buttons, BINDS_NUM);

        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                next_screen = SCREEN_QUIT;
                running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (e.button.button != SDL_BUTTON_LEFT)
                {
                    break;
                }
                if (point_in_rect(e.button.x, e.button.y, start_button_pos))
                {
                    next_screen = GAME;
                    running = false;
                    break;
                }
                if (editing)
                {
                    if (new_key != SDLK_UNKNOWN && point_in_rect(e.button.x, e.button.y, edit_buttons[0]))
                    {
                        // Submit: use the new key for the action and store it
                        keymap.keys[edited_action] = new_key;
                        if (!keymap_save(&keymap, CFG_FILE, TMP_FILE))
                        {
                            fprintf(stderr, "Could not save %s\n", CFG_FILE);
                        }
                        format_bind_rows(binds_rows, &keymap);
                        editing = false;
                    }
                    else if (point_in_rect(e.button.x, e.button.y, edit_buttons[1]) || !point_in_rect(e.button.x, e.button.y, edit_block))
                    {
                        editing = false; // Cancel button or click outside of the edit block
                    }
                }
                // Edit Config buttons
                for (int i = 0; i < BINDS_NUM; i++)
                {
                    if (point_in_rect(e.button.x, e.button.y, config_buttons[i].button_pos))
                    {
                        editing = true;
                        edited_action = (Action)i;
                        new_key = SDLK_UNKNOWN;
                        break;
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_F11)
                {
                    toggle_fullscreen(renderer); // F11 is reserved, it cannot be bound
                }
                else if (editing && e.key.keysym.sym == SDLK_ESCAPE)
                {
                    editing = false; // escape is reserved for pausing the game
                }
                else if (editing && e.key.keysym.sym != SDLK_UNKNOWN)
                {
                    new_key = e.key.keysym.sym;
                }
                break;
            }
        }
        if (editing)
        {
            draw_edit_block(renderer, edit_block, data_font, NEW_BIND,
                            new_key == SDLK_UNKNOWN ? "" : SDL_GetKeyName(new_key), edit_buttons);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(1); // do not spin the CPU when vsync is not available
    }

    TTF_CloseFont(title_font);
    TTF_CloseFont(data_font);
    return next_screen;
}

int game_infinite_loop(SDL_Renderer *renderer, int window_width, int window_height)
{
    SDL_FPoint object_size = {
        .x = SQUARE_SIZE * 4 + SPACING_WIDTH * 7,              // Object box width
        .y = SQUARE_SIZE * 2 + SPACING_WIDTH * 10 + TITLE_SIZE // Object box height
    };
    SDL_FPoint matrice_size = {
        .y = FIELD_HEIGHT * (SQUARE_SIZE + SPACING_WIDTH),
        .x = FIELD_WIDTH * (SQUARE_SIZE + SPACING_WIDTH),
    };
    SDL_FPoint starting_pos = {
        .x = 0.5 * (window_width - (SPACING_WIDTH + object_size.x + SPACING_WIDTH + matrice_size.x + SPACING_WIDTH + object_size.x + SPACING_WIDTH)),
        .y = 0.5 * (window_height - (SPACING_WIDTH + matrice_size.y + SPACING_WIDTH))};
    const SDL_FRect hold_box = {
        .h = object_size.y,
        .w = object_size.x, // ||___||___||___||___||
        .x = starting_pos.x,
        .y = starting_pos.y};
    const SDL_FRect matrice_box = {
        .h = matrice_size.y,
        .w = matrice_size.x,
        .x = hold_box.w + hold_box.x + SPACING_WIDTH,
        .y = hold_box.y};
    const SDL_FRect next_box = {
        .h = object_size.y,
        .w = object_size.x, // ||___||___||___||___||
        .x = matrice_box.w + matrice_box.x,
        .y = matrice_box.y};
    const SDL_FRect score_box = {
        .h = (TITLE_SIZE + TEXT_SIZE),
        .w = hold_box.w,
        .x = hold_box.x,
        .y = matrice_box.h + matrice_box.y};
    const SDL_FRect cog_img_box = {
        .h = ICON_SIZE,
        .w = ICON_SIZE,
        .x = next_box.x + next_box.w - ICON_SIZE,
        .y = matrice_box.h + matrice_box.y - ICON_SIZE};
    const SDL_FRect binds_box = {
        .h = BINDS_NUM * (ICON_SIZE + SPACING_WIDTH),
        .w = next_box.w,
        .x = next_box.x,
        .y = cog_img_box.y - cog_img_box.h - BINDS_NUM * (ICON_SIZE + SPACING_WIDTH),
    };
    const SDL_FRect game_over_box = {
        .h = TITLE_SIZE,
        .w = window_width,
        .x = 0,
        .y = (window_height - TITLE_SIZE) / 2,
    };
    SDL_FPoint resume_button = {
        .x = window_width,
        .y = game_over_box.y};
    SDL_FPoint main_menu_button = {
        .x = window_width,
        .y = game_over_box.y + game_over_box.h + SPACING_WIDTH * 2};
    SDL_FPoint restart_button = {
        .x = window_width,
        .y = main_menu_button.y + TITLE_SIZE + 2 * SPACING_WIDTH};
    SDL_FPoint game_field_pos = {
        .x = matrice_box.x,
        .y = matrice_box.y,
    };

    // Which icon belongs to which action in the binds box
    static const struct
    {
        Action action;
        const char *icon_path;
    } bind_rows[BINDS_NUM] = {
        {ACTION_MOVE_DOWN, ARROW_DOWN},
        {ACTION_MOVE_LEFT, ARROW_LEFT},
        {ACTION_MOVE_RIGHT, ARROW_RIGHT},
        {ACTION_MOVE_HOLD, HOLD},
        {ACTION_ROTATE_LEFT, ROTATE_CC},
        {ACTION_ROTATE_RIGHT, ROTATE_CW},
    };

    // Resources are released at the cleanup label, whichever way the screen is left
    int next_screen = SCREEN_ERROR;
    TTF_Font *title_font = NULL;
    TTF_Font *data_font = NULL;
    SDL_Texture *pause_icon = NULL;
    SDL_Texture *play_icon = NULL;
    TIconText binds[BINDS_NUM] = {0};

    title_font = TTF_OpenFont(FONT, TITLE_SIZE);
    data_font = TTF_OpenFont(FONT, TEXT_SIZE);
    if (!title_font || !data_font)
    {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        goto cleanup;
    }

    TKeyMap keymap;
    if (!keymap_load(&keymap, CFG_FILE))
    {
        fprintf(stderr, "Could not read %s, using default keys\n", CFG_FILE);
    }
    pause_icon = load_icon(renderer, PAUSE);
    play_icon = load_icon(renderer, PLAY);
    for (int i = 0; i < BINDS_NUM; i++)
    {
        binds[i].name = action_name(bind_rows[i].action);
        snprintf(binds[i].key_name, sizeof(binds[i].key_name), "%s", SDL_GetKeyName(keymap.keys[bind_rows[i].action]));
        binds[i].icon = load_icon(renderer, bind_rows[i].icon_path);
    }

    // Different seed on every start, also for restarts within the same second
    TGame game;
    game_init(&game, (unsigned int)(SDL_GetPerformanceCounter() ^ (Uint64)time(NULL)));

    enum
    {
        PLAYING,
        PAUSED,
        OVER
    } state = PLAYING;
    bool score_saved = false;
    SDL_FRect resume_button_pos = {0}; // button rects of the last drawn frame, used for clicks
    SDL_FRect main_button_pos = {0};
    SDL_FRect restart_button_pos = {0};
    Uint32 last_gravity_tick = SDL_GetTicks();
    SDL_Event e;
    bool running = true;

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                next_screen = SCREEN_QUIT;
                running = false;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                const float x = e.button.x;
                const float y = e.button.y;
                if (state != OVER && point_in_rect(x, y, cog_img_box))
                {
                    state = state == PAUSED ? PLAYING : PAUSED;
                    last_gravity_tick = SDL_GetTicks(); // time spent paused does not count
                }
                else if (state == PAUSED && point_in_rect(x, y, resume_button_pos))
                {
                    state = PLAYING;
                    last_gravity_tick = SDL_GetTicks();
                }
                else if (state != PLAYING && point_in_rect(x, y, main_button_pos))
                {
                    next_screen = MENU;
                    running = false;
                }
                else if (state != PLAYING && point_in_rect(x, y, restart_button_pos))
                {
                    next_screen = GAME;
                    running = false;
                }
            }
            else if (e.type == SDL_KEYDOWN)
            {
                const SDL_Keycode key = e.key.keysym.sym;
                if (key == SDLK_F11)
                {
                    toggle_fullscreen(renderer);
                }
                else if (key == SDLK_ESCAPE)
                {
                    if (state != OVER)
                    {
                        state = state == PAUSED ? PLAYING : PAUSED;
                        last_gravity_tick = SDL_GetTicks();
                    }
                }
                else if (state == PLAYING)
                {
                    switch (keymap_action(&keymap, key))
                    {
                    case ACTION_MOVE_DOWN:
                        game_soft_drop(&game);
                        break;
                    case ACTION_MOVE_LEFT:
                        game_move(&game, -1);
                        break;
                    case ACTION_MOVE_RIGHT:
                        game_move(&game, 1);
                        break;
                    case ACTION_MOVE_HOLD:
                        game_hold(&game);
                        break;
                    case ACTION_ROTATE_LEFT:
                        game_rotate(&game, -1);
                        break;
                    case ACTION_ROTATE_RIGHT:
                        game_rotate(&game, 1);
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        if (state == PLAYING)
        {
            const Uint32 current_tick = SDL_GetTicks();
            if (current_tick - last_gravity_tick >= (Uint32)game_gravity_delay_ms(&game))
            {
                game_gravity(&game);
                last_gravity_tick = current_tick;
            }
        }

        if (game.game_over && state != OVER)
        {
            state = OVER;
        }
        if (state == OVER && !score_saved)
        {
            score_saved = true;
            if (!scores_record(SCORE_FILE, game.score))
            {
                fprintf(stderr, "Could not save the score to %s\n", SCORE_FILE);
            }
        }

        // Score and lines text
        char level_str[WORD_SIZE];
        char score_str[WORD_SIZE];
        char lines_str[WORD_SIZE];
        snprintf(level_str, sizeof(level_str), "%d", game.level);
        snprintf(score_str, sizeof(score_str), "%d", game.score);
        snprintf(lines_str, sizeof(lines_str), "%d", game.lines);
        TDataText data_texts[3] = {
            {LEVEL_TEXT, level_str},
            {SCORE_TEXT, score_str},
            {LINES_TEXT, lines_str}};

        const TObject next_object = object_create(game.next);
        const TObject held_object = object_create(game.held);

        draw_background(renderer, black);

        draw_object_box(renderer, light.secondary, hold_box, game.has_held ? &held_object : NULL, HOLD_BOX_TEXT, title_font); // Hold object box
        draw_text_box(renderer, title_font, data_font, data_texts, 3, score_box);                                              // Score box
        draw_playing_field(renderer, game.field, game_field_pos);                                                              // Game matrice
        draw_object_matrice(renderer, game_field_pos, &game.current);                                                          // Object
        draw_object_box(renderer, light.secondary, next_box, &next_object, NEXT_BOX_TEXT, title_font);                         // Next object box
        draw_icon_text_block(renderer, binds_box, binds, BINDS_NUM, title_font, data_font, white);                             // Binds box
        draw_icon(renderer, cog_img_box, state == PAUSED ? play_icon : pause_icon);                                            // Play / Pause icon

        if (state == PLAYING)
        {
            resume_button_pos = main_button_pos = restart_button_pos = (SDL_FRect){0};
        }
        else
        {
            if (state == PAUSED)
            {
                resume_button_pos = draw_button(renderer, resume_button, orange, white, title_font, RESUME);
            }
            else
            {
                resume_button_pos = (SDL_FRect){0};
                draw_text(renderer, game_over_box, GAME_OVER_TEXT, title_font, white, true); // Game Over text
            }
            main_button_pos = draw_button(renderer, main_menu_button, blue, white, title_font, MAIN_MENU);
            restart_button_pos = draw_button(renderer, restart_button, green, white, title_font, RESTART);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(1); // do not spin the CPU when vsync is not available
    }

    // Leaving a round early still counts its score
    if (!score_saved)
    {
        scores_record(SCORE_FILE, game.score);
    }

cleanup:
    for (int i = 0; i < BINDS_NUM; i++)
    {
        if (binds[i].icon)
        {
            SDL_DestroyTexture(binds[i].icon);
        }
    }
    if (pause_icon)
    {
        SDL_DestroyTexture(pause_icon);
    }
    if (play_icon)
    {
        SDL_DestroyTexture(play_icon);
    }
    if (title_font)
    {
        TTF_CloseFont(title_font);
    }
    if (data_font)
    {
        TTF_CloseFont(data_font);
    }
    return next_screen;
}
