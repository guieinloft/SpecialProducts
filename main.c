#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

//SDL
#define SDL_DISABLE_IMMINTRIN_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

//structs
#include "objects/game.h"
#include "objects/texture.h"
#include "objects/button.h"

//screens
#include "screens/screens.h"
#include "screens/titlescreen.h"
#include "screens/testscreen1.h"

int main(int argc, char **argv) {
    srand(time(0));
    //init sdl game
    Game *game = game_init();

    //call title screen
    Screen current = SCREEN_TITLESCREEN;
    do {
        current = screens_redirect(current, game);
        printf("next screen: %d\n", current);
    } while (current != SCREEN_ERROR && current != SCREEN_QUIT);

    //close everything
    game_close(game);
}
