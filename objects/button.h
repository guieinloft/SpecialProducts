#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "texture.h"

typedef struct button_t Button;

Button *button_create(Texture *texture);
void button_free(Button *self);

bool button_change_text(Button *self, SDL_Renderer *renderer, TTF_Font *font, char *text, int width, SDL_Color text_color, bool centered);
void button_change_position(Button *self, int x, int y);

void button_handle_event(Button *self, SDL_Event e, double scalex, double scaley, int screenx, int screeny);

void button_render(Button *self, SDL_Renderer *renderer);

bool button_isover(Button *self);
bool button_isselected(Button *self);
bool button_ispressed(Button *self);
bool button_issticky(Button *self);
int button_get_w(Button *self);
int button_get_real_w(Button *self);
void button_set_sticky(Button *self, bool sticky);

#endif
