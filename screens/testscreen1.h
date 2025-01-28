#ifndef SCREENS_TESTSCREEN1_H
#define SCREENS_TESTSCREEN1_H

//libc includes
#include <stdbool.h>

//sdl includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//object includes
#include "../objects/window.h"
#include "../objects/texture.h"
#include "../objects/button.h"

//screen include
#include "screens.h"

Screen screens_TestScreen1(Window *window);

#endif
