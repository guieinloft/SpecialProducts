//libc includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//sdl includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

//object includes
#include "../objects/window.h"
#include "../objects/texture.h"
#include "../objects/button.h"

//screen include
#include "screens.h"

struct media_t {
    Texture *tex_button;
    Texture *tex_text;
    Texture *tex_bg;
    SDL_Rect clip_bg;

    TTF_Font *font_normal;

    Mix_Music *music;
};

bool screens_TestScreen2_loadmedia(Window *window, struct media_t *media) {
    media->tex_button = texture_create();
    if (media->tex_button == NULL) return false;
    media->tex_text = texture_create();
    if (media->tex_text == NULL) return false;
    media->tex_bg = texture_create();
    if (media->tex_bg == NULL) return false;

    if(!texture_load_from_file(media->tex_button, window_get_renderer(window), "img/ButtonTexture256.png")) return false;
    
    media->font_normal = TTF_OpenFont("fonts/PlayerSansMono8x13-Classic.ttf", 8);
    if (media->font_normal == NULL) return false;
    
    SDL_Color textColor = {0, 0, 0, 255};
    if(!texture_load_from_text(media->tex_text, window_get_renderer(window), media->font_normal, "TELA DE TESTE 2", textColor)) return false;

    if(!texture_load_from_file(media->tex_bg, window_get_renderer(window), "img/BgTexture.png")) return false;
    media->clip_bg.x = 0;
    media->clip_bg.y = 360;
    media->clip_bg.w = SCREEN_W;
    media->clip_bg.h = SCREEN_H;
    
    media->music = Mix_LoadMUS("snd/dong.wav");
    if (media->music == NULL) return false;

    return true;
}

void screens_TestScreen2_close(Button *button1, struct media_t *media) {
    button_free(button1);
    texture_free(media->tex_button);
    texture_free(media->tex_text);
    texture_free(media->tex_bg);
    TTF_CloseFont(media->font_normal);
    Mix_FreeMusic(media->music);
}

Screen screens_TestScreen2(Window *window) {
    struct media_t *media = (struct media_t*)malloc(sizeof(struct media_t));
    if (media == NULL) {
        printf("Couldnt open media struct\n");
        return SCREEN_ERROR;
    }
    if (!screens_TestScreen2_loadmedia(window, media)) {
        printf("Couldnt open textures\n");
        return SCREEN_ERROR;
    }
    
    Button *button1 = button_create(SCREEN_W / 2 - 128, 256, 256, media->tex_button);

    if (button1 == NULL) {
        printf("Couldnt make buttons\n");
        return SCREEN_ERROR;
    }
    SDL_Color text_color = {0, 0, 0, 255};
    if (!button_change_text(button1, window_get_renderer(window), media->font_normal, "SAIR", text_color, false)) {
        printf("Couldnt change button text\n");
        return SCREEN_ERROR;
    }

    bool quit = false;
    int ret = SCREEN_QUIT;

    SDL_Event e;

    Mix_PlayMusic(media->music, -1);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            button_handle_event(button1, e, 1.0, 1.0);
        }

        texture_render(media->tex_bg, window_get_renderer(window), 0, 0, &media->clip_bg);
        button_render(button1, window_get_renderer(window));
        texture_render(media->tex_text, window_get_renderer(window), (SCREEN_W - texture_getw(media->tex_text)) / 2, 64, NULL);
        window_render(window);

        if (button_isselected(button1)) {
            ret = SCREEN_QUIT;
            quit = true;
        }
    }
    
    Mix_HaltMusic();
    screens_TestScreen2_close(button1, media);
    return ret;
}
