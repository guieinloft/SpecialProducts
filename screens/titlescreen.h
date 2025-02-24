#ifndef SCREENS_TITLESCREEN_H
#define SCREENS_TITLESCREEN_H

//libc includes
#include <stdbool.h>

//sdl includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//object includes
#include "../objects/game.h"
#include "../objects/texture.h"
#include "../objects/button.h"

//screen include
#include "screens.h"

Screen screens_titlescreen(Game *game);

#endif
