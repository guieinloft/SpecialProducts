#ifndef SCREENS_TITLESCREEN_H
#define SCREENS_TITLESCREEN_H

//libc includes
#include <stdbool.h>

//sdl includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//object includes
#include "../objects/window.h"
#include "../objects/texture.h"
#include "../objects/button.h"

bool screens_TitleScreen_loadtextures(Window *window, Texture *buttonTexture, Texture *textTexture, TTF_Font **font);
void screens_TitleScreen_close(Button *button1, Button *button2, Texture *buttonTexture, Texture *textTexture, TTF_Font *font);
int screens_TitleScreen(Window *window);

#endif
