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
#include "../objects/window.h"
#include "../objects/texture.h"
#include "../objects/button.h"
#include "../objects/colors.h"

//screen include
#include "screens.h"

struct media_t {
    Texture *tex_button;
    Texture *tex_text;
    Texture *tex_bg;
    SDL_Rect clip_bg;

    TTF_Font *font;

    Mix_Music *music;
};

bool screens_menu_loadmedia(Window *window, struct media_t *media) {
    media->tex_button = texture_create();
    if (media->tex_button == NULL) return false;
    media->tex_text = texture_create();
    if (media->tex_text == NULL) return false;
    media->tex_bg = texture_create();
    if (media->tex_bg == NULL) return false;

    if(!texture_load_from_file(media->tex_button, window_get_renderer(window), "img/ButtonTexture.png")) return false;
    
    media->font = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 8);
    if (media->font == NULL) return false;
    
    if(!texture_load_from_text(media->tex_text, window_get_renderer(window), media->font, "SELECIONE O PRODUTO NOTÁVEL:", COLOR_TEXT_DEFAULT_LIGHT)) return false;

    if(!texture_load_from_file(media->tex_bg, window_get_renderer(window), "img/BgTexture.png")) return false;
    media->clip_bg.x = 0;
    media->clip_bg.y = 360;
    media->clip_bg.w = SCREEN_W;
    media->clip_bg.h = SCREEN_H;
    
    media->music = Mix_LoadMUS("snd/dong.wav");
    if (media->music == NULL) return false;

    return true;
}

void screens_menu_close(Button *button2, struct media_t *media) {
    button_free(button2);
    texture_free(media->tex_button);
    texture_free(media->tex_text);
    texture_free(media->tex_bg);
    TTF_CloseFont(media->font);
    Mix_FreeMusic(media->music);
}

Screen screens_menu(Window *window) {
    struct media_t *media = (struct media_t*)malloc(sizeof(struct media_t));
    if (media == NULL) {
        printf("Couldnt open media struct\n");
        return SCREEN_ERROR;
    }
    if (!screens_menu_loadmedia(window, media)) {
        printf("Couldnt open textures\n");
        return SCREEN_ERROR;
    }
    
    Button *button1 = button_create(media->tex_button);
    Button *button2 = button_create(media->tex_button);
    Button *button3 = button_create(media->tex_button);
    Button *button4 = button_create(media->tex_button);

    if (button1 == NULL || button2 == NULL || button3 == NULL || button4 == NULL) {
        printf("Couldnt make buttons\n");
        return SCREEN_ERROR;
    }
    
    if (!button_change_text(button1, window_get_renderer(window), media->font, "1. QUADRADO DA SOMA", 32, COLOR_TEXT_DEFAULT, false)) {
        printf("Couldnt change button text\n");
        return SCREEN_ERROR;
    }
    if (!button_change_text(button2, window_get_renderer(window), media->font, "2. QUADRADO DA DIFERENÇA", 32, COLOR_TEXT_DEFAULT, false)) {
        printf("Couldnt change button text\n");
        return SCREEN_ERROR;
    }
    if (!button_change_text(button3, window_get_renderer(window), media->font, "3. P. DA SOMA PELA DIFERENÇA", 32, COLOR_TEXT_DEFAULT, false)) {
        printf("Couldnt change button text\n");
        return SCREEN_ERROR;
    }
    if (!button_change_text(button4, window_get_renderer(window), media->font, "4. EXPRESSÕES DE 2º GRAU", 32, COLOR_TEXT_DEFAULT, false)) {
        printf("Couldnt change button text\n");
        return SCREEN_ERROR;
    }
    button_change_position(button1, (SCREEN_W - button_get_real_w(button1)) / 2, 128);
    button_change_position(button2, (SCREEN_W - button_get_real_w(button2)) / 2, 160);
    button_change_position(button3, (SCREEN_W - button_get_real_w(button3)) / 2, 192);
    button_change_position(button4, (SCREEN_W - button_get_real_w(button4)) / 2, 224);

    bool quit = false;
    int ret = SCREEN_QUIT;

    SDL_Event e;

    Mix_PlayMusic(media->music, -1);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            button_handle_event(button1, e, window_get_scalex(window), window_get_scaley(window));
            button_handle_event(button2, e, window_get_scalex(window), window_get_scaley(window));
            button_handle_event(button3, e, window_get_scalex(window), window_get_scaley(window));
            button_handle_event(button4, e, window_get_scalex(window), window_get_scaley(window));
        }

        texture_render(media->tex_bg, window_get_renderer(window), 0, 0, &media->clip_bg);
        button_render(button1, window_get_renderer(window));
        button_render(button2, window_get_renderer(window));
        button_render(button3, window_get_renderer(window));
        button_render(button4, window_get_renderer(window));
        texture_render(media->tex_text, window_get_renderer(window), (SCREEN_W - texture_getw(media->tex_text)) / 2, 64, NULL);
        window_render(window);

        if (button_isselected(button1)) {
            ret = SCREEN_QUIZ1;
            quit = true;
        }
        if (button_isselected(button2)) {
            ret = SCREEN_DEMO2;
            quit = true;
        }
        if (button_isselected(button3)) {
            ret = SCREEN_DEMO3;
            quit = true;
        }
        if (button_isselected(button4)) {
            ret = SCREEN_DEMO4;
            quit = true;
        }
    }
    
    Mix_HaltMusic();
    screens_menu_close(button2, media);
    return ret;
}
