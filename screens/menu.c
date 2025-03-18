//object includes
#include "../objects/game.h"
#include "../objects/texture.h"
#include "../objects/button.h"
#include "../objects/textbox.h"
#include "../objects/colors.h"

//screen include
#include "screens.h"

#define NUM_CREDITS 3

struct media_t {
    Texture *tex_button;
    Texture *tex_screen;
    Texture *tex_bg;
    Texture *tex_name;
    Texture *tex_points;
    Texture *tex_bilinear;
    Texture *tex_credits[NUM_CREDITS][2];
    Texture *tex_ranking[6];
    SDL_Rect clip_bg;

    TTF_Font *font;

    Mix_Music *music;
    Mix_Chunk *sfx_click;
};

struct objects_t {
    Button *button1;
    Button *button2;
    Button *button3;
    Button *button4;
    Button *button5;
    
    Button *bt_options;
    Button *bt_credits;
    Button *bt_ranking;
};

struct variables_t {
    bool next;
    Screen ret;
    SDL_Event e;
    int transition;
    int points;
    int position;
};

bool screens_menu_loadmedia(Game *game, struct media_t *media, struct variables_t *var) {
    media->tex_button = texture_create();
    if (media->tex_button == NULL) return false;
    media->tex_screen = texture_create();
    if (media->tex_screen == NULL) return false;
    media->tex_bg = texture_create();
    if (media->tex_bg == NULL) return false;
    media->tex_name = texture_create();
    if (media->tex_name == NULL) return false;
    media->tex_points = texture_create();
    if (media->tex_points == NULL) return false;
    media->tex_bilinear = texture_create();
    if (media->tex_bilinear == NULL) return false;
    for (int i = 0; i < NUM_CREDITS; i++) {
        for (int j = 0; j < 2; j++) {
            media->tex_credits[i][j] = texture_create();
            if (media->tex_credits[i][j] == NULL) return false;
        }
    }
    for (int i = 0; i < 6; i++) {
        media->tex_ranking[i] = texture_create();
        if (media->tex_ranking[i] == NULL) return false;
    }

    if(!texture_load_from_file(media->tex_button, game_get_renderer(game), "img/ButtonTexture.png")) return false;
    
    media->font = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 8);
    if (media->font == NULL) return false;
    
    if(!texture_load_from_text(media->tex_screen, game_get_renderer(game), media->font, "SELECIONE O PRODUTO NOTÁVEL:", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    
    if(!texture_load_from_text(media->tex_bilinear, game_get_renderer(game), media->font, "OBS: REQUER REINICIALIZAÇÃO PARA FAZER EFEITO", COLOR_TEXT_DEFAULT_LIGHT)) return false;

    if(!texture_load_from_file(media->tex_bg, game_get_renderer(game), "img/BgTexture.png")) return false;
    media->clip_bg.x = 0;
    media->clip_bg.y = 360;
    media->clip_bg.w = SCREEN_W;
    media->clip_bg.h = SCREEN_H;
    
    media->music = Mix_LoadMUS("snd/menuproducts.wav");
    if (media->music == NULL) return false;
    media->sfx_click = Mix_LoadWAV("snd/sfx_click.wav");
    if (media->sfx_click == NULL) return false;

    //NAME AND POINTS
    if(!texture_load_from_text(media->tex_name, game_get_renderer(game), media->font, game_get_name(game), COLOR_TEXT_DEFAULT_LIGHT)) return false;

    char str_points[7];
    sprintf(str_points, "%06d", var->points);
    if(!texture_load_from_text(media->tex_points, game_get_renderer(game), media->font, str_points, COLOR_TEXT_DEFAULT_LIGHT)) return false;

    //CREDITS
    if (!texture_load_from_text(media->tex_credits[0][0], game_get_renderer(game), media->font, "SUZIANE BOPP ANTONELLO", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if (!texture_load_from_text(media->tex_credits[0][1], game_get_renderer(game), media->font, "Professora orientadora", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if (!texture_load_from_text(media->tex_credits[1][0], game_get_renderer(game), media->font, "GUILHERME MENEGHETTI EINLOFT", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if (!texture_load_from_text(media->tex_credits[1][1], game_get_renderer(game), media->font, "Programação, músicas, artes", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if (!texture_load_from_text(media->tex_credits[2][0], game_get_renderer(game), media->font, "CAMILLA LEAL SEVERO", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if (!texture_load_from_text(media->tex_credits[2][1], game_get_renderer(game), media->font, "Textos, artes", COLOR_TEXT_DEFAULT_LIGHT)) return false;

    //RANKING
    char str_ranking[TEXTBOX_TEXT_SIZE + 13];
    sprintf(str_ranking, "%02d: %-31s %06d", var->position, game_get_name(game), var->points);
    if (!texture_load_from_text(media->tex_ranking[0], game_get_renderer(game), media->font, str_ranking, COLOR_TEXT_DEFAULT_LIGHT)) return false;
    for (int i = 1; i < 6; i++) {
        sprintf(str_ranking, "%02d: %-31s %06d", i, game_get_rank_name(game, i), game_get_rank_points(game, i));
        if (i == var->position) {
            if (!texture_load_from_text(media->tex_ranking[i], game_get_renderer(game), media->font, str_ranking, COLOR_TEXT_DEFAULT_LIGHT)) return false;
        }
        else if (game_get_rank_points(game, i) != 0) {
            if (!texture_load_from_text(media->tex_ranking[i], game_get_renderer(game), media->font, str_ranking, COLOR_TEXT_WEAK_LIGHT)) return false;
        }
    }

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
    objects->button5 = button_create(media->tex_button);
    if (objects->button5 == NULL) return false;
    
    if (!button_change_text(objects->button1, game_get_renderer(game), media->font, "1. QUADRADO DA SOMA", 30, COLOR_TEXT_DEFAULT, false)) return false;
    if (!button_change_text(objects->button2, game_get_renderer(game), media->font, "2. QUADRADO DA DIFERENÇA", 30, COLOR_TEXT_DEFAULT, false)) return false;
    if (!button_change_text(objects->button3, game_get_renderer(game), media->font, "3. P. DA SOMA PELA DIFERENÇA", 30, COLOR_TEXT_DEFAULT, false)) return false;
    if (!button_change_text(objects->button4, game_get_renderer(game), media->font, "4. EXPRESSÕES DE 2º GRAU", 30, COLOR_TEXT_DEFAULT, false)) return false;
    
    button_change_position(objects->button1, (SCREEN_W - button_get_real_w(objects->button1)) / 2, 128);
    button_change_position(objects->button2, (SCREEN_W - button_get_real_w(objects->button2)) / 2, 160);
    button_change_position(objects->button3, (SCREEN_W - button_get_real_w(objects->button3)) / 2, 192);
    button_change_position(objects->button4, (SCREEN_W - button_get_real_w(objects->button4)) / 2, 224);
    button_change_position(objects->button5, (SCREEN_W - button_get_real_w(objects->button4)) / 2, 264);

    objects->bt_options = button_create(media->tex_button);
    if (objects->bt_options == NULL) return false;
    objects->bt_credits = button_create(media->tex_button);
    if (objects->bt_credits == NULL) return false;
    objects->bt_ranking = button_create(media->tex_button);
    if (objects->bt_ranking == NULL) return false;
    
    if (!button_change_text(objects->bt_options, game_get_renderer(game), media->font, "OPÇÕES", 24, COLOR_TEXT_DEFAULT, true)) return false;
    if (!button_change_text(objects->bt_credits, game_get_renderer(game), media->font, "CRÉDITOS", 24, COLOR_TEXT_DEFAULT, true)) return false;
    if (!button_change_text(objects->bt_ranking, game_get_renderer(game), media->font, "PLACAR", 24, COLOR_TEXT_DEFAULT, true)) return false;
    button_change_position(objects->bt_options, 8, SCREEN_H - 32);
    button_change_position(objects->bt_credits, 8 + button_get_real_w(objects->bt_options), SCREEN_H - 32);
    button_change_position(objects->bt_ranking, 8 + button_get_real_w(objects->bt_options) + button_get_real_w(objects->bt_credits), SCREEN_H - 32);

    return true;
}

void screens_menu_loadvariables(Game *game, struct variables_t *var) {
    var->next = false;
    var->ret = SCREEN_NEXT;
    var->transition = 15;
    var->points = game_get_points(game, POINTS_ALL);
    game_calc_position(game);
    var->position = game_get_position(game);
}

Screen screens_menu_close(struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    button_free(objects->button1);
    button_free(objects->button2);
    button_free(objects->button3);
    button_free(objects->button4);
    button_free(objects->button5);
    button_free(objects->bt_options);
    button_free(objects->bt_credits);
    button_free(objects->bt_ranking);

    texture_free(media->tex_button);
    texture_free(media->tex_screen);
    texture_free(media->tex_bg);
    texture_free(media->tex_name);
    texture_free(media->tex_points);
    texture_free(media->tex_bilinear);
    for (int i = 0; i < NUM_CREDITS; i++) {
        for (int j = 0; j < 2; j++) {
            texture_free(media->tex_credits[i][j]);
        }
    }
    for (int i = 0; i < 6; i++) {
        texture_free(media->tex_ranking[i]);
    }

    TTF_CloseFont(media->font);
    Mix_FreeMusic(media->music);
    Mix_FreeChunk(media->sfx_click);
    free(media);
    free(objects);
    Screen ret = var->ret;
    free(var);
    return ret;
}

bool screens_menu_options_init(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    //mudar botoes
    if (!game_get_option(game, OPTION_IGNORE_RATIO)) {
        if (!button_change_text(objects->button1, game_get_renderer(game), media->font, "IGNORAR PROPORÇÃO DA TELA", 30, COLOR_TEXT_DEFAULT, true)) return false;
    }
    else {
        if (!button_change_text(objects->button1, game_get_renderer(game), media->font, "MANTER PROPORÇÃO DA TELA", 30, COLOR_TEXT_DEFAULT, true)) return false;
    }

    if (!game_get_option(game, OPTION_INTEGER_SCALING)) {
        if (!button_change_text(objects->button2, game_get_renderer(game), media->font, "REDIMENSIONAMENTO INTEGRAL", 30, COLOR_TEXT_DEFAULT, true)) return false;
    }
    else {
        if (!button_change_text(objects->button2, game_get_renderer(game), media->font, "REDIMENSIONAMENTO REGULAR", 30, COLOR_TEXT_DEFAULT, true)) return false;
    }

    if (!game_get_option(game, OPTION_MUTE_MUSIC)) {
        if (!button_change_text(objects->button3, game_get_renderer(game), media->font, "SILENCIAR MÚSICAS", 30, COLOR_TEXT_DEFAULT, true)) return false;
    }
    else {
        if (!button_change_text(objects->button3, game_get_renderer(game), media->font, "TOCAR MÚSICAS", 30, COLOR_TEXT_DEFAULT, true)) return false;
    }

    if (!game_get_option(game, OPTION_MUTE_SOUND)) {
        if (!button_change_text(objects->button4, game_get_renderer(game), media->font, "SILENCIAR EFEITOS SONOROS", 30, COLOR_TEXT_DEFAULT, true)) return false;
    }
    else {
        if (!button_change_text(objects->button4, game_get_renderer(game), media->font, "TOCAR EFEITOS SONOROS", 30, COLOR_TEXT_DEFAULT, true)) return false;
    }
    
    if (!game_get_option(game, OPTION_BILINEAR_FILTERING)) {
            if (!button_change_text(objects->button5, game_get_renderer(game), media->font, "FILTRAGEM BILINEAR", 30, COLOR_TEXT_DEFAULT, true)) return false;
    }
    else {
        if (!button_change_text(objects->button5, game_get_renderer(game), media->font, "SEM FILTRAGEM", 30, COLOR_TEXT_DEFAULT, true)) return false;
    }
    
    if (!button_change_text(objects->bt_options, game_get_renderer(game), media->font, "VOLTAR", 24, COLOR_TEXT_DEFAULT, true)) return false;

    if (!texture_load_from_text(media->tex_screen, game_get_renderer(game), media->font, "OPÇÕES", COLOR_TEXT_DEFAULT_LIGHT)) return false;

    var->next = false;

    return true;
}

bool screens_menu_options_close(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    if (!button_change_text(objects->button1, game_get_renderer(game), media->font, "1. QUADRADO DA SOMA", 30, COLOR_TEXT_DEFAULT, false)) return false;
    if (!button_change_text(objects->button2, game_get_renderer(game), media->font, "2. QUADRADO DA DIFERENÇA", 30, COLOR_TEXT_DEFAULT, false)) return false;
    if (!button_change_text(objects->button3, game_get_renderer(game), media->font, "3. P. DA SOMA PELA DIFERENÇA", 30, COLOR_TEXT_DEFAULT, false)) return false;
    if (!button_change_text(objects->button4, game_get_renderer(game), media->font, "4. EXPRESSÕES DE 2º GRAU", 30, COLOR_TEXT_DEFAULT, false)) return false;
    if (!button_change_text(objects->bt_options, game_get_renderer(game), media->font, "OPÇÕES", 24, COLOR_TEXT_DEFAULT, true)) return false;
    if(!texture_load_from_text(media->tex_screen, game_get_renderer(game), media->font, "SELECIONE O PRODUTO NOTÁVEL:", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if (var->ret == SCREEN_NEXT) var->next = false;
    return true;
}

bool screens_menu_options(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    if (!screens_menu_options_init(game, media, objects, var)) return false;
    while (!var->next) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                var->next = true;
                var->ret = SCREEN_QUIT;
                var->transition = 0;
                game_save(game, true);
            }
            button_handle_event(objects->button1, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
            button_handle_event(objects->button2, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
            button_handle_event(objects->button3, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
            button_handle_event(objects->button4, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
            button_handle_event(objects->button5, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
            
            button_handle_event(objects->bt_options, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
        }
        game_clear_screen(game);

        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        button_render(objects->button1, game_get_renderer(game));
        button_render(objects->button2, game_get_renderer(game));
        button_render(objects->button3, game_get_renderer(game));
        button_render(objects->button4, game_get_renderer(game));
        button_render(objects->button5, game_get_renderer(game));
        texture_render(media->tex_bilinear, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_bilinear)) / 2, 256, NULL);
        
        button_render(objects->bt_options, game_get_renderer(game));
        texture_render(media->tex_screen, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_screen)) / 2, 64, NULL);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);

        game_render(game, var->transition);

        if (button_ispressed(objects->button1)) {
            if (game_change_option(game, OPTION_IGNORE_RATIO)) {
                if (!button_change_text(objects->button1, game_get_renderer(game), media->font, "MANTER PROPORÇÃO DA TELA", 30, COLOR_TEXT_DEFAULT, true)) return false;
            }
            else {
                if (!button_change_text(objects->button1, game_get_renderer(game), media->font, "IGNORAR PROPORÇÃO DA TELA", 30, COLOR_TEXT_DEFAULT, true)) return false;
            }
            Mix_PlayChannel(-1, media->sfx_click, 0);
        }
        if (button_ispressed(objects->button2)) {
            if (game_change_option(game, OPTION_INTEGER_SCALING)) {
                if (!button_change_text(objects->button2, game_get_renderer(game), media->font, "REDIMENSIONAMENTO REGULAR", 30, COLOR_TEXT_DEFAULT, true)) return false;
            }
            else {
                if (!button_change_text(objects->button2, game_get_renderer(game), media->font, "REDIMENSIONAMENTO INTEGRAL", 30, COLOR_TEXT_DEFAULT, true)) return false;
            }
            Mix_PlayChannel(-1, media->sfx_click, 0);
        }
        if (button_ispressed(objects->button3)) {
            if (game_change_option(game, OPTION_MUTE_MUSIC)) {
                if (!button_change_text(objects->button3, game_get_renderer(game), media->font, "TOCAR MÚSICAS", 30, COLOR_TEXT_DEFAULT, true)) return false;
                Mix_VolumeMusic(0);
            }
            else {
                if (!button_change_text(objects->button3, game_get_renderer(game), media->font, "SILENCIAR MÚSICAS", 30, COLOR_TEXT_DEFAULT, true)) return false;
                Mix_VolumeMusic(MIX_MAX_VOLUME);
            }
            Mix_PlayChannel(-1, media->sfx_click, 0);
        }
        if (button_ispressed(objects->button4)) {
            if (game_change_option(game, OPTION_MUTE_SOUND)) {
                if (!button_change_text(objects->button4, game_get_renderer(game), media->font, "TOCAR EFEITOS SONOROS", 30, COLOR_TEXT_DEFAULT, true)) return false;
                Mix_Volume(-1, 0);
            }
            else {
                if (!button_change_text(objects->button4, game_get_renderer(game), media->font, "SILENCIAR EFEITOS SONOROS", 30, COLOR_TEXT_DEFAULT, true)) return false;
                Mix_Volume(-1, MIX_MAX_VOLUME);
            }
            Mix_PlayChannel(-1, media->sfx_click, 0);
        }
        if (button_ispressed(objects->button5)) {
            if (game_change_option(game, OPTION_BILINEAR_FILTERING)) {
                if (!button_change_text(objects->button5, game_get_renderer(game), media->font, "SEM FILTRAGEM", 30, COLOR_TEXT_DEFAULT, true)) return false;
            }
            else {
                if (!button_change_text(objects->button5, game_get_renderer(game), media->font, "FILTRAGEM BILINEAR", 30, COLOR_TEXT_DEFAULT, true)) return false;
            }
            Mix_PlayChannel(-1, media->sfx_click, 0);
        }
        
        if (button_ispressed(objects->bt_options)) {
            var->next = true;
            Mix_PlayChannel(-1, media->sfx_click, 0);
        }

    }
    return screens_menu_options_close(game, media, objects, var);
}

bool screens_menu_credits(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    if (!button_change_text(objects->bt_credits, game_get_renderer(game), media->font, "VOLTAR", 24, COLOR_TEXT_DEFAULT, true)) return false;
    if (!texture_load_from_text(media->tex_screen, game_get_renderer(game), media->font, "CRÉDITOS", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    var->next = false;
    while (!var->next) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                var->next = true;
                var->ret = SCREEN_QUIT;
                var->transition = 0;
                game_save(game, true);
            }
            button_handle_event(objects->bt_credits, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
        }

        game_clear_screen(game);
        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        texture_render(media->tex_screen, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_screen)) / 2, 64, NULL);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        
        for (int i = 0; i < NUM_CREDITS; i++) {
            for (int j = 0; j < 2; j++) {
                texture_render(media->tex_credits[i][j], game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_credits[i][j])) / 2, 128 + 48 * i + 16 * j, NULL);
            }
        }
        button_render(objects->bt_credits, game_get_renderer(game));
        
        game_render(game, var->transition);
        
        if (button_ispressed(objects->bt_credits)) {
            var->next = true;
            Mix_PlayChannel(-1, media->sfx_click, 0);
        }
    }
    if (!button_change_text(objects->bt_credits, game_get_renderer(game), media->font, "CRÉDITOS", 24, COLOR_TEXT_DEFAULT, true)) return false;
    if(!texture_load_from_text(media->tex_screen, game_get_renderer(game), media->font, "SELECIONE O PRODUTO NOTÁVEL:", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if (var->ret == SCREEN_NEXT) var->next = false;
    return true;
}

bool screens_menu_ranking(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    if (!button_change_text(objects->bt_ranking, game_get_renderer(game), media->font, "VOLTAR", 24, COLOR_TEXT_DEFAULT, true)) return false;
    if (!texture_load_from_text(media->tex_screen, game_get_renderer(game), media->font, "PLACAR", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    var->next = false;
    while (!var->next) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                var->next = true;
                var->ret = SCREEN_QUIT;
                var->transition = 0;
                game_save(game, true);
            }
            button_handle_event(objects->bt_ranking, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
        }

        game_clear_screen(game);
        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        texture_render(media->tex_screen, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_screen)) / 2, 64, NULL);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        
        for (int i = 1; i < 6; i++) {
            texture_render(media->tex_ranking[i], game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_ranking[i])) / 2, 112 + 16 * i, NULL);
        }
        texture_render(media->tex_ranking[0], game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_ranking[0])) / 2, 248, NULL);
        button_render(objects->bt_ranking, game_get_renderer(game));
        
        game_render(game, var->transition);
        
        if (button_ispressed(objects->bt_ranking)) {
            var->next = true;
            Mix_PlayChannel(-1, media->sfx_click, 0);
        }
    }
    if (!button_change_text(objects->bt_ranking, game_get_renderer(game), media->font, "PLACAR", 24, COLOR_TEXT_DEFAULT, true)) return false;
    if(!texture_load_from_text(media->tex_screen, game_get_renderer(game), media->font, "SELECIONE O PRODUTO NOTÁVEL:", COLOR_TEXT_DEFAULT_LIGHT)) return false;
    if (var->ret == SCREEN_NEXT) var->next = false;
    return true;
}

void screens_menu_menu(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    while (!var->next || var->transition > 0) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                var->next = true;
                var->ret = SCREEN_QUIT;
                var->transition = 0;
                game_save(game, true);
            }
            button_handle_event(objects->button1, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
            button_handle_event(objects->button2, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
            button_handle_event(objects->button3, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
            button_handle_event(objects->button4, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
            
            button_handle_event(objects->bt_options, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
            button_handle_event(objects->bt_credits, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
            button_handle_event(objects->bt_ranking, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
        }

        game_clear_screen(game);
        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        button_render(objects->button1, game_get_renderer(game));
        button_render(objects->button2, game_get_renderer(game));
        button_render(objects->button3, game_get_renderer(game));
        button_render(objects->button4, game_get_renderer(game));
        
        button_render(objects->bt_options, game_get_renderer(game));
        button_render(objects->bt_credits, game_get_renderer(game));
        button_render(objects->bt_ranking, game_get_renderer(game));
        texture_render(media->tex_screen, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_screen)) / 2, 64, NULL);
        texture_render(media->tex_name, game_get_renderer(game), 8, 8, NULL);
        texture_render(media->tex_points, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_points) - 8), 8, NULL);
        game_render(game, var->transition);

        if (button_ispressed(objects->button1)) {
            var->ret = SCREEN_DEMO1;
            var->next = true;
            Mix_PlayChannel(-1, media->sfx_click, 0);
        }
        if (button_ispressed(objects->button2)) {
            var->ret = SCREEN_DEMO2;
            var->next = true;
            Mix_PlayChannel(-1, media->sfx_click, 0);
        }
        if (button_ispressed(objects->button3)) {
            var->ret = SCREEN_DEMO3;
            var->next = true;
            Mix_PlayChannel(-1, media->sfx_click, 0);
        }
        if (button_ispressed(objects->button4)) {
            var->ret = SCREEN_DEMO4;
            var->next = true;
            Mix_PlayChannel(-1, media->sfx_click, 0);
        }

        if (button_ispressed(objects->bt_options)) {
            Mix_PlayChannel(-1, media->sfx_click, 0);
            if (!screens_menu_options(game, media, objects, var)) {
                var->next = true;
                var->ret = SCREEN_ERROR;
                var->transition = 0;
            }
        }
        if (button_ispressed(objects->bt_credits)) {
            Mix_PlayChannel(-1, media->sfx_click, 0);
            if (!screens_menu_credits(game, media, objects, var)) {
                var->next = true;
                var->ret = SCREEN_ERROR;
                var->transition = 0;
            }
        }
        if (button_ispressed(objects->bt_ranking)) {
            if (!screens_menu_ranking(game, media, objects, var)) {
                var->next = true;
                var->ret = SCREEN_ERROR;
                var->transition = 0;
            }
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
