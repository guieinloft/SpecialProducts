#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

//structs
#include "objects/window.h"
#include "objects/texture.h"
#include "objects/button.h"

//screens
#include "screens/titlescreen.h"

int main() {
    //init sdl window
    Window *window = window_init();

    //call title screen
    printf("RETORNO: %d\n", screens_TitleScreen(window));

    //close everything
    window_close(window);
}
