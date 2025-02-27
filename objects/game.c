#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "game.h"

struct game_t {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *target;
    SDL_Texture *target_prev;

    int scalex;
    int scaley;

    int points[4];
    char name[31];
};

Game *game_init(void) {
    //create Game struct
    Game *self = (Game*)malloc(sizeof(Game));
    assert(self != NULL);

    //initialize SDL
    assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) >= 0);

    //create SDL self
    self->window = SDL_CreateWindow("JOOJ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    assert(self->window != NULL);

    //create renderer
    self->renderer = SDL_CreateRenderer(self->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    assert(self->renderer != NULL);

    //set renderer color
    SDL_SetRenderDrawColor(self->renderer, 255, 255, 255, 255);

    //create target texture
    self->target = SDL_CreateTexture(self->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_W, SCREEN_H);
    assert(self->target != NULL);
    SDL_SetTextureBlendMode(self->target, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(self->target, 255);
    
    self->target_prev = SDL_CreateTexture(self->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_W, SCREEN_H);
    assert(self->target_prev != NULL);
    SDL_SetTextureBlendMode(self->target_prev, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(self->target_prev, 255);
    
    SDL_SetRenderTarget(self->renderer, self->target);
    SDL_RenderClear(self->renderer);
    SDL_SetRenderDrawBlendMode(self->renderer, SDL_BLENDMODE_BLEND);

    //init subsystems
    assert(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG);
    assert(TTF_Init() != -1);
    assert(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) >= 0);

    //SDL_ShowCursor(SDL_DISABLE);

    self->scalex = 1.0;
    self->scaley = 1.0;

    self->points[0] = 0;
    self->points[1] = 0;
    self->points[2] = 0;
    self->points[3] = 0;
    strcpy(self->name, "GUI");

    return self;
}

void game_close(Game *self) {
    //destroy renderer and self
    SDL_DestroyRenderer(self->renderer);
    SDL_DestroyWindow(self->window);

    //quit subsystems
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    
    //SDL_ShowCursor(SDL_ENABLE);
    
    //free self struct
    free(self);
}

void game_render(Game *self, uint8_t alpha) {
    //set target texture alpha
    SDL_SetTextureAlphaMod(self->target, alpha);
    //save screen
    SDL_SetRenderTarget(self->renderer, self->target_prev);
    SDL_RenderCopy(self->renderer, self->target, NULL, NULL);
    //reset render target
    SDL_SetRenderTarget(self->renderer, NULL);
    //render target texture
    SDL_RenderCopy(self->renderer, self->target, NULL, NULL);
    //render to screen
    SDL_RenderPresent(self->renderer);
    //clear screen
    SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, 255);
    SDL_RenderClear(self->renderer);
    //set target texture as render target
    SDL_SetRenderTarget(self->renderer, self->target);
    //clear target texture
    SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, 255);
    SDL_RenderClear(self->renderer);
}

SDL_Renderer *game_get_renderer(Game *self) {
    return self->renderer;
}

int game_get_scalex(Game *self) {
    return self->scalex;
}

int game_get_scaley(Game *self) {
    return self->scaley;
}

void game_set_points(Game *self, Points q, int points) {
    int index = (q == POINTS_1) * 0 + (q == POINTS_2) * 1 + (q == POINTS_3) * 2 + (q == POINTS_4) * 3;
    self->points[index] = points;
}

int game_get_points(Game *self, Points p) {
    int points = 0;
    points += self->points[0] * ((p & POINTS_1) == POINTS_1);
    points += self->points[1] * ((p & POINTS_2) == POINTS_2);
    points += self->points[2] * ((p & POINTS_3) == POINTS_3);
    points += self->points[3] * ((p & POINTS_4) == POINTS_4);
    return points;
}

void game_set_name(Game *self, char *name) {
    memcpy(self->name, name, 31);
}

char *game_get_name(Game *self) {
    return self->name;
}

void game_reset_render_target(Game *self) {
    SDL_SetRenderTarget(self->renderer, self->target);
}
