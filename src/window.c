#include "window.h"

int window_select()
{
    int select_window = MENU;
    int window_width,
        window_height;

    // SDL Initialization
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    { // If Initialization Failed, return 1
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init())
    {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        fprintf(stderr, "IMG_Init Error: %s\n", IMG_GetError());
        return 1;
    }
    do
    {
        if (select_window == MENU)
        {
            SDL_Window *menu_window = SDL_CreateWindow(
                "ZAM0074 - Tetris/menu_window", // Window title
                100,                            // Y coords
                100,                            // X coords
                800,                            // Default window width
                600,                            // Default window height
                SDL_WINDOW_SHOWN                // Show window right after creation
            );
            // If Creation Failed, return 1
            if (!menu_window)
            {
                fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
                TTF_Quit();
                IMG_Quit();
                SDL_Quit();
                return 1;
            }
            SDL_Renderer *menu_renderer = SDL_CreateRenderer(
                menu_window,
                -1,
                SDL_RENDERER_ACCELERATED //| SDL_RENDERER_PRESENTVSYNC // Hardware acceleration,  VSYNC enabled
            );
            // If Creation Failed, return 1
            if (!menu_renderer)
            {
                SDL_DestroyWindow(menu_window);
                fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
                TTF_Quit();
                IMG_Quit();
                SDL_Quit();
                return 1;
            }
            SDL_GetWindowSize(menu_window, &window_width, &window_height); // Get window width and height
            select_window = main_menu(menu_renderer, window_width, window_height);
            SDL_DestroyRenderer(menu_renderer);
            SDL_DestroyWindow(menu_window);
        }
        if (select_window == GAME)
        {
            // Create SDL Window
            SDL_Window *game_window = SDL_CreateWindow(
                "ZAM0074 - Tetris/game_window", // Window title
                100,                            // Y coords
                100,                            // X coords
                1920,                           // Default window width
                1080,                           // Default window height
                // next_box.x + next_box.w + SPACING_WIDTH, // Window width - based on size and position of next object box
                // matrice_box.h + matrice_box.y * 2,       // Window height - based on spacing around matrice and matrice height
                SDL_WINDOW_FULLSCREEN_DESKTOP // Show window right after creation
            );
            if (!game_window)
            {
                fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
                SDL_Quit();
                return 1;
            }
            // Create SDL Renderer
            SDL_Renderer *game_renderer = SDL_CreateRenderer(
                game_window,
                -1,
                SDL_RENDERER_ACCELERATED //| SDL_RENDERER_PRESENTVSYNC // Hardware acceleration,  VSYNC enabled
            );

            // If Creation Failed, return 1
            if (!game_renderer)
            {
                SDL_DestroyWindow(game_window);
                fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
                SDL_Quit();
                return 1;
            }
            SDL_GetWindowSize(game_window, &window_width, &window_height); // Get window width and height
            do
            {
                select_window = game_infinite_loop(game_renderer, window_width, window_height);
            } while (select_window == GAME);
            SDL_DestroyRenderer(game_renderer);
            SDL_DestroyWindow(game_window);
        }
    } while (select_window == GAME || select_window == MENU);

    TTF_Quit();
    SDL_Quit();
    IMG_Quit();
    return 0;
}

int SDL_rand(int max)
{
    return rand() % (max + 1);
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

    TTF_Font *title_font = TTF_OpenFont(FONT, 48);
    if (!title_font)
    {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        return -1;
    }
    TTF_Font *data_font = TTF_OpenFont(FONT, TEXT_SIZE);
    if (!data_font)
    {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        TTF_CloseFont(title_font);
        return -1;
    }
    char row[SETTINGS_ROW_SIZE];
    int text_num = 0;

    char **best_scores_text = malloc(sizeof(char *) * BEST_SCORE_NUM);
    if (!best_scores_text)
    {
        printf("Could not allocate memory\n");
        TTF_CloseFont(title_font);
        TTF_CloseFont(data_font);
        return -1;
    }
    for (int i = 0; i < BEST_SCORE_NUM; i++)
    {
        best_scores_text[i] = malloc(SETTINGS_ROW_SIZE);
        if (!best_scores_text[i])
        {
            printf("Could not allocate memory\n");
            for (int j = 0; j < i; j++)
            {
                free(best_scores_text[j]);
                best_scores_text[j] = NULL;
            }
            free(best_scores_text);
            best_scores_text = NULL;
            TTF_CloseFont(title_font);
            TTF_CloseFont(data_font);
            return -1;
        }
    }

    char **binds_texts = malloc(sizeof(char *) * BINDS_NUM);
    if (!binds_texts)
    {
        printf("Could not allocate memory\n");
        for (int i = 0; i < text_num; i++)
        {
            free(best_scores_text[i]);
            best_scores_text[i] = NULL;
        }
        free(best_scores_text);
        best_scores_text = NULL;
        TTF_CloseFont(title_font);
        TTF_CloseFont(data_font);
        return -1;
    }
    for (int i = 0; i < BINDS_NUM; i++)
    {
        binds_texts[i] = malloc(WORD_SIZE * 2 + 2);
        if (!binds_texts[i])
        {
            printf("Could not allocate memory\n");
            for (int j = 0; j < text_num; j++)
            {
                free(best_scores_text[j]);
                best_scores_text[j] = NULL;
            }
            free(best_scores_text);
            best_scores_text = NULL;
            for (int j = 0; j < i; j++)
            {
                free(binds_texts[j]);
                binds_texts[j] = NULL;
            }
            free(binds_texts);
            binds_texts = NULL;
            TTF_CloseFont(title_font);
            TTF_CloseFont(data_font);
            return -1;
        }
    }

    // TMovement move;
    TMovement move = {
        .move_down.bind[0] = move_down,
        .move_down.sdl_name = move_down,
        .move_left.bind[0] = move_left,
        .move_left.sdl_name = move_left,
        .move_right.bind[0] = move_right,
        .move_right.sdl_name = move_right,
        .move_hold.bind[0] = move_hold,
        .move_hold.sdl_name = move_hold,
        .rotate_right.bind[0] = rotate_right,
        .rotate_right.sdl_name = rotate_right,
        .rotate_left.bind[0] = rotate_left,
        .rotate_left.sdl_name = rotate_left};
    if (!get_settings(&move))
    {
        fprintf(stderr, "Failed to open config file", SDL_GetError());
        for (int j = 0; j < text_num; j++)
        {
            free(best_scores_text[j]);
            best_scores_text[j] = NULL;
        }
        free(best_scores_text);
        best_scores_text = NULL;
        for (int j = 0; j < BINDS_NUM; j++)
        {
            free(binds_texts[j]);
            binds_texts[j] = NULL;
        }
        free(binds_texts);
        binds_texts = NULL;
        TTF_CloseFont(title_font);
        TTF_CloseFont(data_font);
        return -1;
    }

    sprintf(binds_texts[0], "%s: %s\0", move.move_down.name, move.move_down.bind);
    sprintf(binds_texts[1], "%s: %s\0", move.move_left.name, move.move_left.bind);
    sprintf(binds_texts[2], "%s: %s\0", move.move_right.name, move.move_right.bind);
    sprintf(binds_texts[3], "%s: %s\0", move.move_hold.name, move.move_hold.bind);
    sprintf(binds_texts[4], "%s: %s\0", move.rotate_right.name, move.rotate_right.bind);
    sprintf(binds_texts[5], "%s: %s\0", move.rotate_left.name, move.rotate_left.bind);

    FILE *f = fopen(SCORE_FILE, "r");
    if (!f)
    {
        printf("could not open file");
        strcpy(best_scores_text[0], NO_SCORE);
        text_num = 1;
    }
    else
    {
        while (fgets(row, sizeof(row), f))
        {
            row[strcspn(row, "\n")] = '\0';
            strcpy(best_scores_text[text_num], row);
            text_num++;
            if (text_num >= BEST_SCORE_NUM)
            {
                break;
            }
        }
        fclose(f);
    }

    TConfigButton config_buttons[BINDS_NUM];
    // Render Main menu
    draw_title_texts(renderer, best_scores, title_font, data_font, text_num, BEST_SCORES, best_scores_text);
    SDL_FRect start_button_pos = draw_button(renderer, start_button, green, white, title_font, START);
    if (start_button_pos.h == -1)
    {
        SDL_FRect retry = draw_button(renderer, start_button, blue, white, title_font, MAIN_MENU); // draw main menu button
        if (retry.h == -1)
        {
            printf("could not draw button\n");
            for (int j = 0; j < text_num; j++)
            {
                free(best_scores_text[j]);
                best_scores_text[j] = NULL;
            }
            free(best_scores_text);
            best_scores_text = NULL;
            for (int j = 0; j < BINDS_NUM; j++)
            {
                free(binds_texts[j]);
                binds_texts[j] = NULL;
            }
            free(binds_texts);
            binds_texts = NULL;
            TTF_CloseFont(title_font);
            TTF_CloseFont(data_font);
            return -1;
        }
        else
        {
            start_button_pos = retry;
        }
    }
    draw_title_config_box(renderer, config_box, title_font, data_font, CONFIG, binds_texts, config_buttons, BINDS_NUM);
    SDL_Delay(200);
    SDL_RenderPresent(renderer);

    SDL_Event e;
    bool running = true;
    while (running)
    {
        if (SDL_PollEvent(&e))
        {

            switch (e.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    // check if start had been pressed
                    if (e.button.x >= start_button_pos.x && e.button.x <= (start_button_pos.x + start_button_pos.w) && e.button.y >= start_button_pos.y && e.button.y <= (start_button_pos.y + start_button_pos.h))
                    {
                        printf("start button clicked\n");
                        for (int j = 0; j < text_num; j++)
                        {
                            free(best_scores_text[j]);
                            best_scores_text[j] = NULL;
                        }
                        free(best_scores_text);
                        best_scores_text = NULL;
                        for (int j = 0; j < BINDS_NUM; j++)
                        {
                            free(binds_texts[j]);
                            binds_texts[j] = NULL;
                        }
                        free(binds_texts);
                        binds_texts = NULL;
                        TTF_CloseFont(title_font);
                        TTF_CloseFont(data_font);
                        return GAME; // return to main menu
                    }
                    for (int i = 0; i < BINDS_NUM; i++)
                    {
                        if ((float)e.button.x >= config_buttons[i].button_pos.x && (float)e.button.x <= (config_buttons[i].button_pos.x + config_buttons[i].button_pos.w) && (float)e.button.y >= config_buttons[i].button_pos.y && (float)e.button.y <= (config_buttons[i].button_pos.y + config_buttons[i].button_pos.h))
                        {
                            // Open Bind window that returns new character in SDL format for config, or returns original character
                            printf("pressed edit: %s\n", config_buttons[i].button_name);
                        }
                    }
                }
                break;
            }
        }
    }
    for (int j = 0; j < text_num; j++)
    {
        free(best_scores_text[j]);
        best_scores_text[j] = NULL;
    }
    free(best_scores_text);
    best_scores_text = NULL;
    for (int j = 0; j < BINDS_NUM; j++)
    {
        free(binds_texts[j]);
        binds_texts[j] = NULL;
    }
    free(binds_texts);
    binds_texts = NULL;
    TTF_CloseFont(title_font);
    TTF_CloseFont(data_font);
    return -1;
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
    const SDL_FRect sound_img_box = {
        .h = ICON_SIZE,
        .w = ICON_SIZE,
        .x = cog_img_box.x - ICON_SIZE - SPACING_WIDTH,
        .y = cog_img_box.y,
    };
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

    bool running = true;
    bool game_over = false;
    SDL_Event e;
    int game_field[FIELD_HEIGHT][FIELD_WIDTH];

    // Matrice initialization
    matrice_init(game_field);

    // Font Definition
    TTF_Font *title_font = TTF_OpenFont(FONT, TITLE_SIZE);
    if (!title_font)
    {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        return -1;
    }
    TTF_Font *data_font = TTF_OpenFont(FONT, TEXT_SIZE);
    if (!data_font)
    {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        TTF_CloseFont(title_font);
        return -1;
    }

    // Score and lines text
    char level_str[WORD_SIZE];
    char score_str[WORD_SIZE];
    char lines_str[WORD_SIZE];
    TDataText data_texts[3];
    data_texts[0].title = LEVEL_TEXT;
    data_texts[0].data = "0";
    data_texts[1].title = SCORE_TEXT;
    data_texts[1].data = "0";
    data_texts[2].title = LINES_TEXT;
    data_texts[2].data = "0";

    // An array Cotaining all objects
    TObject object_array[7] = {
        object_I,
        object_J,
        object_L,
        object_O,
        object_S,
        object_T,
        object_Z,
    };

    // Object definitions
    TObject object;
    TObject hold;
    TObject next;

    // Generating random object index
    int random_object = SDL_rand(6);
    int random_next = SDL_rand(6);

    // Moving object starting position variables
    int rand_x_position = SDL_rand(6) + 1;

    // Movement variables

    // TMovement move;
    TMovement move = {
        .move_down.bind[0] = move_down,
        .move_down.sdl_name = move_down,
        .move_left.bind[0] = move_left,
        .move_left.sdl_name = move_left,
        .move_right.bind[0] = move_right,
        .move_right.sdl_name = move_right,
        .move_hold.bind[0] = move_hold,
        .move_hold.sdl_name = move_hold,
        .rotate_right.bind[0] = rotate_right,
        .rotate_right.sdl_name = rotate_right,
        .rotate_left.bind[0] = rotate_left,
        .rotate_left.sdl_name = rotate_left};

    if (!get_settings(&move))
    {
        fprintf(stderr, "Failed to open config file", SDL_GetError());
        TTF_CloseFont(title_font);
        TTF_CloseFont(data_font);
        return -1;
    }

    TIconText binds[BINDS_NUM];
    binds[0].text = move.move_down;
    binds[0].icon_path = ARROW_DOWN;
    binds[1].text = move.move_left;
    binds[1].icon_path = ARROW_LEFT;
    binds[2].text = move.move_right;
    binds[2].icon_path = ARROW_RIGHT;
    binds[3].text = move.move_hold;
    binds[3].icon_path = HOLD;
    binds[4].text = move.rotate_left;
    binds[4].icon_path = ROTATE_CC;
    binds[5].text = move.rotate_right;
    binds[5].icon_path = ROTATE_CW;

    for (int i = 0; i < BINDS_NUM; i++)
    {
        printf("%s, %s\n", binds[i].text.bind, binds[i].icon_path);
    }

    // Object init
    object = object_array[random_object];
    // next = object_I;
    next = object_array[random_next];
    hold = empty_object;
    // int held_matrice[OBJECT_MATRICE_SIZE][OBJECT_MATRICE_SIZE];
    // memcpy(held_matrice, object.matrice, sizeof(object.matrice));

    SDL_Point new_pos = {
        // Next position variable (used as temp position variable)
        .x = rand_x_position,
        .y = 1,
    };

    enum direction new_rotation = object.rotation; // Next rotation variable (used in rotation as temp rotation variable)

    object.pos = new_pos; // Default object position

    int gravity_delay_ms = 800;
    int delay = 48;
    Uint32 last_gravity_tick = SDL_GetTicks();
    Uint32 current_tick = SDL_GetTicks();

    int move_y_failed = 0;
    bool held_current_object = false;
    int lines = 0;
    int score = 0;
    int level = 0;
    int level_cleared_lines = 0;
    int cleared_lines = 0;

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                printf("%c\t%d\n", e.key.keysym.sym, e.key.keysym.sym);
                if (e.key.keysym.sym == move.move_down.sdl_name)
                {

                    printf("Down\n");
                    new_pos.y += 1;
                    if (!try_move_object(&object, game_field, new_pos))
                    {
                        if (move_y_failed >= 1)
                        {
                            // Solidify current object
                            solidify(object, game_field);
                            cleared_lines = remove_full_rows(game_field);
                            lines += cleared_lines;
                            level_cleared_lines += cleared_lines;
                            if (level_cleared_lines > 10)
                            {
                                if (level < 9)
                                {
                                    delay -= 5;
                                }
                                if (level > 18)
                                {
                                    delay = 1;
                                }
                                level++;
                                gravity_delay_ms = delay / 60 * 1000;
                                level_cleared_lines = 0;

                                // Update Level
                                sprintf(level_str, "%d", level);
                                data_texts[0].data = level_str;
                            }

                            // Original BPS scoring system -- edit - cannot be used (would have to add stages) - use Original Nintendo scoring system
                            switch (cleared_lines)
                            {
                            case 1:
                                score += 40 * (level + 1);
                                break;
                            case 2:
                                score += 100 * (level + 1);
                                break;
                            case 3:
                                score += 300 * (level + 1);
                                break;
                            case 4:
                                score += 1200 * (level + 1);
                                break;
                            }

                            // Update Score
                            sprintf(score_str, "%d", score);
                            data_texts[1].data = score_str;

                            // Update Lines
                            sprintf(lines_str, "%d", lines);
                            data_texts[2].data = lines_str;

                            // reset variables
                            held_current_object = false;
                            move_y_failed = 0;
                            rand_x_position = SDL_rand(6) + 1;
                            random_next = SDL_rand(6);

                            // Set next as current
                            object = next;

                            // Set object's rotation
                            object.rotation = UP;
                            new_rotation = object.rotation;

                            // Set current object's position
                            new_pos.x = rand_x_position;
                            new_pos.y = 1;
                            object.pos = new_pos;

                            // Set new next object
                            next = object_array[random_next];

                            if (object_collision_detection(object, game_field))
                            {
                                game_over = true;
                                running = 0;
                            }
                        }
                        else
                        {
                            move_y_failed++;
                            new_pos = object.pos;
                        }
                    }
                    else
                    {
                        object.pos = new_pos;
                    }
                    continue;
                }
                if (e.key.keysym.sym == move.move_left.sdl_name)
                {

                    printf("Left\n");
                    new_pos.x -= 1;
                    if (!try_move_object(&object, game_field, new_pos))
                    {
                        new_pos = object.pos;
                    }
                    else
                    {
                        object.pos = new_pos;
                    }
                    continue;
                }
                if (e.key.keysym.sym == move.move_right.sdl_name)
                {

                    printf("Right\n");
                    new_pos.x += 1;
                    if (!try_move_object(&object, game_field, new_pos))
                    {
                        new_pos = object.pos;
                    }
                    else
                    {
                        object.pos = new_pos;
                    }
                    continue;
                }
                if (e.key.keysym.sym == move.move_hold.sdl_name)
                {

                    if (!held_current_object)
                    {
                        if (hold.h == 0 && hold.w == 0)
                        {
                            hold = object;
                            object = next;
                            random_next = SDL_rand(6);
                        }
                        else
                        {
                            next = object;
                            object = hold;
                            hold = next;
                        }
                        hold.pos.x = 0;
                        hold.pos.y = 0;
                        rotate_UP(&hold);
                        hold.rotation = UP;
                        next = object_array[random_next];
                        new_pos.x = rand_x_position;
                        new_pos.y = 1;
                        object.pos = new_pos;
                        held_current_object = true;
                    }
                    continue;
                }
                if ((char)e.key.keysym.sym == move.rotate_left.sdl_name)
                {

                    printf("RotateL\n");
                    new_rotation = (object.rotation - 1) % 4;
                    if (!try_rotate_object(&object, game_field, -1))
                    {
                        new_rotation = object.rotation;
                    }
                    else
                    {
                        object.rotation = new_rotation;
                    }

                    continue;
                }
                if ((char)e.key.keysym.sym == move.rotate_right.sdl_name)
                {

                    printf("RotateR\n");

                    new_rotation = (object.rotation + 1) % 4;
                    if (!try_rotate_object(&object, game_field, 1))
                    {
                        new_rotation = object.rotation;
                    }
                    else
                    {
                        object.rotation = new_rotation;
                    }
                    continue;
                }
            }
        }

        current_tick = SDL_GetTicks();
        if (current_tick - last_gravity_tick >= gravity_delay_ms)
        {
            new_pos.y += 1;
            if (!try_move_object(&object, game_field, new_pos))
            {
                // Let it fail 1 time
                if (move_y_failed >= 1)
                {
                    // Solidify current object
                    solidify(object, game_field);
                    cleared_lines = remove_full_rows(game_field);
                    lines += cleared_lines;
                    level_cleared_lines += cleared_lines;
                    if (level_cleared_lines > 10)
                    {
                        if (level < 9)
                        {
                            delay -= 5;
                        }
                        if (level > 18)
                        {
                            delay = 1;
                        }
                        level++;
                        gravity_delay_ms = delay / 60 * 1000;
                        level_cleared_lines = 0;

                        // Update Level
                        sprintf(level_str, "%d", level);
                        data_texts[0].data = level_str;
                    }

                    // Original Nintendo scoring system
                    switch (cleared_lines)
                    {
                    case 1:
                        score += 40 * (level + 1);
                        break;
                    case 2:
                        score += 100 * (level + 1);
                        break;
                    case 3:
                        score += 300 * (level + 1);
                        break;
                    case 4:
                        score += 1200 * (level + 1);
                        break;
                    }

                    // Update Score
                    sprintf(score_str, "%d", score);
                    data_texts[1].data = score_str;

                    // Update Lines
                    sprintf(lines_str, "%d", lines);
                    data_texts[2].data = lines_str;

                    // reset variables
                    held_current_object = false;
                    move_y_failed = 0;
                    rand_x_position = SDL_rand(6) + 1;
                    random_next = SDL_rand(6);

                    // Set next as current
                    object = next;

                    // Set object's rotation
                    object.rotation = UP;
                    new_rotation = object.rotation;

                    // Set current object's position
                    new_pos.x = rand_x_position;
                    new_pos.y = 1;
                    object.pos = new_pos;

                    // Set new next object
                    next = object_array[random_next];

                    if (object_collision_detection(object, game_field))
                    {
                        game_over = true;
                        running = 0;
                    }
                }
                else
                {
                    new_pos = object.pos;
                    move_y_failed++;
                }
            }
            else
            {
                score += 1;
                object.pos = new_pos;
                move_y_failed = 0;
            }
            last_gravity_tick = current_tick;
        }

        draw_background(renderer, black);

        draw_object_box(renderer, light.secondary, hold_box, hold, HOLD_BOX_TEXT, title_font);     // Hold object box
        draw_text_box(renderer, title_font, data_font, data_texts, 3, score_box);                  // Score box
        draw_playing_field(renderer, game_field, game_field_pos);                                  // Game matrice
        draw_object_matrice(renderer, game_field_pos, object);                                     // Object
        draw_object_box(renderer, light.secondary, next_box, next, NEXT_BOX_TEXT, title_font);     // Next object box
        draw_icon_text_block(renderer, binds_box, binds, BINDS_NUM, title_font, data_font, white); // Binds box
        draw_icon(renderer, cog_img_box, COG);                                                     // Settings Icon
        draw_icon(renderer, sound_img_box, VOLUME_ON);                                             // Sound Icon
        if (game_over)
        {
            int clicked = 0;
            draw_text(renderer, game_over_box, GAME_OVER_TEXT, title_font, white, true);                             // Game Over text
            SDL_FRect main_button_pos = draw_button(renderer, main_menu_button, blue, white, title_font, MAIN_MENU); // draw main menu button
            if (main_button_pos.h == -1)
            {
                SDL_FRect retry = draw_button(renderer, main_menu_button, blue, white, title_font, MAIN_MENU); // draw main menu button
                if (retry.h == -1)
                {
                    printf("could not draw button\n");
                    TTF_CloseFont(title_font);
                    TTF_CloseFont(data_font);
                    return -1;
                }
                else
                {
                    main_button_pos = retry;
                }
            }
            SDL_FRect restart_button_pos = draw_button(renderer, restart_button, green, white, title_font, RESTART); // draw restart button
            if (main_button_pos.h == -1)
            {
                SDL_FRect retry = draw_button(renderer, restart_button, green, white, title_font, RESTART); // draw restart button
                if (retry.h == -1)
                {
                    printf("could not draw button\n");
                    TTF_CloseFont(title_font);
                    TTF_CloseFont(data_font);
                    return -1;
                }
                else
                {
                    restart_button_pos = retry;
                }
            }

            SDL_RenderPresent(renderer);
            while (1) // while the button was not clicked or while there are more events, continue the loop
            {
                if (SDL_PollEvent(&e) && e.button.button == SDL_BUTTON_LEFT && e.type == SDL_MOUSEBUTTONDOWN)
                {
                    // return to main menu
                    if (e.button.x >= main_button_pos.x && e.button.x <= (main_button_pos.x + main_button_pos.w) && e.button.y >= main_button_pos.y && e.button.y <= (main_button_pos.y + main_button_pos.h))
                    {
                        printf("main menu button clicked\n");
                        TTF_CloseFont(title_font);
                        TTF_CloseFont(data_font);
                        return MENU; // return to main menu
                    }
                    // restart
                    if (e.button.x >= restart_button_pos.x && e.button.x <= (restart_button_pos.x + restart_button_pos.w) && e.button.y >= restart_button_pos.y && e.button.y <= (restart_button_pos.y + restart_button_pos.h))
                    {
                        printf("restart button clicked\n");
                        TTF_CloseFont(title_font);
                        TTF_CloseFont(data_font);
                        return GAME; // return to main menu
                    }
                }
            }
        }
        else
        {
            SDL_RenderPresent(renderer);
        }
    }
    TTF_CloseFont(title_font);
    TTF_CloseFont(data_font);
    if (update_best_scores(score))
    {
        printf("score has been updated\n");
    }
    else
    {
        printf("could not update score\n");
    }
};

int update_best_scores(int score)
{
    char row[SETTINGS_ROW_SIZE];
    int scores[BEST_SCORE_NUM], i = 0;
    bool score_added = false;
    FILE *f = fopen(SCORE_FILE, "r");
    if (f)
    {
        while (fgets(row, sizeof(row), f))
        {
            scores[i] = atoi(strtok(row, "\n"));
            i++;
        }
        fclose(f);
    }
    else
    {
        printf("file does not exist\n");
    }
    f = fopen(SCORE_FILE, "w");
    if (!f)
    {
        printf("could not open file\n");
        return 0;
    }
    if (i > 0)
    {
        for (int ii = 0; ii < i && ii < BEST_SCORE_NUM; ii++)
        {
            if (scores[ii] > score)
            {
                fprintf(f, "%d\n", scores[ii]);
            }
            else
            {
                if (!score_added)
                {
                    fprintf(f, "%d\n", score);
                    score_added = true;
                    ii--; // so you could add the current score into the file too
                }
                else
                {
                    fprintf(f, "%d\n", scores[ii]);
                }
            }
        }
    }
    else
    {
        fprintf(f, "%d\n", score);
    }
    fclose(f);
    return 1;
}

void matrice_init(int matrice[FIELD_HEIGHT][FIELD_WIDTH])
{
    for (int i = 0; i < FIELD_HEIGHT; i++)
    {
        for (int j = 0; j < FIELD_WIDTH; j++)
        {
            if (i == 0 || i == FIELD_HEIGHT - 1 || j == 0 || j == FIELD_WIDTH - 1)
            {
                matrice[i][j] = 1;
            }
            else
            {
                matrice[i][j] = 0;
            }
        }
    }
}

int get_settings(TMovement *binds)
{

    FILE *f;
    char row[SETTINGS_ROW_SIZE];
    char *name, *bind, sdl_bind;
    char normalized_bind[WORD_SIZE];
    f = fopen("../data/cfg/user.cfg", "r");
    if (!f)
    {
        return 0;
    }
    while (fgets(row, SETTINGS_ROW_SIZE, f))
    {
        memset(normalized_bind, '\0', WORD_SIZE);
        name = strtok(row, ":");
        if (!name)
        {
            continue;
        }
        bind = strtok(NULL, ";");
        if (!bind)
        {
            continue;
        }
        strtok(NULL, "\n"); // Skip comments

        for (int i = 0, j = 0; i < (int)strlen(bind); i++, j++)
        {
            if (bind[i] == ' ')
            {
                j--;
                continue;
            }
            normalized_bind[j] = bind[i];
        }
        sdl_bind = get_sdl_name(normalized_bind);
        if (strcmp("move_left", name) == 0)
        {
            SDL_strlcpy(binds->move_left.name, name, sizeof(binds->move_left.name));
            SDL_strlcpy(binds->move_left.bind, normalized_bind, sizeof(normalized_bind));
            binds->move_left.sdl_name = sdl_bind;

            continue;
        }
        if (strcmp("move_right", name) == 0)
        {
            SDL_strlcpy(binds->move_right.name, name, sizeof(binds->move_left.name));
            SDL_strlcpy(binds->move_right.bind, normalized_bind, sizeof(normalized_bind));
            binds->move_right.sdl_name = sdl_bind;
            continue;
        }
        if (strcmp("move_down", name) == 0)
        {
            SDL_strlcpy(binds->move_down.name, name, sizeof(binds->move_left.name));
            SDL_strlcpy(binds->move_down.bind, normalized_bind, sizeof(normalized_bind));
            binds->move_down.sdl_name = sdl_bind;
            continue;
        }
        if (strcmp("move_hold", name) == 0)
        {
            SDL_strlcpy(binds->move_hold.name, name, sizeof(binds->move_left.name));
            SDL_strlcpy(binds->move_hold.bind, normalized_bind, sizeof(normalized_bind));
            binds->move_hold.sdl_name = sdl_bind;
            continue;
        }
        if (strcmp("rotate_right", name) == 0)
        {
            SDL_strlcpy(binds->rotate_right.name, name, sizeof(binds->move_left.name));
            SDL_strlcpy(binds->rotate_right.bind, normalized_bind, sizeof(normalized_bind));
            binds->rotate_right.sdl_name = sdl_bind;
            continue;
        }
        if (strcmp("rotate_left", name) == 0)
        {
            SDL_strlcpy(binds->rotate_left.name, name, sizeof(binds->move_left.name));
            SDL_strlcpy(binds->rotate_left.bind, normalized_bind, sizeof(normalized_bind));
            binds->rotate_left.sdl_name = sdl_bind;
            continue;
        }
    }

    fclose(f);
    return 1;
}

char get_sdl_name(char *name)
{
    for (int i = 0; i < (int)strlen(name); i++)
    {
        tolower(name[i]);
    }

    if (strcmp(name, "arrow_left") == 0)
    {
        return 'P';
    }
    if (strcmp(name, "arrow_right") == 0)
    {
        return 'O';
    }
    if (strcmp(name, "arrow_up") == 0)
    {
        return 'R';
    }
    if (strcmp(name, "arrow_down") == 0)
    {
        return 'Q';
    }
    if (strcmp(name, "space") == 0)
    {
        return ' ';
    }
    return name[0];
}