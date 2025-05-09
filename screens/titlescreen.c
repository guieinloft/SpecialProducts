//object includes
#include "../objects/game.h"
#include "../objects/texture.h"
#include "../objects/button.h"
#include "../objects/textbox.h"
#include "../objects/colors.h"

//screen include
#include "screens.h"

struct media_t {
    Texture *tex_prompt;
    Texture *tex_prompt2;
    Texture *tex_copyright;
    Texture *tex_bg;
    Texture *tex_title;
    Texture *tex_button;

    TTF_Font *font;

    Mix_Music *music;
    Mix_Chunk *sfx_click;
    Mix_Chunk *sfx_right;
    Mix_Chunk *sfx_wrong;
};

struct objects_t {
    Button *bt_enter;
    Textbox *textbox;
};

struct variables_t {
    bool next;
    Screen ret;
    SDL_Event e;
    int flicker;
    double size;
    int transition;
};

bool screens_titlescreen_loadmedia(Game *game, struct media_t *media) {
    media->tex_prompt = texture_create();
    if (media->tex_prompt == NULL) return false;
    media->tex_prompt2 = texture_create();
    if (media->tex_prompt2 == NULL) return false;
    media->tex_copyright = texture_create();
    if (media->tex_copyright == NULL) return false;
    media->tex_bg = texture_create();
    if (media->tex_bg == NULL) return false;
    media->tex_title = texture_create();
    if (media->tex_title == NULL) return false;
    media->tex_button = texture_create();
    if (media->tex_button == NULL) return false;

    media->font = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 8);
    if (media->font == NULL) return false;
    
    if(!texture_load_from_text(media->tex_prompt, game_get_renderer(game), media->font, "PRESSIONE O BOTÃO ESQUERDO DO MOUSE", COLOR_TEXT_DEFAULT)) return false;
    if(!texture_load_from_text(media->tex_prompt2, game_get_renderer(game), media->font, "DIGITE O SEU NOME:", COLOR_TEXT_DEFAULT)) return false;
    if(!texture_load_from_text(media->tex_copyright, game_get_renderer(game), media->font, "Versão 1.0.0", COLOR_TEXT_DEFAULT)) return false;

    if(!texture_load_from_file(media->tex_bg, game_get_renderer(game), "img/BgTexture.png")) return false;
    
    if(!texture_load_from_file(media->tex_title, game_get_renderer(game), "img/TitleTexture.png")) return false;
    
    if(!texture_load_from_file(media->tex_button, game_get_renderer(game), "img/ButtonTexture.png")) return false;

    media->music = Mix_LoadMUS("snd/titleproducts.ogg");
    if (media->music == NULL) return false;
    media->sfx_click = Mix_LoadWAV("snd/sfx_click.wav");
    if (media->sfx_click == NULL) return false;
    media->sfx_right = Mix_LoadWAV("snd/sfx_right.wav");
    if (media->sfx_right == NULL) return false;
    media->sfx_wrong = Mix_LoadWAV("snd/sfx_wrong.wav");
    if (media->sfx_wrong == NULL) return false;

    return true;
}

void screens_titlescreen_loadvariables(struct variables_t *var) {
    var->next = false;
    var->ret = SCREEN_NEXT;
    var->size = 0.0;
    var->flicker = 0;
    var->transition = 15;
}

bool screens_titlescreen_loadobjects(Game *game, struct objects_t *objects, struct media_t *media) {
    objects->textbox = textbox_create(180, 264, media->tex_button);
    if (objects->textbox == NULL) return false;

    objects->bt_enter = button_create(media->tex_button);
    if (objects->bt_enter == NULL) return false;
    if (!button_change_text(objects->bt_enter, game_get_renderer(game), media->font, "!", 1, COLOR_TEXT_DEFAULT, false)) return false;
    button_change_position(objects->bt_enter, 436, 264);
    return true;
}

Screen screens_titlescreen_close(struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    button_free(objects->bt_enter);
    textbox_free(objects->textbox);
    texture_free(media->tex_prompt);
    texture_free(media->tex_copyright);
    texture_free(media->tex_bg);
    texture_free(media->tex_title);
    TTF_CloseFont(media->font);
    Mix_FreeMusic(media->music);
    Mix_FreeChunk(media->sfx_click);
    Mix_FreeChunk(media->sfx_right);
    Mix_FreeChunk(media->sfx_wrong);
    free(media);
    free(objects);
    
    Screen ret = var->ret;
    free(var);
    return ret;
}

void screens_titlescreen_init(Game *game, struct media_t *media, struct variables_t *var) {
    while (!var->next || (var->transition > 0 && var->ret != SCREEN_NEXT)) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                var->next = true;
                var->ret = SCREEN_QUIT;
            }
            else if (var->e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, media->sfx_right, 0);
                var->next = true;
            }
        }
        game_clear_screen(game);

        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, NULL);
        if (var->flicker < 30) {
            texture_render(media->tex_prompt, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_prompt)) / 2, 256, NULL);
        }
        texture_render(media->tex_copyright, game_get_renderer(game), 8, SCREEN_H - 16, NULL);
        texture_render_scaled(media->tex_title, game_get_renderer(game), SCREEN_W / 2 - 128 * sin(var->size), 128 - 128 * sin(var->size), NULL, sin(var->size), sin(var->size), 0.0, NULL);
        game_render(game, var->transition);

        var->size += 0.1;
        if (var->size > 1.6) {
            var->size = M_PI/2;
        }
        var->flicker = (var->flicker + 1) % 40;
        if (var->next && var->ret != SCREEN_NEXT) {
            var->transition -= 16;
            var->transition *= (var->transition > 0);
        }
        else
            var->transition += 16 * (var->transition < 255);
    }
}

void screens_titlescreen_nameinput(Game *game, struct media_t *media, struct objects_t *objects, struct variables_t *var) {
    while (!var->next || var->transition > 0) {
        while (SDL_PollEvent(&var->e) != 0) {
            if (game_handle_event(game, var->e)) {
                var->next = true;
                var->ret = SCREEN_QUIT;
            }
            textbox_handle_event(objects->textbox, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
            button_handle_event(objects->bt_enter, var->e, game_get_scalex(game), game_get_scaley(game), game_get_screenx(game), game_get_screeny(game));
        }
        game_clear_screen(game);

        //check button
        if (button_ispressed(objects->bt_enter)) {
            textbox_set_enter(objects->textbox, true);
        }
        //check textbox
        if (textbox_get_enter(objects->textbox)) {
            if (!textbox_isempty(objects->textbox)) {
                game_set_name(game, textbox_get_text(objects->textbox));
                textbox_set_typable(objects->textbox, false);
                var->next = true;
                var->ret = SCREEN_MENU;
                Mix_PlayChannel(-1, media->sfx_right, 0);
            }
            else {
                Mix_PlayChannel(-1, media->sfx_wrong, 0);
            }
        }
        else if (textbox_ispressed(objects->textbox)) {
            Mix_PlayChannel(-1, media->sfx_click, 0);
        }

        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, NULL);
        texture_render(media->tex_prompt2, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_prompt2)) / 2, 256, NULL);
        texture_render(media->tex_copyright, game_get_renderer(game), 8, SCREEN_H - 16, NULL);
        textbox_render(objects->textbox, game_get_renderer(game), media->font);
        button_render(objects->bt_enter, game_get_renderer(game));
        //texture_render_scaled(media->tex_title, game_get_renderer(game), SCREEN_W / 2 - 128 * var->size, 128 - 128 * var->size, NULL, var->size, var->size, 0.0, NULL);
        texture_render_scaled(media->tex_title, game_get_renderer(game), SCREEN_W / 2 - 128 / sin(var->size), 128 - 128 / sin(var->size), NULL, 1 / sin(var->size), 1 / sin(var->size), 0.0, NULL);
        game_render(game, var->transition);
        if (var->next) {
            var->transition -= 16;
            var->transition *= (var->transition > 0);
            var->size += 0.1;
            if (var->size > 3.1) {
                var->size = M_PI;
            }
        }
    }
}

Screen screens_titlescreen(Game *game) {
    struct media_t *media = (struct media_t*)malloc(sizeof(struct media_t));
    struct variables_t *var = (struct variables_t*)malloc(sizeof(struct variables_t));
    struct objects_t *objects = (struct objects_t*)malloc(sizeof(struct objects_t));
    if (media == NULL || var == NULL || objects == NULL) {
        printf("Couldnt open structs\n");
        return SCREEN_ERROR;
    }
    screens_titlescreen_loadvariables(var);
    if (!screens_titlescreen_loadmedia(game, media)) {
        printf("Couldnt open textures\n");
        return SCREEN_ERROR;
    }
    if (!screens_titlescreen_loadobjects(game, objects, media)) {
        printf("Couldnt open textures\n");
        return SCREEN_ERROR;
    }

    Mix_PlayMusic(media->music, -1);

    screens_titlescreen_init(game, media, var);
    var->next = false;
    if (var->ret == SCREEN_NEXT) {
        screens_titlescreen_nameinput(game, media, objects, var);
    }
    
    Mix_HaltMusic();

    return screens_titlescreen_close(media, objects, var);
}
