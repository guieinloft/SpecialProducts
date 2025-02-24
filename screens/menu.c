//libc includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//sdl includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

//object includes
#include "../objects/game.h"
#include "../objects/texture.h"
#include "../objects/button.h"
#include "../objects/colors.h"

//screen include
#include "screens.h"

struct media_t {
    Texture *tex_button;
    Texture *tex_text;
    Texture *tex_bg;
    Texture *tex_name;
    Texture *tex_points;
    SDL_Rect clip_bg;

    TTF_Font *font;

    Mix_Music *music;
};

struct objects_t {
    Button *button1;
    Button *button2;
    Button *button3;
    Button *button4;
    
    Button *bt_back;
    Button *bt_credits;
    Button *bt_exit;
};

struct variables_t {
    bool next;
    Screen ret;
    SDL_Event e;
    int transition;
    int points;
};

bool screens_menu_loadmedia(Game *game, struct media_t *media, struct variables_t *var) {
    media->tex_button = texture_create();
    if (media->tex_button == NULL) return false;
    media->tex_text = texture_create();
    if (media->tex_text == NULL) return false;
    media->tex_bg = texture_create();
    if (media->tex_bg == NULL) return false;
    media->tex_name = texture_create();
    if (media->tex_name == NULL) return false;
    media->tex_points = texture_create();
    if (media->tex_points == NULL) return false;

    if(!texture_load_from_file(media->tex_button, game_get_renderer(game), "img/ButtonTexture.png")) return false;
    
    media->font = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 8);
    if (media->font == NULL) return false;
    
    if(!texture_load_from_text(media->tex_text, game_get_renderer(game), media->font, "SELECIONE O PRODUTO NOTÁVEL:", COLOR_TEXT_DEFAULT_LIGHT)) return false;

    if(!texture_load_from_file(media->tex_bg, game_get_renderer(game), "img/BgTexture.png")) return false;
    media->clip_bg.x = 0;
    media->clip_bg.y = 360;
    media->clip_bg.w = SCREEN_W;
    media->clip_bg.h = SCREEN_H;
    
    media->music = Mix_LoadMUS("snd/dong.wav");
    if (media->music == NULL) return false;

    //NAME AND POINTS
    if(!texture_load_from_text(media->tex_name, game_get_renderer(game), media->font, game_get_name(game), COLOR_TEXT_DEFAULT_LIGHT)) return false;

    char str_points[5];
    sprintf(str_points, "%04d", var->points);
    if(!texture_load_from_text(media->tex_points, game_get_renderer(game), media->font, str_points, COLOR_TEXT_DEFAULT_LIGHT)) return false;

    return true;
}

bool screens_menu_loadobjects(Game *game, struct objects_t *objects, struct media_t *media) {
    objects->button1 = button_create(media->tex_button);
    if (objects->button1 == NULL) return false;
    objects->button2 = button_create(media->tex_button);
    if (objects->button2 == NULL) return false;
    objects->button3 = button_create(media->tex_button);
    if (objects->button3 == NULL) return false;
    objects->button4 = button_create(media->tex_button);
    if (objects->button4 == NULL) return false;
    
    if (!button_change_text(objects->button1, game_get_renderer(game), media->font, "1. QUADRADO DA SOMA", 32, COLOR_TEXT_DEFAULT, false)) return false;
    if (!button_change_text(objects->button2, game_get_renderer(game), media->font, "2. QUADRADO DA DIFERENÇA", 32, COLOR_TEXT_DEFAULT, false)) return false;
    if (!button_change_text(objects->button3, game_get_renderer(game), media->font, "3. P. DA SOMA PELA DIFERENÇA", 32, COLOR_TEXT_DEFAULT, false)) return false;
    if (!button_change_text(objects->button4, game_get_renderer(game), media->font, "4. EXPRESSÕES DE 2º GRAU", 32, COLOR_TEXT_DEFAULT, false)) return false;
    
    button_change_position(objects->button1, (SCREEN_W - button_get_real_w(objects->button1)) / 2, 128);
    button_change_position(objects->button2, (SCREEN_W - button_get_real_w(objects->button2)) / 2, 160);
    button_change_position(objects->button3, (SCREEN_W - button_get_real_w(objects->button3)) / 2, 192);
    button_change_position(objects->button4, (SCREEN_W - button_get_real_w(objects->button4)) / 2, 224);

    objects->bt_back = button_create(media->tex_button);
    if (objects->bt_back == NULL) return false;
    objects->bt_credits = button_create(media->tex_button);
    if (objects->bt_credits == NULL) return false;
    objects->bt_exit = button_create(media->tex_button);
    if (objects->bt_exit == NULL) return false;
    
    if (!button_change_text(objects->bt_back, game_get_renderer(game), media->font, "VOLTAR", 24, COLOR_TEXT_DEFAULT, true)) return false;
    if (!button_change_text(objects->bt_credits, game_get_renderer(game), media->font, "CRÉDITOS", 24, COLOR_TEXT_DEFAULT, true)) return false;
    if (!button_change_text(objects->bt_exit, game_get_renderer(game), media->font, "SAIR", 24, COLOR_TEXT_DEFAULT, true)) return false;
    button_change_position(objects->bt_back, 8, SCREEN_H - 32);
    button_change_position(objects->bt_credits, 8 + button_get_real_w(objects->bt_back), SCREEN_H - 32);
    button_change_position(objects->bt_exit, 8 + button_get_real_w(objects->bt_back) + button_get_real_w(objects->bt_credits), SCREEN_H - 32);

    return true;
}

void screens_menu_loadvariables(Game *game, struct variables_t *var) {
    var->next = false;
    var->ret = SCREEN_NEXT;
    var->transition = 15;
    var->points = game_get_points(game, POINTS_ALL);
}

Screen screens_menu_close(struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    button_free(objects->button1);
    button_free(objects->button2);
    button_free(objects->button3);
    button_free(objects->button4);
    button_free(objects->bt_back);
    button_free(objects->bt_credits);
    button_free(objects->bt_exit);
    texture_free(media->tex_button);
    texture_free(media->tex_text);
    texture_free(media->tex_bg);
    texture_free(media->tex_name);
    texture_free(media->tex_points);
    TTF_CloseFont(media->font);
    Mix_FreeMusic(media->music);
    free(media);
    free(objects);
    Screen ret = var->ret;
    free(var);
    return ret;
}

void screens_menu_menu(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    while (!var->next || var->transition > 0) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (var->e.type == SDL_QUIT) {
                var->next = true;
                var->ret = SCREEN_QUIT;
                var->transition = 0;
            }
            button_handle_event(objects->button1, var->e, game_get_scalex(game), game_get_scaley(game));
            button_handle_event(objects->button2, var->e, game_get_scalex(game), game_get_scaley(game));
            button_handle_event(objects->button3, var->e, game_get_scalex(game), game_get_scaley(game));
            button_handle_event(objects->button4, var->e, game_get_scalex(game), game_get_scaley(game));
            
            button_handle_event(objects->bt_back, var->e, game_get_scalex(game), game_get_scaley(game));
            button_handle_event(objects->bt_credits, var->e, game_get_scalex(game), game_get_scaley(game));
            button_handle_event(objects->bt_exit, var->e, game_get_scalex(game), game_get_scaley(game));
        }

        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        button_render(objects->button1, game_get_renderer(game));
        button_render(objects->button2, game_get_renderer(game));
        button_render(objects->button3, game_get_renderer(game));
        button_render(objects->button4, game_get_renderer(game));
        
        button_render(objects->bt_back, game_get_renderer(game));
        button_render(objects->bt_credits, game_get_renderer(game));
        button_render(objects->bt_exit, game_get_renderer(game));
        texture_render(media->tex_text, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_text)) / 2, 64, NULL);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        game_render(game, var->transition);

        if (button_isselected(objects->button1)) {
            var->ret = SCREEN_DEMO1;
            var->next = true;
        }
        if (button_isselected(objects->button2)) {
            var->ret = SCREEN_DEMO2;
            var->next = true;
        }
        if (button_isselected(objects->button3)) {
            var->ret = SCREEN_DEMO3;
            var->next = true;
        }
        if (button_isselected(objects->button4)) {
            var->ret = SCREEN_DEMO4;
            var->next = true;
        }
        if (button_isselected(objects->bt_back)) {
            var->ret = SCREEN_TITLESCREEN;
            var->next = true;
        }
        if (button_isselected(objects->bt_exit)) {
            var->ret = SCREEN_QUIT;
            var->next = true;
        }
        if (var->next) {
            var->transition -= 16;
            var->transition *= (var->transition > 0);
        }
        else {
            var->transition += 16 * (var->transition < 255);
        }
    }
}

Screen screens_menu(Game *game) {
    struct media_t *media = (struct media_t*)malloc(sizeof(struct media_t));
    struct objects_t *objects = (struct objects_t*)malloc(sizeof(struct objects_t));
    struct variables_t *var = (struct variables_t*)malloc(sizeof(struct variables_t));
    if (media == NULL || objects == NULL || var == NULL) {
        printf("Couldnt open structs\n");
        return SCREEN_ERROR;
    }
    screens_menu_loadvariables(game, var);
    if (!screens_menu_loadmedia(game, media, var)) {
        printf("Couldnt open textures\n");
        return SCREEN_ERROR;
    }
    if (!screens_menu_loadobjects(game, objects, media)) {
        printf("Couldnt open objects\n");
        return SCREEN_ERROR;
    }
    
    Mix_PlayMusic(media->music, -1);

    screens_menu_menu(game, media, objects, var);

    Mix_HaltMusic();
    return screens_menu_close(media, objects, var);
}
