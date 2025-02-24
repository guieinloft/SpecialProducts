#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

#define SCREEN_W 640
#define SCREEN_H 360

typedef enum points_e {
    POINTS_1 = 0x00000001,
    POINTS_2 = 0x00000002,
    POINTS_3 = 0x00000004,
    POINTS_4 = 0x00000008,
    POINTS_ALL = 0x00000015,
    POINTS_NONE = 0x00000000
} Points;

typedef struct game_t Game;

Game *game_init(void);
void game_close(Game *self);
void game_render(Game *self, uint8_t alpha);

SDL_Renderer *game_get_renderer(Game *self);
int game_get_scalex(Game *self);
int game_get_scaley(Game *self);

void game_set_points(Game *self, Points q, int points);
int game_get_points(Game *self, Points quiz);

void game_set_name(Game *self, char *name);
char *game_get_name(Game *self);

#endif
