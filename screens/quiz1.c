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
#include "../objects/question.h"

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

// funcoes auxiliares
Question *question_1_7_create(bool inverted);
Question *question_2_8_create(bool inverted);
Question *question_3_9_create(bool inverted);
Question *question_4_10_create(bool inverted);
Question *question_5_11_create(bool inverted);
Question *question_6_12_create(bool inverted);
Question *question_n_create(int n);

bool screens_quiz1_loadmedia(Window *window, struct media_t *media) {
    media->tex_button = texture_create();
    if (media->tex_button == NULL) return false;
    media->tex_bg = texture_create();
    if (media->tex_bg == NULL) return false;
    media->tex_text = texture_create();
    if (media->tex_text == NULL) return false;

    if(!texture_load_from_file(media->tex_button, window_get_renderer(window), "img/ButtonTexture.png")) return false;
    
    media->font = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 8);
    if (media->font == NULL) return false;
    
    if(!texture_load_from_text(media->tex_text, window_get_renderer(window), media->font, "QUESTÕES", COLOR_TEXT_DEFAULT_LIGHT)) return false;

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

void screens_quiz1_close(QuestionContainer questions[12], struct media_t *media) {
    for (int i = 0; i < 12; i++) {
        question_free(questions[i].question);
        texture_free(questions[i].texture);
        textbox_free(questions[i].textbox);
    }
    texture_free(media->tex_button);
    texture_free(media->tex_text);
    texture_free(media->tex_bg);
    TTF_CloseFont(media->font);
    Mix_FreeMusic(media->music);
}

Screen screens_quiz1(Window *window) {
    struct media_t *media = (struct media_t*)malloc(sizeof(struct media_t));
    if (media == NULL) {
        printf("Couldnt open media struct\n");
        return SCREEN_ERROR;
    }
    if (!screens_quiz1_loadmedia(window, media)) {
        printf("Couldnt open textures\n");
        return SCREEN_ERROR;
    }

    QuestionContainer questions[12];

    for (int i = 0; i < 12; i++) {
        questions[i].x = 8 + (i > 5) * 320;
        questions[i].y = 32 + (i % 6) * 40;
        questions[i].correct = false;

        questions[i].question = question_n_create(i);
        
        questions[i].texture = texture_create();
        texture_load_from_text(questions[i].texture, window_get_renderer(window), media->font, question_get_q_text(questions[i].question), COLOR_TEXT_DEFAULT_LIGHT);

        questions[i].textbox = textbox_create(questions[i].x, questions[i].y+8, media->tex_button);
    }
    
    bool quit = false;
    int ret = SCREEN_QUIT;

    SDL_Event e;

    Mix_PlayMusic(media->music, -1);

    for (int i = 0; i < 12; i++) {
        
    }

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            for (int i = 0; i < 12; i++) {
                textbox_handle_event(questions[i].textbox, e, window_get_scalex(window), window_get_scaley(window));
            }
        }

        //check if questions are right
        bool correct = true;
        for (int i = 0; i < 12; i++) {
            if (textbox_get_enter(questions[i].textbox)) {
                questions[i].correct = question_compare(questions[i].question, textbox_get_text(questions[i].textbox));
                textbox_set_typable(questions[i].textbox, !questions[i].correct);
            }
            correct &= questions[i].correct;
        } 
        if (correct) {
            quit = true;
            ret = SCREEN_MENU;
        }
    
        texture_render(media->tex_bg, window_get_renderer(window), 0, 0, &media->clip_bg);
        texture_render(media->tex_text, window_get_renderer(window), (SCREEN_W - texture_getw(media->tex_text)) / 2, 16, NULL);

        for (int i = 0; i < 12; i++) {
            texture_render(questions[i].texture, window_get_renderer(window), questions[i].x, questions[i].y, NULL);
            textbox_render(questions[i].textbox, window_get_renderer(window), media->font);
        }

        window_render(window);
    }

    Mix_HaltMusic();
    screens_quiz1_close(questions, media);
    return ret;
}

//QUESTIONS {{{1

Question *question_1_7_create(bool inverted) {
    int a = rand() % 8 + 2;
    int b = 0;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(x+%d)^2", a);
    sprintf(text2, "x^2+%dx+%d", a*2, a*a);

    if (inverted) {
        return question_create(a, b, c, text2, text1);
    }
    return question_create(a, b, c, text1, text2);
}

Question *question_2_8_create(bool inverted) {
    int a = rand() % 8 + 2;
    int b = 0;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(%d+x)^2", a);
    sprintf(text2, "%d+%dx+x^2", a*a, a*2);

    if (inverted) {
        return question_create(a, b, c, text2, text1);
    }
    return question_create(a, b, c, text1, text2);
}

Question *question_3_9_create(bool inverted) {
    int a = rand() % 8 + 2;
    int b = 0;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(y+%d)^2", a);
    sprintf(text2, "y^2+%dy+%d", a*2, a*a);

    if (inverted) {
        return question_create(a, b, c, text2, text1);
    }
    return question_create(a, b, c, text1, text2);
}

Question *question_4_10_create(bool inverted) {
    int a = (rand() % 4 + 1) * 2 + 1;
    int b = 0;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(x/2+%d/2)^2", a);
    sprintf(text2, "x^2/4+%dx+%d/4", a, a*a);

    if (inverted) {
        return question_create(a, b, c, text2, text1);
    }
    return question_create(a, b, c, text1, text2);
}

Question *question_5_11_create(bool inverted) {
    int a = rand() % 3 + 2;
    int b = rand() % 8 + 2;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(%dx+%d)^2", a, b);
    sprintf(text2, "%dx^2+%dx+%d", a*a, 2*a*b, b*b);

    if (inverted) {
        return question_create(a, b, c, text2, text1);
    }
    return question_create(a, b, c, text1, text2);
}

Question *question_6_12_create(bool inverted) {
    int a = rand() % 8 + 2;
    int b = rand() % 8 + 2;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(%dx+%dy)^2", a, b);
    sprintf(text2, "%dx^2+%dxy+%dy^2", a*a, 2*a*b, b*b);

    if (inverted) {
        return question_create(a, b, c, text2, text1);
    }
    return question_create(a, b, c, text1, text2);
}

Question *question_n_create(int n) {
    switch(n) {
        case 0:
        return question_1_7_create(false);
        case 1:
        return question_2_8_create(false);
        case 2:
        return question_3_9_create(false);
        case 3:
        return question_4_10_create(false);
        case 4:
        return question_5_11_create(false);
        case 5:
        return question_6_12_create(false);
        case 6:
        return question_1_7_create(true);
        case 7:
        return question_2_8_create(true);
        case 8:
        return question_3_9_create(true);
        case 9:
        return question_4_10_create(true);
        case 10:
        return question_5_11_create(true);
        case 11:
        return question_6_12_create(true);
        default:
        return NULL;
    }
}
