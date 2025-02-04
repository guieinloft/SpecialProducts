//libc includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

//sdl includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

//object includes
#include "../objects/window.h"
#include "../objects/texture.h"
#include "../objects/button.h"
#include "../objects/textbox.h"
#include "../objects/colors.h"

//screen include
#include "screens.h"

struct media_t {
    Texture *tex_button;
    Texture *tex_text;
    Texture *tex_text1;
    Texture *tex_text2;
    Texture *tex_text3;
    Texture *tex_text4;
    Texture *tex_bg;
    SDL_Rect clip_bg;

    TTF_Font *font;

    Mix_Music *music;
};

bool screens_testscreen1_loadmedia(Window *window, struct media_t *media) {
    media->tex_button = texture_create();
    if (media->tex_button == NULL) return false;
    media->tex_text = texture_create();
    if (media->tex_text == NULL) return false;
    media->tex_text1 = texture_create();
    if (media->tex_text1 == NULL) return false;
    media->tex_text2 = texture_create();
    if (media->tex_text2 == NULL) return false;
    media->tex_text3 = texture_create();
    if (media->tex_text3 == NULL) return false;
    media->tex_text4 = texture_create();
    if (media->tex_text4 == NULL) return false;
    media->tex_bg = texture_create();
    if (media->tex_bg == NULL) return false;

    if(!texture_load_from_file(media->tex_button, window_get_renderer(window), "img/ButtonTexture.png")) return false;
    
    media->font = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 8);
    if (media->font == NULL) return false;
    
    if(!texture_load_from_text(media->tex_text, window_get_renderer(window), media->font, "TELA DE TESTE 1", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if(!texture_load_from_text(media->tex_text1, window_get_renderer(window), media->font, "çççççç", COLOR_TEXT_DEFAULT)) return false;
    if(!texture_load_from_text(media->tex_text2, window_get_renderer(window), media->font, "ÇÇÇÇÇÇ", COLOR_TEXT_DEFAULT)) return false;
    if(!texture_load_from_text(media->tex_text3, window_get_renderer(window), media->font, "áàãéêíóôõúç", COLOR_TEXT_DEFAULT)) return false;
    if(!texture_load_from_text(media->tex_text4, window_get_renderer(window), media->font, "ÁÀÃÉÊÍÓÔÕÚÇ", COLOR_TEXT_DEFAULT)) return false;
    printf("%d %d %d %d\n", texture_geth(media->tex_text1), texture_geth(media->tex_text2), texture_geth(media->tex_text3), texture_geth(media->tex_text4));

    if(!texture_load_from_file(media->tex_bg, window_get_renderer(window), "img/BgTexture.png")) return false;

    media->clip_bg.x = 0;
    media->clip_bg.y = 360;
    media->clip_bg.w = SCREEN_W;
    media->clip_bg.h = SCREEN_H;

    texture_set_color_mod(media->tex_bg, COLOR_LBLUE, SDL_BLENDMODE_BLEND);
    
    media->music = Mix_LoadMUS("snd/dong.wav");
    if (media->music == NULL) return false;

    return true;
}

void screens_testscreen1_close(Button *button1, Button *button2, Textbox *textbox, struct media_t *media) {
    button_free(button1);
    button_free(button2);
    textbox_free(textbox);
    texture_free(media->tex_button);
    texture_free(media->tex_text);
    texture_free(media->tex_bg);
    TTF_CloseFont(media->font);
    Mix_FreeMusic(media->music);
}

Screen screens_testscreen1(Window *window) {
    struct media_t *media = (struct media_t*)malloc(sizeof(struct media_t));
    if (media == NULL) {
        printf("Couldnt open media struct\n");
        return SCREEN_ERROR;
    }
    if (!screens_testscreen1_loadmedia(window, media)) {
        printf("Couldnt open textures\n");
        return SCREEN_ERROR;
    }
    
    Button *button1 = button_create(media->tex_button);
    Button *button2 = button_create(media->tex_button);
    Textbox *textbox = textbox_create((SCREEN_W - 256) / 2, 192, media->tex_button);

    if (button1 == NULL || button2 == NULL || textbox == NULL) {
        printf("Couldnt make buttons\n");
        return SCREEN_ERROR;
    }
    if (!button_change_text(button1, window_get_renderer(window), media->font, "VOLTAR", 52, COLOR_TEXT_DEFAULT, false)) {
        printf("Couldnt change button text\n");
        return SCREEN_ERROR;
    }
    if (!button_change_text(button2, window_get_renderer(window), media->font, "ANALISAR TEXTO", 30, COLOR_TEXT_DEFAULT, false)) {
        printf("Couldnt change button text\n");
        return SCREEN_ERROR;
    }
    button_change_position(button1, (SCREEN_W - button_get_real_w(button1)) / 2, 256);
    button_change_position(button2, (SCREEN_W - button_get_real_w(button2)) / 2, 224);

    bool quit = false;
    int ret = SCREEN_QUIT;

    SDL_Event e;

    double sum = 0.0;

    char str_correct[] = "Camilla";
    char *str_textbox;

    Mix_PlayMusic(media->music, -1);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            button_handle_event(button1, e, window_get_scalex(window), window_get_scaley(window));
            button_handle_event(button2, e, window_get_scalex(window), window_get_scaley(window));
            textbox_handle_event(textbox, e, window_get_scalex(window), window_get_scaley(window));
        }

        if (button_isselected(button1)) {
            quit = true;
        }
        if (button_ispressed(button2)) {
            textbox_set_enter(textbox, true);
        }
        if (textbox_get_enter(textbox)) {
            str_textbox = textbox_get_text(textbox);
            printf("TEXTO: %s\n", str_textbox);
            if (strcmp(str_textbox, str_correct) == 0) {
                textbox_set_typable(textbox, false);
            }
        }
        
        texture_render(media->tex_bg, window_get_renderer(window), 0, 0, &media->clip_bg);
        button_render(button1, window_get_renderer(window));
        button_render(button2, window_get_renderer(window));
        textbox_render(textbox, window_get_renderer(window), media->font);
        texture_render_scaled(media->tex_text, window_get_renderer(window), (SCREEN_W - texture_getw(media->tex_text) * 2 * fabs(sin(sum))) / 2, 64, NULL, 2 * sin(sum), 2, 0, NULL);
        window_render(window);


        sum += 0.05;
    }
    
    Mix_HaltMusic();
    screens_testscreen1_close(button1, button2, textbox, media);
    return ret;
}
