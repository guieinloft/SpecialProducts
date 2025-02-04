#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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
#include "screens/screens.h"
#include "screens/titlescreen.h"
#include "screens/testscreen1.h"

int main() {
    srand(time(0));
    //init sdl window
    Window *window = window_init();

    //call title screen
    Screen current = SCREEN_TITLESCREEN;
    do {
        current = screens_redirect(current, window);
        printf("next screen: %d\n", current);
    } while (current != SCREEN_ERROR && current != SCREEN_QUIT);

    //close everything
    window_close(window);
}
