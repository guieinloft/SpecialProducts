#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <assert.h>

#include "window.h"

struct window_t {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *target;
};

Window *window_init(void) {
    //create Window struct
    Window *self = (Window*)malloc(sizeof(Window));
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
    SDL_SetRenderTarget(self->renderer, self->target);
    SDL_RenderClear(self->renderer);

    //init subsystems
    assert(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG);
    assert(TTF_Init() != -1);
    assert(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) >= 0);

    return self;
}

void window_close(Window *self) {
    //destroy renderer and self
    SDL_DestroyRenderer(self->renderer);
    SDL_DestroyWindow(self->window);

    //quit subsystems
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    
    //free self struct
    free(self);
}

void window_render(Window *self) {
    //reset render target
    SDL_SetRenderTarget(self->renderer, NULL);
    //render target texture
    SDL_RenderCopy(self->renderer, self->target, NULL, NULL);
    //render to screen
    SDL_RenderPresent(self->renderer);
    //set target texture as render target
    SDL_SetRenderTarget(self->renderer, self->target);
    //clear screen
    SDL_SetRenderDrawColor(self->renderer, 255, 255, 255, 255);
    SDL_RenderClear(self->renderer);
}

SDL_Renderer *window_get_renderer(Window *self) {
    return self->renderer;
}
