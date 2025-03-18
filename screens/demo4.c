//object includes
#include "../objects/game.h"
#include "../objects/texture.h"
#include "../objects/button.h"
#include "../objects/textbox.h"
#include "../objects/colors.h"
#include "../objects/question.h"
#include "../objects/timer.h"
#include "../objects/balloon.h"

//screen include
#include "screens.h"

struct media_t {
    Texture *tex_balloon;
    Texture *tex_anim;
    Texture *tex_bg;
    Texture *tex_name;
    Texture *tex_points;
    Texture *tex_text1[5][7];
    Texture *tex_text2[3][5];
    Texture *tex_text3[2][8];
    
    SDL_Rect clip_bg;

    TTF_Font *font;

    Mix_Music *music;
    Mix_Chunk *sfx_click;
    Mix_Chunk *sfx_right;
};

struct objects_t {
    Balloon *balloon;
};

struct variables_t {
    bool next;
    Screen ret;
    SDL_Event e;
    int transition;
    int text_number;
    int text_offset;
    int old_points;
    char str_points[7];
    bool play_sound;
};

bool screens_demo4_loadmedia(Game *game, struct media_t *media, struct variables_t *var) {
    media->tex_balloon = texture_create();
    if (media->tex_balloon == NULL) return false;
    media->tex_anim = texture_create();
    if (media->tex_anim == NULL) return false;
    media->tex_bg = texture_create();
    if (media->tex_bg == NULL) return false;
    media->tex_name = texture_create();
    if (media->tex_name == NULL) return false;
    media->tex_points = texture_create();
    if (media->tex_points == NULL) return false;

    if(!texture_load_from_file(media->tex_balloon, game_get_renderer(game), "img/BalloonTexture.png")) return false;
    if(!texture_load_from_file(media->tex_anim, game_get_renderer(game), "img/BalloonAnimTexture.png")) return false;
    
    media->font = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 8);
    if (media->font == NULL) return false;

    if(!texture_load_from_file(media->tex_bg, game_get_renderer(game), "img/BgTexture.png")) return false;

    media->clip_bg.x = 0;
    media->clip_bg.y = 0;
    media->clip_bg.w = SCREEN_W;
    media->clip_bg.h = SCREEN_H;

    texture_set_color_mod(media->tex_bg, COLOR_LRED, SDL_BLENDMODE_BLEND);

    media->music = Mix_LoadMUS("snd/demoproducts.wav");
    if (media->music == NULL) return false;
    media->sfx_click = Mix_LoadWAV("snd/sfx_click.wav");
    if (media->sfx_click == NULL) return false;
    media->sfx_right = Mix_LoadWAV("snd/sfx_right.wav");
    if (media->sfx_right == NULL) return false;
    
    //NAME AND POINTS
    if(!texture_load_from_text(media->tex_name, game_get_renderer(game), media->font, game_get_name(game), COLOR_TEXT_DEFAULT_LIGHT)) return false;

    sprintf(var->str_points, "%06d", var->old_points);
    if(!texture_load_from_text(media->tex_points, game_get_renderer(game), media->font, var->str_points, COLOR_TEXT_DEFAULT_LIGHT)) return false;

    //TEXTS 1
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 7; j++) {
            media->tex_text1[i][j] = texture_create();
            if (media->tex_text1[i][j] == NULL) return false;
        }
    }

    if(!texture_load_from_text(media->tex_text1[0][0], game_get_renderer(game), media->font, "a(", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[0][1], game_get_renderer(game), media->font, "x", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text1[0][2], game_get_renderer(game), media->font, "-x1)(", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[0][3], game_get_renderer(game), media->font, "x", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text1[0][4], game_get_renderer(game), media->font, "-x2) = a[", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[0][5], game_get_renderer(game), media->font, "x²", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text1[0][6], game_get_renderer(game), media->font, "]", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    
    if(!texture_load_from_text(media->tex_text1[1][0], game_get_renderer(game), media->font, "a(", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[1][1], game_get_renderer(game), media->font, "x", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text1[1][2], game_get_renderer(game), media->font, "-x1)(x", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[1][3], game_get_renderer(game), media->font, "-x2", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text1[1][4], game_get_renderer(game), media->font, ") = a[x²", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[1][5], game_get_renderer(game), media->font, "-x2·x", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text1[1][6], game_get_renderer(game), media->font, "]", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    
    if(!texture_load_from_text(media->tex_text1[2][0], game_get_renderer(game), media->font, "a(x", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[2][1], game_get_renderer(game), media->font, "-x1", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text1[2][2], game_get_renderer(game), media->font, ")(", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[2][3], game_get_renderer(game), media->font, "x", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text1[2][4], game_get_renderer(game), media->font, "-x2) = a[x²-x2·x", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[2][5], game_get_renderer(game), media->font, "-x1·x", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text1[2][6], game_get_renderer(game), media->font, "]", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    
    if(!texture_load_from_text(media->tex_text1[3][0], game_get_renderer(game), media->font, "a(x", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[3][1], game_get_renderer(game), media->font, "-x1", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text1[3][2], game_get_renderer(game), media->font, ")(x", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[3][3], game_get_renderer(game), media->font, "-x2", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text1[3][4], game_get_renderer(game), media->font, ") = a[x²-x2·x-x1·x", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[3][5], game_get_renderer(game), media->font, "+x1·x2", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text1[3][6], game_get_renderer(game), media->font, "]", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    
    if(!texture_load_from_text(media->tex_text1[4][0], game_get_renderer(game), media->font, "a(x", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[4][1], game_get_renderer(game), media->font, "-x1", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[4][2], game_get_renderer(game), media->font, ")(x", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[4][3], game_get_renderer(game), media->font, "-x2", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[4][4], game_get_renderer(game), media->font, ") = a[x²-", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1[4][5], game_get_renderer(game), media->font, "(x1+x2)", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text1[4][6], game_get_renderer(game), media->font, "x+x1·x2]", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    
    //TEXTS 2
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 7; j++) {
            media->tex_text2[i][j] = texture_create();
            if (media->tex_text2[i][j] == NULL) return false;
        }
    }

    if(!texture_load_from_text(media->tex_text2[0][0], game_get_renderer(game), media->font, "a", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text2[0][1], game_get_renderer(game), media->font, "[", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text2[0][2], game_get_renderer(game), media->font, "x²", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text2[0][3], game_get_renderer(game), media->font, "-(x1+x2)x+x1·x2] = ", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text2[0][4], game_get_renderer(game), media->font, "ax²", COLOR_LGREEN)) return false;
    
    if(!texture_load_from_text(media->tex_text2[1][0], game_get_renderer(game), media->font, "a", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text2[1][1], game_get_renderer(game), media->font, "[x²", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text2[1][2], game_get_renderer(game), media->font, "-(x1+x2)x", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text2[1][3], game_get_renderer(game), media->font, "+x1·x2] = ax²", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text2[1][4], game_get_renderer(game), media->font, "-a(x1+x2)x", COLOR_LGREEN)) return false;
    
    if(!texture_load_from_text(media->tex_text2[2][0], game_get_renderer(game), media->font, "a", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text2[2][1], game_get_renderer(game), media->font, "[x²-(x1+x2)x+", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text2[2][2], game_get_renderer(game), media->font, "x1·x2", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text2[2][3], game_get_renderer(game), media->font, "] = ax²-a(x1+x2)x", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text2[2][4], game_get_renderer(game), media->font, "+a·x1·x2", COLOR_LGREEN)) return false;
    
    //TEXTS 3
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 8; j++) {
            media->tex_text3[i][j] = texture_create();
            if (media->tex_text3[i][j] == NULL) return false;
        }
    }

    if(!texture_load_from_text(media->tex_text3[0][0], game_get_renderer(game), media->font, "ax²-a(", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text3[0][1], game_get_renderer(game), media->font, "x1+x2", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text3[0][2], game_get_renderer(game), media->font, ")x+a·", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text3[0][3], game_get_renderer(game), media->font, "x1·x2", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text3[0][4], game_get_renderer(game), media->font, " = ax²-a(", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text3[0][5], game_get_renderer(game), media->font, "-b/a", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text3[0][6], game_get_renderer(game), media->font, ")x+a·", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text3[0][7], game_get_renderer(game), media->font, "c/a", COLOR_LGREEN)) return false;
    
    if(!texture_load_from_text(media->tex_text3[1][0], game_get_renderer(game), media->font, "ax²-a(", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text3[1][1], game_get_renderer(game), media->font, "x1+x2", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text3[1][2], game_get_renderer(game), media->font, ")x+a·", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text3[1][3], game_get_renderer(game), media->font, "x1·x2", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text3[1][4], game_get_renderer(game), media->font, " = ax²", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text3[1][5], game_get_renderer(game), media->font, "+b", COLOR_LGREEN)) return false;
    if(!texture_load_from_text(media->tex_text3[1][6], game_get_renderer(game), media->font, "x+", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text3[1][7], game_get_renderer(game), media->font, "c", COLOR_LGREEN)) return false;
    
    return true;
}

bool screens_demo4_loadobjects(Game *game, struct objects_t *objects, struct media_t *media, struct variables_t *var) {
    objects->balloon = balloon_create(media->tex_balloon, media->tex_anim, "txt/demo4_1.txt", true);
    if (objects->balloon == NULL) return false;
    return true;
}

void screens_demo4_loadvariables(Game *game, struct variables_t *var) {
    var->next = false;
    var->ret = SCREEN_NEXT;
    var->old_points = game_get_points(game, POINTS_ALL & ~POINTS_4);
    var->transition = 15;
    var->text_number = 0;
    var->text_offset = 8;
    var->play_sound = false;
}

Screen screens_demo4_close(struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    balloon_free(objects->balloon);
    texture_free(media->tex_bg);
    texture_free(media->tex_balloon);
    texture_free(media->tex_anim);
    texture_free(media->tex_name);
    texture_free(media->tex_points);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 7; j++) {
            texture_free(media->tex_text1[i][j]);
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            texture_free(media->tex_text2[i][j]);
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 8; j++) {
            texture_free(media->tex_text3[i][j]);
        }
    }
    TTF_CloseFont(media->font);
    Mix_FreeMusic(media->music);
    Mix_FreeChunk(media->sfx_click);
    Mix_FreeChunk(media->sfx_right);
    free(media);
    free(objects);
    Screen ret = var->ret;
    free(var);
    return ret;
}

void screens_demo4_intro(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    while (!var->next) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                game_save(game, false);
                var->next = true;
                var->ret = SCREEN_QUIT;
                var->transition = 0;
            }
            else if (var->e.type == SDL_MOUSEBUTTONDOWN) {
                var->play_sound = true;
            }
            balloon_handle_event(objects->balloon, var->e);
        }
        if (balloon_read_more(objects->balloon)) {
            balloon_read_text(objects->balloon, game_get_renderer(game), media->font);
            if (var->play_sound) {
                Mix_PlayChannel(-1, media->sfx_click, 0);
                var->play_sound = false;
            }
        }

        balloon_calc_position(objects->balloon);
        if (balloon_ishidden(objects->balloon)) {
            var->next = true;
        }
    
        game_clear_screen(game);
        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        
        balloon_render(objects->balloon, game_get_renderer(game));

        game_render(game, var->transition);

        var->transition += 16 * (var->transition < 255);
    }
}

void screens_demo4_demo1(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    while (!var->next) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                game_save(game, false);
                var->next = true;
                var->ret = SCREEN_QUIT;
                var->transition = 0;
            }
            else if (var->e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, media->sfx_click, 0);
                var->text_number += 1;
                var->text_offset = 8;
                if (var->text_number > 4) {
                    var->next = true;
                    var->text_number = 4;
                    var->text_offset = 0;
                }
                SDL_Color alpha_mod = {255, 255, 255, 255};
                for (int i = var->text_number; i >= 0; i--) {
                    for (int j = 0; j < 7; j++) {
                        texture_set_color_mod(media->tex_text1[i][j], alpha_mod, SDL_BLENDMODE_BLEND);
                    }
                    alpha_mod.a /= 2;
                }
            }
        }

        game_clear_screen(game);

        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        
        for (int k = 0; k <= var->text_number; k++) {
            for (int i = 0; i < 7; i++) {
                int x = 192;
                for (int j = 0; j < i; j++) {
                    x += texture_getw(media->tex_text1[k][j]);
                }
                if (k < var->text_number) texture_render(media->tex_text1[k][i], game_get_renderer(game), x, 80 - 8 * (var->text_number - k) + var->text_offset, NULL);
                else texture_render(media->tex_text1[k][i], game_get_renderer(game), x, 80 - 8 * (var->text_number - k), NULL);
            }
        }

        game_render(game, var->transition);

        var->text_offset >>= 1;
    }
}

void screens_demo4_middle1(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    while (!var->next) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                game_save(game, false);
                var->next = true;
                var->ret = SCREEN_QUIT;
                var->transition = 0;
            }
            else if (var->e.type == SDL_MOUSEBUTTONDOWN) {
                var->play_sound = true;
            }
            balloon_handle_event(objects->balloon, var->e);
        }

        if (balloon_read_more(objects->balloon)) {
            balloon_read_text(objects->balloon, game_get_renderer(game), media->font);
            if (var->play_sound) {
                Mix_PlayChannel(-1, media->sfx_click, 0);
                var->play_sound = false;
            }
        }

        balloon_calc_position(objects->balloon);
        if (balloon_ishidden(objects->balloon)) {
            var->next = true;
        }
    
        game_clear_screen(game);
        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        
        for (int k = 0; k <= 4; k++) {
            for (int i = 0; i < 7; i++) {
                int x = 192;
                for (int j = 0; j < i; j++) {
                    x += texture_getw(media->tex_text1[k][j]);
                }
                texture_render(media->tex_text1[k][i], game_get_renderer(game), x, 80 - 8 * (4 - k), NULL);
            }
        }

        balloon_render(objects->balloon, game_get_renderer(game));

        game_render(game, var->transition);
    }
}

void screens_demo4_demo2(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    while (!var->next) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                game_save(game, false);
                var->next = true;
                var->ret = SCREEN_QUIT;
                var->transition = 0;
            }
            else if (var->e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, media->sfx_click, 0);
                var->text_number += 1;
                var->text_offset = 8;
                if (var->text_number > 2) {
                    var->next = true;
                    var->text_number = 2;
                    var->text_offset = 0;
                }
                SDL_Color alpha_mod = {255, 255, 255, 255};
                for (int i = var->text_number; i >= 0; i--) {
                    for (int j = 0; j < 5; j++) {
                        texture_set_color_mod(media->tex_text2[i][j], alpha_mod, SDL_BLENDMODE_BLEND);
                    }
                    alpha_mod.a /= 2;
                }
            }
        }

        game_clear_screen(game);

        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        
        for (int k = 0; k <= 4; k++) {
            for (int i = 0; i < 7; i++) {
                int x = 192;
                for (int j = 0; j < i; j++) {
                    x += texture_getw(media->tex_text1[k][j]);
                }
                texture_render(media->tex_text1[k][i], game_get_renderer(game), x, 80 - 8 * (4 - k), NULL);
            }
        }

        for (int k = 0; k <= var->text_number; k++) {
            for (int i = 0; i < 5; i++) {
                int x = 192;
                for (int j = 0; j < i; j++) {
                    x += texture_getw(media->tex_text2[k][j]);
                }
                if (k < var->text_number) texture_render(media->tex_text2[k][i], game_get_renderer(game), x, 112 - 8 * (var->text_number - k) + var->text_offset, NULL);
                else texture_render(media->tex_text2[k][i], game_get_renderer(game), x, 112 - 8 * (var->text_number - k), NULL);
            }
        }

        game_render(game, var->transition);

        var->text_offset >>= 1;
    }
}

void screens_demo4_middle2(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    while (!var->next) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                game_save(game, false);
                var->next = true;
                var->ret = SCREEN_QUIT;
                var->transition = 0;
            }
            else if (var->e.type == SDL_MOUSEBUTTONDOWN) {
                var->play_sound = true;
            }
            balloon_handle_event(objects->balloon, var->e);
        }

        if (balloon_read_more(objects->balloon)) {
            balloon_read_text(objects->balloon, game_get_renderer(game), media->font);
            if (var->play_sound) {
                Mix_PlayChannel(-1, media->sfx_click, 0);
                var->play_sound = false;
            }
        }

        balloon_calc_position(objects->balloon);
        if (balloon_ishidden(objects->balloon)) {
            var->next = true;
        }
    
        game_clear_screen(game);
        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        
        for (int k = 0; k <= 4; k++) {
            for (int i = 0; i < 7; i++) {
                int x = 192;
                for (int j = 0; j < i; j++) {
                    x += texture_getw(media->tex_text1[k][j]);
                }
                texture_render(media->tex_text1[k][i], game_get_renderer(game), x, 80 - 8 * (4 - k), NULL);
            }
        }

        for (int k = 0; k <= 2; k++) {
            for (int i = 0; i < 5; i++) {
                int x = 192;
                for (int j = 0; j < i; j++) {
                    x += texture_getw(media->tex_text2[k][j]);
                }
                texture_render(media->tex_text2[k][i], game_get_renderer(game), x, 112 - 8 * (2 - k), NULL);
            }
        }

        balloon_render(objects->balloon, game_get_renderer(game));

        game_render(game, var->transition);
    }
}

void screens_demo4_demo3(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    while (!var->next) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                game_save(game, false);
                var->next = true;
                var->ret = SCREEN_QUIT;
                var->transition = 0;
            }
            else if (var->e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, media->sfx_click, 0);
                var->text_number += 1;
                var->text_offset = 8;
                if (var->text_number > 1) {
                    var->next = true;
                    var->text_number = 1;
                    var->text_offset = 0;
                }
                SDL_Color alpha_mod = {255, 255, 255, 255};
                for (int i = var->text_number; i >= 0; i--) {
                    for (int j = 0; j < 8; j++) {
                        texture_set_color_mod(media->tex_text3[i][j], alpha_mod, SDL_BLENDMODE_BLEND);
                    }
                    alpha_mod.a /= 2;
                }
            }
        }

        game_clear_screen(game);

        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        
        for (int k = 0; k <= 4; k++) {
            for (int i = 0; i < 7; i++) {
                int x = 192;
                for (int j = 0; j < i; j++) {
                    x += texture_getw(media->tex_text1[k][j]);
                }
                texture_render(media->tex_text1[k][i], game_get_renderer(game), x, 80 - 8 * (4 - k), NULL);
            }
        }

        for (int k = 0; k <= 2; k++) {
            for (int i = 0; i < 5; i++) {
                int x = 192;
                for (int j = 0; j < i; j++) {
                    x += texture_getw(media->tex_text2[k][j]);
                }
                texture_render(media->tex_text2[k][i], game_get_renderer(game), x, 112 - 8 * (2 - k), NULL);
            }
        }

        for (int k = 0; k <= var->text_number; k++) {
            for (int i = 0; i < 8; i++) {
                int x = 192;
                for (int j = 0; j < i; j++) {
                    x += texture_getw(media->tex_text3[k][j]);
                }
                if (k < var->text_number) texture_render(media->tex_text3[k][i], game_get_renderer(game), x, 136 - 8 * (var->text_number - k) + var->text_offset, NULL);
                else texture_render(media->tex_text3[k][i], game_get_renderer(game), x, 136 - 8 * (var->text_number - k), NULL);
            }
        }

        game_render(game, var->transition);

        var->text_offset >>= 1;
    }
}

void screens_demo4_end(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    while (!var->next || var->transition > 0) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                game_save(game, false);
                var->next = true;
                var->ret = SCREEN_QUIT;
                var->transition = 0;
            }
            else if (var->e.type == SDL_MOUSEBUTTONDOWN) {
                var->play_sound = true;
            }
            balloon_handle_event(objects->balloon, var->e);
        }

        if (balloon_read_more(objects->balloon)) {
            if (!balloon_read_text(objects->balloon, game_get_renderer(game), media->font)) {
                if (var->play_sound) {
                    Mix_PlayChannel(-1, media->sfx_right, 0);
                    var->play_sound = false;
                }
                var->next = true;
            }
            else {
                if (var->play_sound) {
                    Mix_PlayChannel(-1, media->sfx_click, 0);
                    var->play_sound = false;
                }
            }
        }

        balloon_calc_position(objects->balloon);
    
        game_clear_screen(game);
        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        
        for (int k = 0; k <= 4; k++) {
            for (int i = 0; i < 7; i++) {
                int x = 192;
                for (int j = 0; j < i; j++) {
                    x += texture_getw(media->tex_text1[k][j]);
                }
                texture_render(media->tex_text1[k][i], game_get_renderer(game), x, 80 - 8 * (4 - k), NULL);
            }
        }

        for (int k = 0; k <= 2; k++) {
            for (int i = 0; i < 5; i++) {
                int x = 192;
                for (int j = 0; j < i; j++) {
                    x += texture_getw(media->tex_text2[k][j]);
                }
                texture_render(media->tex_text2[k][i], game_get_renderer(game), x, 112 - 8 * (2 - k), NULL);
            }
        }

        for (int k = 0; k <= 1; k++) {
            for (int i = 0; i < 8; i++) {
                int x = 192;
                for (int j = 0; j < i; j++) {
                    x += texture_getw(media->tex_text3[k][j]);
                }
                texture_render(media->tex_text3[k][i], game_get_renderer(game), x, 136 - 8 * (1 - k), NULL);
            }
        }

        balloon_render(objects->balloon, game_get_renderer(game));

        game_render(game, var->transition);
        
        if (var->next) {
            var->transition -= 16;
            var->transition *= (var->transition > 0);
        }
    }
}

Screen screens_demo4(Game *game) {
    struct media_t *media = (struct media_t*)malloc(sizeof(struct media_t));
    struct objects_t *objects = (struct objects_t*)malloc(sizeof(struct objects_t));
    struct variables_t *var = (struct variables_t*)malloc(sizeof(struct variables_t));
    if (media == NULL || objects == NULL || var == NULL) {
        printf("Couldnt open structs\n");
        return SCREEN_ERROR;
    }
    screens_demo4_loadvariables(game, var);
    if (!screens_demo4_loadmedia(game, media, var)) {
        printf("Couldnt open textures\n");
        return SCREEN_ERROR;
    }
    if (!screens_demo4_loadobjects(game, objects, media, var)) {
        printf("Couldnt open objects\n");
        return SCREEN_ERROR;
    }
    
    Mix_PlayMusic(media->music, -1);

    screens_demo4_intro(game, media, objects, var);

    var->next = false;
    if (var->ret == SCREEN_NEXT) {
        screens_demo4_demo1(game, media, objects, var);
    }

    var->next = false;
    if (var->ret == SCREEN_NEXT) {
        if (!balloon_load_new_file(objects->balloon, "txt/demo4_2.txt", true)) {
            printf("Couldnt load new file\n");
            return SCREEN_ERROR;
        }
        screens_demo4_middle1(game, media, objects, var);
    }
    
    var->text_number = 0;
    var->next = false;
    if (var->ret == SCREEN_NEXT) {
        screens_demo4_demo2(game, media, objects, var);
    }

    var->next = false;
    if (var->ret == SCREEN_NEXT) {
        if (!balloon_load_new_file(objects->balloon, "txt/demo4_3.txt", true)) {
            printf("Couldnt load new file\n");
            return SCREEN_ERROR;
        }
        screens_demo4_middle2(game, media, objects, var);
    }
    
    var->text_number = 0;
    var->next = false;
    if (var->ret == SCREEN_NEXT) {
        screens_demo4_demo3(game, media, objects, var);
    }

    var->next = false;
    if (var->ret == SCREEN_NEXT) {
        if (!balloon_load_new_file(objects->balloon, "txt/demo4_4.txt", true)) {
            printf("Couldnt load new file\n");
            return SCREEN_ERROR;
        }
        screens_demo4_end(game, media, objects, var);
    }
    
    var->next = false;
    if (var->ret == SCREEN_NEXT) {
        var->ret = SCREEN_QUIZ4;
    }

    Mix_HaltMusic();

    return screens_demo4_close(media, objects, var);
}

