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
    Texture *tex_prompt;
    Texture *tex_copyright;
    Texture *tex_bg;
    Texture *tex_title;

    TTF_Font *font;

    Mix_Music *music;
};

bool screens_titlescreen_loadmedia(Window *window, struct media_t *media) {
    media->tex_prompt = texture_create();
    if (media->tex_prompt == NULL) return false;
    media->tex_copyright = texture_create();
    if (media->tex_copyright == NULL) return false;
    media->tex_bg = texture_create();
    if (media->tex_bg == NULL) return false;
    media->tex_title = texture_create();
    if (media->tex_title == NULL) return false;

    media->font = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 8);
    if (media->font == NULL) return false;
    
    if(!texture_load_from_text(media->tex_prompt, window_get_renderer(window), media->font, "PRESSIONE O BOTÃO ESQUERDO DO MOUSE", COLOR_TEXT_DEFAULT)) return false;
    if(!texture_load_from_text(media->tex_copyright, window_get_renderer(window), media->font, "© 2025, Guilherme Einloft", COLOR_TEXT_DEFAULT)) return false;

    if(!texture_load_from_file(media->tex_bg, window_get_renderer(window), "img/BgTexture.png")) return false;
    
    if(!texture_load_from_file(media->tex_title, window_get_renderer(window), "img/TitleTexture.png")) return false;

    media->music = Mix_LoadMUS("snd/dong.wav");
    if (media->music == NULL) return false;

    return true;
}

void screens_titlescreen_close(struct media_t *media) {
    texture_free(media->tex_prompt);
    texture_free(media->tex_copyright);
    texture_free(media->tex_bg);
    texture_free(media->tex_title);
    TTF_CloseFont(media->font);
    Mix_FreeMusic(media->music);
}

Screen screens_titlescreen(Window *window) {
    struct media_t *media = (struct media_t*)malloc(sizeof(struct media_t));
    if (media == NULL) {
        printf("Couldnt open media struct\n");
        return SCREEN_ERROR;
    }
    if (!screens_titlescreen_loadmedia(window, media)) {
        printf("Couldnt open textures\n");
        return SCREEN_ERROR;
    }
    
    bool quit = false;
    Screen ret = SCREEN_QUIT;

    SDL_Event e;

    int flicker = 0;
    double size = 0.0;

    Mix_PlayMusic(media->music, -1);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                ret = SCREEN_MENU;
                quit = true;
            };
        }

        texture_render(media->tex_bg, window_get_renderer(window), 0, 0, NULL);
        if (flicker < 30) {
            texture_render(media->tex_prompt, window_get_renderer(window), (SCREEN_W - texture_getw(media->tex_prompt)) / 2, 256, NULL);
        }
        texture_render(media->tex_copyright, window_get_renderer(window), 8, SCREEN_H - 16, NULL);
        texture_render_scaled(media->tex_title, window_get_renderer(window), SCREEN_W / 2 - 128 * sin(size), 128 - 128 * sin(size), NULL, sin(size), sin(size), 0.0, NULL);
        window_render(window);

        size += 0.1 * (size < 1.6);
        flicker = (flicker + 1) % 40;
    }
    
    Mix_HaltMusic();
    screens_titlescreen_close(media);
    return ret;
}
