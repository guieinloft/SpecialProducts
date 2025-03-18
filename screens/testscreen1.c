//object includes
#include "../objects/game.h"
#include "../objects/texture.h"
#include "../objects/button.h"
#include "../objects/textbox.h"
#include "../objects/balloon.h"
#include "../objects/colors.h"

//screen include
#include "screens.h"

struct media_t {
    Texture *tex_balloon;
    Texture *tex_anim;
    Texture *tex_text;

    Texture *tex_bg;
    SDL_Rect clip_bg;

    TTF_Font *font;

    Mix_Music *music;
};

bool screens_testscreen1_loadmedia(Game *game, struct media_t *media) {
    media->tex_balloon = texture_create();
    if (media->tex_balloon == NULL) return false;
    media->tex_anim = texture_create();
    if (media->tex_anim == NULL) return false;
    media->tex_text = texture_create();
    if (media->tex_text == NULL) return false;
    media->tex_bg = texture_create();
    if (media->tex_bg == NULL) return false;

    if(!texture_load_from_file(media->tex_balloon, game_get_renderer(game), "img/BalloonTexture.png")) return false;
    if(!texture_load_from_file(media->tex_anim, game_get_renderer(game), "img/BalloonAnimTexture.png")) return false;
    
    media->font = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 8);
    if (media->font == NULL) return false;
    if(!texture_load_from_text(media->tex_text, game_get_renderer(game), media->font, "TELA DE TESTE 1", COLOR_TEXT_DEFAULT)) return false;
    
    if(!texture_load_from_file(media->tex_bg, game_get_renderer(game), "img/BgTexture.png")) return false;

    media->clip_bg.x = 0;
    media->clip_bg.y = 360;
    media->clip_bg.w = SCREEN_W;
    media->clip_bg.h = SCREEN_H;

    media->music = Mix_LoadMUS("snd/dong.wav");
    if (media->music == NULL) return false;

    return true;
}

void screens_testscreen1_close(Balloon *balloon, struct media_t *media) {
    balloon_free(balloon);
    texture_free(media->tex_balloon);
    texture_free(media->tex_anim);
    texture_free(media->tex_text);
    texture_free(media->tex_bg);
    TTF_CloseFont(media->font);
    Mix_FreeMusic(media->music);
}

Screen screens_testscreen1(Game *game) {
    struct media_t *media = (struct media_t*)malloc(sizeof(struct media_t));
    if (media == NULL) {
        printf("Couldnt open media struct\n");
        return SCREEN_ERROR;
    }
    if (!screens_testscreen1_loadmedia(game, media)) {
        printf("Couldnt open textures\n");
        return SCREEN_ERROR;
    }
    
    Balloon *balloon = balloon_create(media->tex_balloon, media->tex_anim, "txt/test_text.txt", false);

    if (balloon == NULL) {
        printf("Couldnt make balloon\n");
        return SCREEN_ERROR;
    }

    bool quit = false;
    int ret = SCREEN_MENU;

    SDL_Event e;

    double sum = 0.0;

    Mix_PlayMusic(media->music, -1);

    while (!quit || !balloon_ishidden(balloon)) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            balloon_handle_event(balloon, e);
        }

        if (balloon_read_more(balloon)) {
            if (!balloon_read_text(balloon, game_get_renderer(game), media->font)) {
                quit = true;
            }
        }

        balloon_calc_position(balloon);
        
        texture_render(media->tex_bg, game_get_renderer(game), 0, 0, &media->clip_bg);
        balloon_render(balloon, game_get_renderer(game));
        texture_render_scaled(media->tex_text, game_get_renderer(game), (SCREEN_W - texture_getw(media->tex_text) * 2 * fabs(sin(sum))) / 2, 64, NULL, 2 * sin(sum), 2, 0, NULL);
        game_render(game, 255);

        sum += 0.05;
    }
    
    Mix_HaltMusic();
    screens_testscreen1_close(balloon, media);
    return ret;
}
