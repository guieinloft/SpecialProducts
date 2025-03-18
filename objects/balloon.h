#ifndef BALLOON_H
#define BALLOON_H

#include "game.h"

#include "texture.h"

#define BALLOON_TEXT_SIZE 255
#define MAX_LINES 5

typedef struct balloon_t Balloon;

Balloon *balloon_create(Texture *tex_balloon, Texture *tex_anim, char *path, bool read_more);
void balloon_free(Balloon *self);

bool balloon_load_new_file(Balloon *self, char *path, bool read_more);
bool balloon_read_text(Balloon *self, SDL_Renderer *renderer, TTF_Font *font);
void balloon_handle_event(Balloon *self, SDL_Event e);
void balloon_render(Balloon *self, SDL_Renderer *renderer);

void balloon_calc_position(Balloon *self);

bool balloon_read_more(Balloon *self);
bool balloon_ishidden(Balloon *self);

#endif
