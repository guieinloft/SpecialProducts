//libc includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

//sdl includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

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
    Texture *tex_text[5][6];
    Texture *tex_letters[2];
    Texture *tex_canvas;
    
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

bool screens_demo2_loadmedia(Game *game, struct media_t *media, struct variables_t *var) {
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
    media->tex_canvas = texture_create();
    if (media->tex_canvas == NULL) return false;

    if(!texture_load_from_file(media->tex_balloon, game_get_renderer(game), "img/BalloonTexture.png")) return false;
    if(!texture_load_from_file(media->tex_anim, game_get_renderer(game), "img/BalloonAnimTexture.png")) return false;
    
    media->font = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 8);
    if (media->font == NULL) return false;

    if(!texture_create_blank(media->tex_canvas, game_get_renderer(game), 256, 256, SDL_TEXTUREACCESS_TARGET)) return false;
    
    if(!texture_load_from_file(media->tex_bg, game_get_renderer(game), "img/BgTexture.png")) return false;

    media->clip_bg.x = 0;
    media->clip_bg.y = 0;
    media->clip_bg.w = SCREEN_W;
    media->clip_bg.h = SCREEN_H;

    texture_set_color_mod(media->tex_bg, COLOR_LGREEN, SDL_BLENDMODE_BLEND);

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

    //TEXTS
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            media->tex_text[i][j] = texture_create();
            if (media->tex_text[i][j] == NULL) return false;
        }
    }

    if(!texture_load_from_text(media->tex_text[0][0], game_get_renderer(game), media->font, "(", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[0][1], game_get_renderer(game), media->font, "a", COLOR_DRED)) return false;
    if(!texture_load_from_text(media->tex_text[0][2], game_get_renderer(game), media->font, "-b)(", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[0][3], game_get_renderer(game), media->font, "a", COLOR_DRED)) return false;
    if(!texture_load_from_text(media->tex_text[0][4], game_get_renderer(game), media->font, "-b) = ", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[0][5], game_get_renderer(game), media->font, "a²", COLOR_DRED)) return false;

    if(!texture_load_from_text(media->tex_text[1][0], game_get_renderer(game), media->font, "(", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[1][1], game_get_renderer(game), media->font, "a", COLOR_DRED)) return false;
    if(!texture_load_from_text(media->tex_text[1][2], game_get_renderer(game), media->font, "-b)(a", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[1][3], game_get_renderer(game), media->font, "-b", COLOR_DRED)) return false;
    if(!texture_load_from_text(media->tex_text[1][4], game_get_renderer(game), media->font, ") = a²", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[1][5], game_get_renderer(game), media->font, "-ab", COLOR_DRED)) return false;

    if(!texture_load_from_text(media->tex_text[2][0], game_get_renderer(game), media->font, "(a", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[2][1], game_get_renderer(game), media->font, "-b", COLOR_DRED)) return false;
    if(!texture_load_from_text(media->tex_text[2][2], game_get_renderer(game), media->font, ")(", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[2][3], game_get_renderer(game), media->font, "a", COLOR_DRED)) return false;
    if(!texture_load_from_text(media->tex_text[2][4], game_get_renderer(game), media->font, "-b) = a²-ab", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[2][5], game_get_renderer(game), media->font, "-ba", COLOR_DRED)) return false;

    if(!texture_load_from_text(media->tex_text[3][0], game_get_renderer(game), media->font, "(a", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[3][1], game_get_renderer(game), media->font, "-b", COLOR_DRED)) return false;
    if(!texture_load_from_text(media->tex_text[3][2], game_get_renderer(game), media->font, ")(a", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[3][3], game_get_renderer(game), media->font, "-b", COLOR_DRED)) return false;
    if(!texture_load_from_text(media->tex_text[3][4], game_get_renderer(game), media->font, ") = a²-ab-ba", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[3][5], game_get_renderer(game), media->font, "+b²", COLOR_DRED)) return false;

    if(!texture_load_from_text(media->tex_text[4][0], game_get_renderer(game), media->font, "(a-b)", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[4][1], game_get_renderer(game), media->font, "(a-b)", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[4][2], game_get_renderer(game), media->font, " = ", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[4][3], game_get_renderer(game), media->font, "a²-", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text[4][4], game_get_renderer(game), media->font, "2ab", COLOR_DRED)) return false;
    if(!texture_load_from_text(media->tex_text[4][5], game_get_renderer(game), media->font, "+b²", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    
    media->tex_letters[0] = texture_create();
    if (media->tex_letters[0] == NULL) return false;
    media->tex_letters[1] = texture_create();
    if (media->tex_letters[1] == NULL) return false;
    
    if(!texture_load_from_text(media->tex_letters[0], game_get_renderer(game), media->font, "a", COLOR_TEXT_DEFAULT)) return false;
    if(!texture_load_from_text(media->tex_letters[1], game_get_renderer(game), media->font, "b", COLOR_TEXT_DEFAULT)) return false;

    return true;
}

bool screens_demo2_loadobjects(Game *game, struct objects_t *objects, struct media_t *media, struct variables_t *var) {
    objects->balloon = balloon_create(media->tex_balloon, media->tex_anim, "txt/demo2_1.txt", true);
    if (objects->balloon == NULL) return false;
    return true;
}

void screens_demo2_loadvariables(Game *game, struct variables_t *var) {
    var->next = false;
    var->ret = SCREEN_NEXT;
    var->old_points = game_get_points(game, POINTS_ALL & ~POINTS_2);
    var->transition = 15;
    var->text_number = 0;
    var->text_offset = 8;
    var->play_sound = false;
}

Screen screens_demo2_close(struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    balloon_free(objects->balloon);
    texture_free(media->tex_bg);
    texture_free(media->tex_balloon);
    texture_free(media->tex_anim);
    texture_free(media->tex_name);
    texture_free(media->tex_points);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            texture_free(media->tex_text[i][j]);
        }
    }
    texture_free(media->tex_letters[0]);
    texture_free(media->tex_letters[1]);
    texture_free(media->tex_canvas);
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

void screens_demo2_drawbg(Game *game, struct media_t *media, struct variables_t *var) {
    //set canvas as render target
    texture_set_as_render_target(media->tex_canvas, game_get_renderer(game));
    //draw rect
    SDL_Rect bgrect = {0, 0, 256, 256};
    SDL_SetRenderDrawColor(game_get_renderer(game), COLOR_LWHITE.r, COLOR_LWHITE.g, COLOR_LWHITE.b, COLOR_LWHITE.a);
    SDL_RenderFillRect(game_get_renderer(game), &bgrect);
    SDL_SetRenderDrawColor(game_get_renderer(game), COLOR_DWHITE.r, COLOR_DWHITE.g, COLOR_DWHITE.b, COLOR_DWHITE.a);
    SDL_RenderDrawRect(game_get_renderer(game), &bgrect);

    //draw vertical lines
    SDL_SetRenderDrawColor(game_get_renderer(game), COLOR_DBLACK.r, COLOR_DBLACK.g, COLOR_DBLACK.b, COLOR_DBLACK.a);
    SDL_RenderDrawLine(game_get_renderer(game), 28, 33, 28, 222);
    SDL_RenderDrawLine(game_get_renderer(game), 24, 33, 28, 33);
    SDL_RenderDrawLine(game_get_renderer(game), 24, 222, 28, 222);

    SDL_RenderDrawLine(game_get_renderer(game), 16, 33, 16, 94);
    SDL_RenderDrawLine(game_get_renderer(game), 12, 33, 16, 33);
    SDL_RenderDrawLine(game_get_renderer(game), 12, 94, 16, 94);

    SDL_RenderDrawLine(game_get_renderer(game), 16, 97, 16, 158);
    SDL_RenderDrawLine(game_get_renderer(game), 12, 97, 16, 97);
    SDL_RenderDrawLine(game_get_renderer(game), 12, 158, 16, 158);

    //draw horizontal lines
    SDL_SetRenderDrawColor(game_get_renderer(game), COLOR_DBLACK.r, COLOR_DBLACK.g, COLOR_DBLACK.b, COLOR_DBLACK.a);
    SDL_RenderDrawLine(game_get_renderer(game), 33, 227, 222, 227);
    SDL_RenderDrawLine(game_get_renderer(game), 33, 227, 33, 231);
    SDL_RenderDrawLine(game_get_renderer(game), 222, 227, 222, 231);

    SDL_RenderDrawLine(game_get_renderer(game), 97, 239, 158, 239);
    SDL_RenderDrawLine(game_get_renderer(game), 97, 239, 97, 243);
    SDL_RenderDrawLine(game_get_renderer(game), 158, 239, 158, 243);

    SDL_RenderDrawLine(game_get_renderer(game), 161, 239, 222, 239);
    SDL_RenderDrawLine(game_get_renderer(game), 161, 239, 161, 243);
    SDL_RenderDrawLine(game_get_renderer(game), 222, 239, 222, 243);

    //draw letters
    texture_render(media->tex_letters[1], game_get_renderer(game), 8, 60, NULL);
    texture_render(media->tex_letters[1], game_get_renderer(game), 8, 124, NULL);
    texture_render(media->tex_letters[0], game_get_renderer(game), 20, 124, NULL);
    texture_render(media->tex_letters[0], game_get_renderer(game), 124, 228, NULL);
    texture_render(media->tex_letters[1], game_get_renderer(game), 124, 240, NULL);
    texture_render(media->tex_letters[1], game_get_renderer(game), 188, 240, NULL);

    //reset render target (NEVER FORGET)
    game_reset_render_target(game);
}

void screens_demo2_drawsquares(Game *game, struct media_t *media, struct variables_t *var) {
    //set canvas as render target
    texture_set_as_render_target(media->tex_canvas, game_get_renderer(game));
    //draw rect aa
    SDL_Rect rect_aa1 = {32, 96, 64, 128};
    SDL_Rect rect_aa2 = {96, 160, 64, 64};
    SDL_SetRenderDrawColor(game_get_renderer(game), COLOR_DBLUE.r, COLOR_DBLUE.g, COLOR_DBLUE.b, (255 - 255 * var->text_offset / 8 * (var->text_number == 0)) * (var->text_number >= 0));
    SDL_RenderFillRect(game_get_renderer(game), &rect_aa1);
    SDL_RenderFillRect(game_get_renderer(game), &rect_aa2);
    //draw rect ab
    SDL_Rect rect_ab = {32, 32, 192, 64};
    if (var->text_number < 1) {
        SDL_SetRenderDrawColor(game_get_renderer(game), COLOR_DBLUE.r, COLOR_DBLUE.g, COLOR_DBLUE.b, (255 - 255 * var->text_offset / 8 * (var->text_number == 0)) * (var->text_number >= 0));
    }
    else {
        SDL_SetRenderDrawColor(game_get_renderer(game), COLOR_DRED.r, COLOR_DRED.g, COLOR_DRED.b, (255 * var->text_offset / 8 * (var->text_number == 1)) * (var->text_number >= 1));
    }
    SDL_RenderFillRect(game_get_renderer(game), &rect_ab);
    //draw rect ba
    SDL_Rect rect_ba = {160, 96, 64, 128};
    if (var->text_number < 2) {
        SDL_SetRenderDrawColor(game_get_renderer(game), COLOR_DBLUE.r, COLOR_DBLUE.g, COLOR_DBLUE.b, (255 - 255 * var->text_offset / 8 * (var->text_number == 0)) * (var->text_number >= 0));
    }
    else {
        SDL_SetRenderDrawColor(game_get_renderer(game), COLOR_DRED.r, COLOR_DRED.g, COLOR_DRED.b, (255 * var->text_offset / 8 * (var->text_number == 2)) * (var->text_number >= 2));
    }
    SDL_RenderFillRect(game_get_renderer(game), &rect_ba);
    //draw rect bb
    SDL_Rect rect_bb = {96, 96, 64, 64};
    if (var->text_number < 2) {
        SDL_SetRenderDrawColor(game_get_renderer(game), COLOR_DBLUE.r, COLOR_DBLUE.g, COLOR_DBLUE.b, (255 - 255 * var->text_offset / 8 * (var->text_number == 0)) * (var->text_number >= 0));
    }
    else if (var->text_number == 2) {
        SDL_SetRenderDrawColor(game_get_renderer(game), COLOR_DRED.r, COLOR_DRED.g, COLOR_DRED.b, (255 * var->text_offset / 8 * (var->text_number == 2)) * (var->text_number >= 2));
    }
    else {
        SDL_SetRenderDrawColor(game_get_renderer(game), COLOR_DGREEN.r, COLOR_DGREEN.g, COLOR_DGREEN.b, (255 - 255 * var->text_offset / 8 * (var->text_number == 3)) * (var->text_number >= 3));
    }
    SDL_RenderFillRect(game_get_renderer(game), &rect_bb);

    //reset render target (NEVER FORGET)
    game_reset_render_target(game);
}

void screens_demo2_intro(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    while (!var->next) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (var->e.type == SDL_QUIT) {
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
    
        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        
        //draw canvas
        screens_demo2_drawbg(game, media, var);
        texture_render(media->tex_canvas, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_canvas))/2, 32, NULL);

        balloon_render(objects->balloon, game_get_renderer(game));

        game_render(game, var->transition);

        var->transition += 16 * (var->transition < 255);
    }
}

void screens_demo2_demo(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    while (!var->next) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (var->e.type == SDL_QUIT) {
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
                if (var->text_number > 4) {
                    var->next = true;
                    var->text_number = 4;
                }
                SDL_Color alpha_mod = {255, 255, 255, 255};
                for (int i = var->text_number; i >= 0; i--) {
                    for (int j = 0; j < 6; j++) {
                        texture_set_color_mod(media->tex_text[i][j], alpha_mod, SDL_BLENDMODE_BLEND);
                    }
                    alpha_mod.a /= 2;
                }
            }
        }


        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        
        //draw canvas
        screens_demo2_drawbg(game, media, var);
        screens_demo2_drawsquares(game, media, var);
        texture_render(media->tex_canvas, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_canvas))/2, 32, NULL);

        for (int k = 0; k <= var->text_number; k++) {
            for (int i = 0; i < 6; i++) {
                int x = 216;
                for (int j = 0; j < i; j++) {
                    x += texture_getw(media->tex_text[k][j]);
                }
                if (k < var->text_number) texture_render(media->tex_text[k][i], game_get_renderer(game), x, 320 - 8 * (var->text_number - k) + var->text_offset, NULL);
                else texture_render(media->tex_text[k][i], game_get_renderer(game), x, 320 - 8 * (var->text_number - k), NULL);
            }
        }

        game_render(game, var->transition);

        var->text_offset /= 1.1;
    }
}

void screens_demo2_end(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    while (!var->next || var->transition > 0) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (var->e.type == SDL_QUIT) {
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
    
        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        
        //draw canvas
        screens_demo2_drawbg(game, media, var);
        screens_demo2_drawsquares(game, media, var);
        texture_render(media->tex_canvas, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_canvas))/2, 32, NULL);

        for (int k = 0; k <= 4; k++) {
            for (int i = 0; i < 6; i++) {
                int x = 216;
                for (int j = 0; j < i; j++) {
                    x += texture_getw(media->tex_text[k][j]);
                }
                texture_render(media->tex_text[k][i], game_get_renderer(game), x, 320 - 8 * (4 - k), NULL);
            }
        }

        balloon_render(objects->balloon, game_get_renderer(game));
        balloon_render(objects->balloon, game_get_renderer(game));

        game_render(game, var->transition);
        
        if (var->next) {
            var->transition -= 16;
            var->transition *= (var->transition > 0);
        }
    }
}

Screen screens_demo2(Game *game) {
    struct media_t *media = (struct media_t*)malloc(sizeof(struct media_t));
    struct objects_t *objects = (struct objects_t*)malloc(sizeof(struct objects_t));
    struct variables_t *var = (struct variables_t*)malloc(sizeof(struct variables_t));
    if (media == NULL || objects == NULL || var == NULL) {
        printf("Couldnt open structs\n");
        return SCREEN_ERROR;
    }
    screens_demo2_loadvariables(game, var);
    if (!screens_demo2_loadmedia(game, media, var)) {
        printf("Couldnt open textures\n");
        return SCREEN_ERROR;
    }
    if (!screens_demo2_loadobjects(game, objects, media, var)) {
        printf("Couldnt open objects\n");
        return SCREEN_ERROR;
    }
    
    Mix_PlayMusic(media->music, -1);

    screens_demo2_intro(game, media, objects, var);

    var->next = false;
    if (var->ret == SCREEN_NEXT) {
        screens_demo2_demo(game, media, objects, var);
    }

    var->next = false;
    if (var->ret == SCREEN_NEXT) {
        if (!balloon_load_new_file(objects->balloon, "txt/demo2_2.txt", true)) {
            printf("Couldnt load new file\n");
            return SCREEN_ERROR;
        }
        screens_demo2_end(game, media, objects, var);
    }

    var->next = false;
    if (var->ret == SCREEN_NEXT) {
        var->ret = SCREEN_QUIZ2;
    }

    Mix_HaltMusic();

    return screens_demo2_close(media, objects, var);
}

