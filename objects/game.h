#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define SCREEN_W 640
#define SCREEN_H 360

typedef enum points_e {
    POINTS_1 = 0x00000001,
    POINTS_2 = 0x00000002,
    POINTS_3 = 0x00000004,
    POINTS_4 = 0x00000008,
    POINTS_ALL = 0x000000F,
    POINTS_NONE = 0x00000000
} Points;

typedef enum options_e {
    OPTION_IGNORE_RATIO,
    OPTION_INTEGER_SCALING,
    OPTION_MUTE_MUSIC,
    OPTION_MUTE_SOUND,
    OPTION_BILINEAR_FILTERING,
    OPTION_TOTAL
} Option;

typedef struct game_t Game;

Game *game_init(void);
void game_close(Game *self);
void game_render(Game *self, uint8_t alpha);
void game_clear_screen(Game *self);
void game_reset_render_target(Game *self);

SDL_Renderer *game_get_renderer(Game *self);
double game_get_scalex(Game *self);
double game_get_scaley(Game *self);
int game_get_screenx(Game *self);
int game_get_screeny(Game *self);

void game_set_points(Game *self, Points p, int points);
void game_set_name(Game *self, char *name);

int game_get_points(Game *self, Points p);
char *game_get_name(Game *self);

int game_get_rank_points(Game *self, int n);
char *game_get_rank_name(Game *self, int n);

int game_get_position(Game *self);
void game_calc_position(Game *self);

bool game_handle_event(Game *self, SDL_Event e);
bool game_save(Game *self, bool complete);

bool game_get_option(Game *self, Option option);
bool game_change_option(Game *self, Option option);

#endif
