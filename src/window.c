#include "window.h"

int game_window()
{
    int window_width, window_height;

    // SDL Initialization
    if (SDL_Init(SDL_INIT_VIDEO))
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

    // Create SDL Window
    SDL_Window *window = SDL_CreateWindow(
        "ZAM0074 - Tetris", // Window title
        100,                // Y coords
        100,                // X coords
        1920,               // Default window width
        1080,               // Default window height
        // next_box.x + next_box.w + SPACING_WIDTH, // Window width - based on size and position of next object box
        // matrice_box.h + matrice_box.y * 2,       // Window height - based on spacing around matrice and matrice height
        SDL_WINDOW_FULLSCREEN_DESKTOP // Show window right after creation
    );

    // If Creation Failed, return 1
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GetWindowSize(window, &window_width, &window_height); // Get window width and height

    // Create SDL Renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED //| SDL_RENDERER_PRESENTVSYNC // Hardware acceleration,  VSYNC enabled
    );

    // If Creation Failed, return 1
    if (!renderer)
    {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    game_infinite_loop(renderer, window_width, window_height);

    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

int SDL_rand(int max)
{
    return rand() % (max + 1);
}

void game_infinite_loop(SDL_Renderer *renderer, int window_width, int window_height)
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

    bool running = true;
    SDL_Event e;
    int game_field[FIELD_HEIGHT][FIELD_WIDTH];
    SDL_FPoint game_field_pos = {
        .x = matrice_box.x,
        .y = matrice_box.y,
    };

    // Font Definition
    TTF_Font *title_font = TTF_OpenFont(FONT, TITLE_SIZE);
    TTF_Font *data_font = TTF_OpenFont(FONT, TEXT_SIZE);

    if (!title_font)
    {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        return;
    }
    if (!data_font)
    {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        return;
    }

    // Score and lines text
    char data_str[WORD_SIZE];
    TDataText data_texts[2];
    data_texts[0].title = "SCORE";
    data_texts[0].data = "0";
    data_texts[1].title = "LINES";
    data_texts[1].data = "0";

    // An array Cntaining all objects
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
    enum
    {
        move_left = 'a',
        move_right = 'd',
        move_down = 's',
        move_hold = 'c',
        rotate_right = 'O',
        rotate_left = 'P'
    };

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

    // Matrice initialization
    matrice_init(game_field);

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

    int gravity_delay_ms = 1000;
    Uint32 last_gravity_tick = SDL_GetTicks();
    Uint32 current_tick = SDL_GetTicks();

    int move_y_failed = 0;
    bool held_current_object = false;
    int cleared_rows = 0;

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
                            cleared_rows += remove_full_rows(game_field);

                            // Set new value to score and cleared lines
                            printf("cleared rows: %d\n", cleared_rows);
                            sprintf(data_str, "%d", cleared_rows);
                            data_texts[0].data = data_str;
                            sprintf(data_str, "%d", cleared_rows);
                            data_texts[1].data = data_str;
                            printf("%s\n", data_texts[0].data);

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
                    cleared_rows += remove_full_rows(game_field);

                    // Set new value to score and cleared lines
                    printf("cleared rows: %d\n", cleared_rows);
                    sprintf(data_str, "%d", cleared_rows);
                    data_texts[0].data = data_str;
                    sprintf(data_str, "%d", cleared_rows);
                    data_texts[1].data = data_str;
                    printf("%s\n", data_texts[0].data);

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
                }
                else
                {
                    new_pos = object.pos;
                    move_y_failed++;
                }
            }
            else
            {
                object.pos = new_pos;
                move_y_failed = 0;
            }
            last_gravity_tick = current_tick;
        }

        draw_background(renderer, black);

        draw_object_box(renderer, light.secondary, hold_box, hold, "hold", title_font); // Hold object box
        draw_text_box(renderer, title_font, data_font, data_texts, 2, score_box);
        draw_playing_field(renderer, game_field, game_field_pos);
        draw_object_matrice(renderer, game_field_pos, object);
        draw_object_box(renderer, light.secondary, next_box, next, "next", title_font); // Next object box
        draw_icon_text_block(renderer, binds_box, binds, BINDS_NUM, title_font, data_font, white);
        draw_icon(renderer, cog_img_box, COG);
        draw_icon(renderer, sound_img_box, VOLUME_ON);
        SDL_RenderPresent(renderer);
    }
    TTF_CloseFont(title_font);
    TTF_CloseFont(data_font);
};

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
            printf("true");
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