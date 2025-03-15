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

//screen include
#include "screens.h"

struct media_t {
    Texture *tex_button;
    Texture *tex_text;
    Texture *tex_bg;
    Texture *tex_name;
    Texture *tex_points;
    Texture *tex_time;
    Texture *tex_questions[13];
    
    SDL_Rect clip_bg;

    TTF_Font *font;

    Mix_Music *music;
    Mix_Chunk *sfx_click;
    Mix_Chunk *sfx_right;
    Mix_Chunk *sfx_wrong;
    Mix_Chunk *sfx_auto;
    Mix_Chunk *sfx_win;
    Mix_Chunk *sfx_lose;
    Mix_Chunk *sfx_tally;
};

struct objects_t {
    Question *questions[13];
    Textbox *textboxes[13];
    Button *bt_enter[13];
    Button *bt_complete[13];
    Timer *timer;
};

struct variables_t {
    bool next;
    bool correct[13];
    bool correct_all;
    bool autocomplete;
    bool rerender_points;
    char str_points[7];
    char str_time[15];
    int old_points;
    int current_points;
    int remaining_time;
    int question_x[13];
    int question_y[13];
    Screen ret;
    SDL_Event e;
    int transition;
};

//QUESTION DEFINITIONS
Question *quiz3_question_n_create(int n);
Question *quiz3_question_1_7_create(bool inverted);
Question *quiz3_question_2_8_create(bool inverted);
Question *quiz3_question_3_9_create(bool inverted);
Question *quiz3_question_4_10_create(bool inverted);
Question *quiz3_question_5_11_create(bool inverted);
Question *quiz3_question_6_12_create(bool inverted);
Question *quiz3_question_13_create();

bool screens_quiz3_loadmedia(Game *game, struct media_t *media, struct variables_t *var) {
    media->tex_button = texture_create();
    if (media->tex_button == NULL) return false;
    media->tex_bg = texture_create();
    if (media->tex_bg == NULL) return false;
    media->tex_text = texture_create();
    if (media->tex_text == NULL) return false;
    media->tex_name = texture_create();
    if (media->tex_name == NULL) return false;
    media->tex_points = texture_create();
    if (media->tex_points == NULL) return false;
    media->tex_time = texture_create();
    if (media->tex_time == NULL) return false;

    if(!texture_load_from_file(media->tex_button, game_get_renderer(game), "img/ButtonTexture.png")) return false;
    
    media->font = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 8);
    if (media->font == NULL) return false;
    
    if(!texture_load_from_text(media->tex_text, game_get_renderer(game), media->font, "QUESTÕES", COLOR_TEXT_DEFAULT_LIGHT)) return false;

    if(!texture_load_from_file(media->tex_bg, game_get_renderer(game), "img/BgTexture.png")) return false;

    media->clip_bg.x = 0;
    media->clip_bg.y = 360;
    media->clip_bg.w = SCREEN_W;
    media->clip_bg.h = SCREEN_H;

    texture_set_color_mod(media->tex_bg, COLOR_LYELLOW, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < 13; i++) {
        media->tex_questions[i] = texture_create();
        if (media->tex_questions[i] == NULL) return false;
    }
    
    media->music = Mix_LoadMUS("snd/quizproducts.wav");
    if (media->music == NULL) return false;
    media->sfx_click = Mix_LoadWAV("snd/sfx_click.wav");
    if (media->sfx_click == NULL) return false;
    media->sfx_right = Mix_LoadWAV("snd/sfx_right.wav");
    if (media->sfx_right == NULL) return false;
    media->sfx_wrong = Mix_LoadWAV("snd/sfx_wrong.wav");
    if (media->sfx_wrong == NULL) return false;
    media->sfx_auto = Mix_LoadWAV("snd/sfx_auto.wav");
    if (media->sfx_auto == NULL) return false;
    media->sfx_win = Mix_LoadWAV("snd/sfx_win.wav");
    if (media->sfx_win == NULL) return false;
    media->sfx_lose = Mix_LoadWAV("snd/sfx_lose.wav");
    if (media->sfx_lose == NULL) return false;
    media->sfx_tally = Mix_LoadWAV("snd/sfx_tally.wav");
    if (media->sfx_tally == NULL) return false;

    //NAME AND POINTS
    if(!texture_load_from_text(media->tex_name, game_get_renderer(game), media->font, game_get_name(game), COLOR_TEXT_DEFAULT_LIGHT)) return false;

    sprintf(var->str_points, "%06d", var->old_points);
    if(!texture_load_from_text(media->tex_points, game_get_renderer(game), media->font, var->str_points, COLOR_TEXT_DEFAULT_LIGHT)) return false;

    return true;
}

bool screens_quiz3_loadobjects(Game *game, struct objects_t *objects, struct media_t *media, struct variables_t *var) {
    //create question containers
    for (int i = 0; i < 13; i++) {
        objects->questions[i] = quiz3_question_n_create(i);
        if (objects->questions[i] == NULL) return false;
        
        texture_load_from_text(media->tex_questions[i], game_get_renderer(game), media->font, question_get_q_text(objects->questions[i]), COLOR_TEXT_DEFAULT_LIGHT);

        objects->textboxes[i] = textbox_create(var->question_x[i], var->question_y[i]+8, media->tex_button);
        if (objects->textboxes[i] == NULL) return false;

        objects->bt_enter[i] = button_create(media->tex_button);
        if (objects->bt_enter[i] == NULL) return false;
        button_change_position(objects->bt_enter[i], var->question_x[i]+256, var->question_y[i]+8);
        
        objects->bt_complete[i] = button_create(media->tex_button);
        if (objects->bt_complete[i] == NULL) return false;
        button_change_position(objects->bt_complete[i], var->question_x[i]+280, var->question_y[i]+8);
    }
    char str_question_bonus[32];
    sprintf(str_question_bonus, "BÔNUS: %s", question_get_q_text(objects->questions[12]));
    texture_load_from_text(media->tex_questions[12], game_get_renderer(game), media->font, str_question_bonus, COLOR_TEXT_DEFAULT_LIGHT);

    //create timer
    objects->timer = timer_create();
    if (objects->timer == NULL) return false;
    return true;
}

void screens_quiz3_loadvariables(Game *game, struct variables_t *var) {
    var->next = false;
    for (int i = 0; i < 12; i++) {
        var->correct[i] = false;
        var->question_x[i] = 8 + (i > 5) * SCREEN_W / 2;
        var->question_y[i] = 64 + (i % 6) * 40;
    }
    var->correct[12] = false;
    var->question_x[12] = 168;
    var->question_y[12] = 304;
    var->correct_all = false;
    var->autocomplete = false;
    var->rerender_points = false;
    var->ret = SCREEN_NEXT;
    var->old_points = game_get_points(game, POINTS_ALL & ~POINTS_3);
    var->current_points = 0;
    var->remaining_time = TICKS_TOTAL;
    var->transition = 15;
}

Screen screens_quiz3_close(struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    for (int i = 0; i < 13; i++) {
        question_free(objects->questions[i]);
        texture_free(media->tex_questions[i]);
        textbox_free(objects->textboxes[i]);
        button_free(objects->bt_enter[i]);
        button_free(objects->bt_complete[i]);
    }
    timer_free(objects->timer);
    texture_free(media->tex_button);
    texture_free(media->tex_text);
    texture_free(media->tex_bg);
    texture_free(media->tex_time);
    texture_free(media->tex_name);
    texture_free(media->tex_points);
    TTF_CloseFont(media->font);
    Mix_FreeMusic(media->music);
    Mix_FreeChunk(media->sfx_click);
    Mix_FreeChunk(media->sfx_right);
    Mix_FreeChunk(media->sfx_wrong);
    Mix_FreeChunk(media->sfx_auto);
    Mix_FreeChunk(media->sfx_win);
    Mix_FreeChunk(media->sfx_lose);
    Mix_FreeChunk(media->sfx_tally);
    free(media);
    free(objects);
    Screen ret = var->ret;
    free(var);
    return ret;
}

void screens_quiz3_quiz(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    timer_start(objects->timer);
    while (!var->next) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                game_save(game, false);
                var->next = true;
                var->ret = SCREEN_QUIT;
                var->transition = 0;
            }
            for (int i = 0; i < 13; i++) {
                textbox_handle_event(objects->textboxes[i], var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
                button_handle_event(objects->bt_enter[i], var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
                button_handle_event(objects->bt_complete[i], var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
            }
        }

        //check if questions are right
        var->correct_all = true;
        var->rerender_points = false;
        for (int i = 0; i < 13; i++) {
            //check if textbox is pressed
            if (textbox_ispressed(objects->textboxes[i])) {
                Mix_PlayChannel(-1, media->sfx_click, 0);
            }
            //check if enter button pressed
            if (button_ispressed(objects->bt_enter[i])) {
                textbox_set_enter(objects->textboxes[i], true);
            }
            if (textbox_get_enter(objects->textboxes[i])) {
                //check if answer is correct
                var->correct[i] = question_compare(objects->questions[i], textbox_get_text(objects->textboxes[i]));

                //if answer is correct, add points to point counter
                var->current_points += question_get_points(objects->questions[i]) * var->correct[i];
                var->rerender_points |= var->correct[i];
                textbox_set_typable(objects->textboxes[i], !var->correct[i]);
                if (var->correct[i]) {
                    Mix_PlayChannel(-1, media->sfx_right, 0);
                }
                else {
                    Mix_PlayChannel(-1, media->sfx_wrong, 0);
                }
            }
            if (button_ispressed(objects->bt_complete[i]) && !var->correct[i]) {
                var->correct[i] = true;
                textbox_set_typable(objects->textboxes[i], !var->correct[i]);
                textbox_set_text(objects->textboxes[i], question_get_a_text(objects->questions[i]));
                var->autocomplete = true;
                Mix_PlayChannel(-1, media->sfx_auto, 0);
            }

            var->correct_all &= var->correct[i];
        }
        if (var->correct_all) {
            var->next = true;
        }
        if (var->rerender_points) {
            sprintf(var->str_points, "%06d", var->old_points + var->current_points);
            texture_load_from_text(media->tex_points, game_get_renderer(game), media->font, var->str_points, COLOR_TEXT_DEFAULT_LIGHT);
        }

        //calculate and render time
        var->remaining_time = TICKS_TOTAL - (int)timer_get_ticks(objects->timer);
        sprintf(var->str_time, "%02d:%02d:%02d", var->remaining_time/60000, (var->remaining_time % 60000) / 1000, (var->remaining_time % 1000) / 10);
        texture_load_from_text(media->tex_time, game_get_renderer(game), media->font, var->str_time, COLOR_TEXT_DEFAULT_LIGHT);
        
        if (var->remaining_time < 0) {
            var->next = true;
        }
    
        game_clear_screen(game);
        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        texture_render(media->tex_text, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_text)) / 2, 32, NULL);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        texture_render(media->tex_time, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_time)) / 2, 8, NULL);

        for (int i = 0; i < 13; i++) {
            texture_render(media->tex_questions[i], game_get_renderer(game), var->question_x[i], var->question_y[i], NULL);
            textbox_render(objects->textboxes[i], game_get_renderer(game), media->font);
            button_render(objects->bt_enter[i], game_get_renderer(game));
            button_render(objects->bt_complete[i], game_get_renderer(game));
        }

        game_render(game, var->transition);

        var->transition += 16 * (var->transition < 255);
    }
    
    timer_stop(objects->timer);
}

void screens_quiz3_results(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    int timer = 120;

    if (var->correct_all) {
        texture_load_from_text(media->tex_text, game_get_renderer(game), media->font, "PARABÉNS!", COLOR_LGREEN);
        Mix_PlayChannel(-1, media->sfx_win, 0);
    }
    else {
        texture_load_from_text(media->tex_text, game_get_renderer(game), media->font, "QUE PENA!", COLOR_LRED);
        Mix_PlayChannel(-1, media->sfx_lose, 0);
    }
    while (!var->next || var->transition > 0) {
        if (timer < 0) {
            if (var->remaining_time > 0 && !var->autocomplete) {
                if (!Mix_Playing(1)) Mix_PlayChannel(1, media->sfx_tally, -1);
                var->remaining_time -= 10000;
                var->remaining_time *= (var->remaining_time > 0);
                var->current_points += 100;
            }
            else {
                Mix_HaltChannel(1);
                var->next = true;
                var->transition -= 16;
                var->transition *= (var->transition > 0);
            }
        }
        
        //if next
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                game_save(game, false);
                var->next = true;
                var->ret = SCREEN_QUIT;
                var->transition = 0;
            }
        }

        //render
        sprintf(var->str_points, "%06d", var->old_points + var->current_points);
        texture_load_from_text(media->tex_points, game_get_renderer(game), media->font, var->str_points, COLOR_TEXT_DEFAULT_LIGHT);
        
        sprintf(var->str_time, "%02d:%02d:%02d", var->remaining_time/60000, (var->remaining_time % 60000) / 1000, (var->remaining_time % 1000) / 10);
        texture_load_from_text(media->tex_time, game_get_renderer(game), media->font, var->str_time, COLOR_TEXT_DEFAULT_LIGHT);
        
        game_clear_screen(game);
        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        texture_render(media->tex_text, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_text)) / 2, 32, NULL);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        texture_render(media->tex_time, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_time)) / 2, 8, NULL);

        for (int i = 0; i < 13; i++) {
            texture_render(media->tex_questions[i], game_get_renderer(game), var->question_x[i], var->question_y[i], NULL);
            textbox_render(objects->textboxes[i], game_get_renderer(game), media->font);
            button_render(objects->bt_enter[i], game_get_renderer(game));
            button_render(objects->bt_complete[i], game_get_renderer(game));
        }

        game_render(game, var->transition);
        timer--;
    }
}

Screen screens_quiz3(Game *game) {
    struct media_t *media = (struct media_t*)malloc(sizeof(struct media_t));
    struct objects_t *objects = (struct objects_t*)malloc(sizeof(struct objects_t));
    struct variables_t *var = (struct variables_t*)malloc(sizeof(struct variables_t));
    if (media == NULL || objects == NULL || var == NULL) {
        printf("Couldnt open structs\n");
        return SCREEN_ERROR;
    }
    screens_quiz3_loadvariables(game, var);
    if (!screens_quiz3_loadmedia(game, media, var)) {
        printf("Couldnt open textures\n");
        return SCREEN_ERROR;
    }
    if (!screens_quiz3_loadobjects(game, objects, media, var)) {
        printf("Couldnt open objects\n");
        return SCREEN_ERROR;
    }
    
    Mix_PlayMusic(media->music, -1);

    screens_quiz3_quiz(game, media, objects, var);

    Mix_HaltMusic();

    var->remaining_time *= (var->remaining_time > 0);

    //points tally
    var->next = false;
    if (var->ret == SCREEN_NEXT) {
        screens_quiz3_results(game, media, objects, var);
    }
    if (var->correct_all) {
        game_set_points(game, POINTS_3, var->current_points);
    }
    if (var->ret == SCREEN_NEXT) var->ret = SCREEN_MENU;

    return screens_quiz3_close(media, objects, var);
}

//QUESTIONS {{{1

Question *quiz3_question_1_7_create(bool inverted) {
    int a = rand() % 8 + 2;
    int b = 0;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];
    char text3[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(x+%d)(x-%d)", a, a);
    sprintf(text2, "(x-%d)(x+%d)", a, a);
    sprintf(text3, "x²-%d", a*a);

    if (inverted) {
        return question_create(a, b, c, text3, text1, text2, false);
    }
    return question_create(a, b, c, text1, text3, text3, false);
}

Question *quiz3_question_2_8_create(bool inverted) {
    int a = rand() % 8 + 2;
    int b = 0;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];
    char text3[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(%d+x)(%d-x)", a, a);
    sprintf(text2, "(%d-x)(%d+x)", a, a);
    sprintf(text3, "%d-x²", a*a);

    if (inverted) {
        return question_create(a, b, c, text3, text1, text2, false);
    }
    return question_create(a, b, c, text1, text3, text3, false);
}

Question *quiz3_question_3_9_create(bool inverted) {
    int a = rand() % 8 + 2;
    int b = 0;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];
    char text3[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(y+%d)(y-%d)", a, a);
    sprintf(text2, "(y-%d)(y+%d)", a, a);
    sprintf(text3, "y²-%d", a*a);

    if (inverted) {
        return question_create(a, b, c, text3, text1, text2, false);
    }
    return question_create(a, b, c, text1, text3, text3, false);
}

Question *quiz3_question_4_10_create(bool inverted) {
    int a = (rand() % 4 + 1) * 2 + 1;
    int b = 0;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];
    char text3[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(x/2+%d/2)(x/2-%d/2)", a, a);
    sprintf(text2, "(x/2-%d/2)(x/2+%d/2)", a, a);
    sprintf(text3, "x²/4-%d/4", a*a);

    if (inverted) {
        return question_create(a, b, c, text3, text1, text2, false);
    }
    return question_create(a, b, c, text1, text3, text3, false);
}

Question *quiz3_question_5_11_create(bool inverted) {
    int a = rand() % 3 + 2;
    int b = rand() % 8 + 2;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];
    char text3[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(%dx+%d)(%dx-%d)", a, b, a, b);
    sprintf(text2, "(%dx-%d)(%dx+%d)", a, b, a, b);
    sprintf(text3, "%dx²-%d", a*a, b*b);

    if (inverted) {
        return question_create(a, b, c, text3, text1, text2, false);
    }
    return question_create(a, b, c, text1, text3, text3, false);
}

Question *quiz3_question_6_12_create(bool inverted) {
    int a = rand() % 8 + 2;
    int b = rand() % 8 + 2;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];
    char text3[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(%dx+%dy)(%dx-%dy)", a, b, a, b);
    sprintf(text2, "(%dx-%dy)(%dx+%dy)", a, b, a, b);
    sprintf(text3, "%dx²-%dy²", a*a, b*b);

    if (inverted) {
        return question_create(a, b, c, text3, text1, text2, false);
    }
    return question_create(a, b, c, text1, text3, text3, false);
}

Question *quiz3_question_13_create() {
    int a = rand() % 4 + 2;
    int b = 0;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];
    char text3[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(x²+%d)(x+%d)(x-%d)", a*a, a, a);
    sprintf(text2, "(x²+%d)(x-%d)(x+%d)", a*a, a, a);
    sprintf(text3, "x⁴-%d", a*a*a*a);

    return question_create(a, b, c, text3, text1, text2, true);
}

Question *quiz3_question_n_create(int n) {
    switch(n) {
        case 0:
        return quiz3_question_1_7_create(false);
        case 1:
        return quiz3_question_2_8_create(false);
        case 2:
        return quiz3_question_3_9_create(false);
        case 3:
        return quiz3_question_4_10_create(false);
        case 4:
        return quiz3_question_5_11_create(false);
        case 5:
        return quiz3_question_6_12_create(false);
        case 6:
        return quiz3_question_1_7_create(true);
        case 7:
        return quiz3_question_2_8_create(true);
        case 8:
        return quiz3_question_3_9_create(true);
        case 9:
        return quiz3_question_4_10_create(true);
        case 10:
        return quiz3_question_5_11_create(true);
        case 11:
        return quiz3_question_6_12_create(true);
        case 12:
        return quiz3_question_13_create();
        default:
        return NULL;
    }
}

