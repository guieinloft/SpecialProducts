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
    Texture *tex_title;

    TTF_Font *font_normal;
    TTF_Font *font_light;

    Mix_Music *music;
};

bool screens_TitleScreen_loadmedia(Window *window, struct media_t *media) {
    media->tex_button = texture_create();
    if (media->tex_button == NULL) return false;
    media->tex_text = texture_create();
    if (media->tex_text == NULL) return false;
    media->tex_bg = texture_create();
    if (media->tex_bg == NULL) return false;
    media->tex_title = texture_create();
    if (media->tex_title == NULL) return false;

    if(!texture_load_from_file(media->tex_button, window_get_renderer(window), "img/ButtonTexture256.png")) return false;
    
    media->font_normal = TTF_OpenFont("fonts/PlayerSansMono8x13-Classic.ttf", 8);
    if (media->font_normal == NULL) return false;
    
    media->font_light = TTF_OpenFont("fonts/PlayerSansMono8x13-Light.ttf", 8);
    if (media->font_light == NULL) return false;
    
    SDL_Color textColor = {0, 0, 0, 255};
    if(!texture_load_from_text(media->tex_text, window_get_renderer(window), media->font_light, "COPYRIGHT Guilherme Einloft 2025", textColor)) return false;

    if(!texture_load_from_file(media->tex_bg, window_get_renderer(window), "img/BgTexture.png")) return false;
    
    if(!texture_load_from_file(media->tex_title, window_get_renderer(window), "img/TitleTexture.png")) return false;

    media->music = Mix_LoadMUS("snd/dong.wav");
    if (media->music == NULL) return false;

    return true;
}

void screens_TitleScreen_close(Button *button1, Button *button2, struct media_t *media) {
    button_free(button1);
    button_free(button2);
    texture_free(media->tex_button);
    texture_free(media->tex_text);
    texture_free(media->tex_bg);
    texture_free(media->tex_title);
    TTF_CloseFont(media->font_normal);
    TTF_CloseFont(media->font_light);
    Mix_FreeMusic(media->music);
}

Screen screens_TitleScreen(Window *window) {
    struct media_t *media = (struct media_t*)malloc(sizeof(struct media_t));
    if (media == NULL) {
        printf("Couldnt open media struct\n");
        return SCREEN_ERROR;
    }
    if (!screens_TitleScreen_loadmedia(window, media)) {
        printf("Couldnt open textures\n");
        return SCREEN_ERROR;
    }
    
    Button *button1 = button_create(SCREEN_W / 2 - 128, 256, 256, media->tex_button);
    Button *button2 = button_create(SCREEN_W / 2 - 128, 288, 256, media->tex_button);

    if (button1 == NULL || button2 == NULL) {
        printf("Couldnt make buttons\n");
        return SCREEN_ERROR;
    }
    SDL_Color text_color = {0, 0, 0, 255};
    if (!button_change_text(button1, window_get_renderer(window), media->font_normal, "INICIAR JOGO", text_color, true)) {
        printf("Couldnt change button text\n");
        return -1;
    }
    if (!button_change_text(button2, window_get_renderer(window), media->font_normal, "OPÇÕES", text_color, true)) {
        printf("Couldnt change button text\n");
        return SCREEN_ERROR;
    }

    bool quit = false;
    Screen ret = SCREEN_QUIT;

    SDL_Event e;

    Mix_PlayMusic(media->music, -1);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            button_handle_event(button1, e, 1.0, 1.0);
            button_handle_event(button2, e, 1.0, 1.0);
        }

        texture_render(media->tex_bg, window_get_renderer(window), 0, 0, NULL);
        button_render(button1, window_get_renderer(window));
        button_render(button2, window_get_renderer(window));
        texture_render(media->tex_text, window_get_renderer(window), 8, SCREEN_H - 16, NULL);
        texture_render(media->tex_title, window_get_renderer(window), SCREEN_W / 2 - 128, 0, NULL);
        window_render(window);

        if (button_isselected(button1)) {
            ret = SCREEN_TESTSCREEN1;
            quit = true;
        }
        if (button_isselected(button2)) {
            ret = SCREEN_TESTSCREEN2;
            quit = true;
        }
    }
    
    Mix_HaltMusic();
    screens_TitleScreen_close(button1, button2, media);
    return ret;
}
