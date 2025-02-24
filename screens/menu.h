#ifndef SCREENS_MENU_H
#define SCREENS_MENU_H

//libc includes
#include <stdbool.h>

//sdl includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//object includes
#include "../objects/game.h"

//screen include
#include "screens.h"

Screen screens_menu(Game *game);

#endif
