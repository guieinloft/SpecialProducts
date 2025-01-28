//libc includes
#include <stdio.h>
#include <stdbool.h>

//sdl includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//object includes
#include "../objects/window.h"
#include "../objects/texture.h"
#include "../objects/button.h"

bool screens_TitleScreen_loadtextures(Window *window, Texture *buttonTexture, Texture *textTexture, TTF_Font **font) {
    if(!texture_load_from_file(buttonTexture, window_get_renderer(window), "img/ButtonTexture256.png")) return false;
    *font = TTF_OpenFont("fonts/PlayerSansMono8x13-Classic.ttf", 8);
    if (*font == NULL) return false;

    SDL_Color textColor = {0, 0, 0, 255};
    if(!texture_load_from_text(textTexture, window_get_renderer(window), *font, "TESTE", textColor)) return false;
    return true;
}

void screens_TitleScreen_close(Button *button1, Button *button2, Texture *buttonTexture, Texture *textTexture, TTF_Font *font) {
    button_free(button1);
    button_free(button2);
    texture_free(buttonTexture);
    texture_free(textTexture);
    TTF_CloseFont(font);
}

int screens_TitleScreen(Window *window) {
    Texture *buttonTexture = texture_create();
    Texture *textTexture = texture_create();
    TTF_Font *font = NULL;
    if (!screens_TitleScreen_loadtextures(window, buttonTexture, textTexture, &font)) {
        printf("Couldnt open textures\n");
        return -1;
    }
    
    Button *button1 = button_create(0, 0, 256, buttonTexture);
    Button *button2 = button_create(0, 32, 256, buttonTexture);

    if (button1 == NULL || button2 == NULL) {
        printf("Couldnt make buttons\n");
        return -1;
    }
    SDL_Color text_color = {0, 0, 0, 255};
    if (!button_change_text(button1, window_get_renderer(window), font, "BOTAO 01", 8, text_color)) {
        printf("Couldnt change button text\n");
        return -1;
    }
    if (!button_change_text(button2, window_get_renderer(window), font, "123456789012345678901234567890", 30, text_color)) {
        printf("Couldnt change button text\n");
        return -1;
    }

    bool quit = false;
    int ret = 0;

    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            button_handle_event(button1, e, 1.0, 1.0);
            button_handle_event(button2, e, 1.0, 1.0);
        }

        button_render(button1, window_get_renderer(window));
        button_render(button2, window_get_renderer(window));
        texture_render(textTexture, window_get_renderer(window), 0, 300, NULL);
        window_render(window);

        if (button_isselected(button1)) {
            ret = 1;
            quit = true;
        }
        if (button_isselected(button2)) {
            ret = 2;
            quit = true;
        }
    }
    
    screens_TitleScreen_close(button1, button2, buttonTexture, textTexture, font);
    return ret;
}
