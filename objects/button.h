#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "texture.h"

typedef struct button_t Button;

Button *button_create(int x, int y, int size, Texture *texture);
void button_free(Button *self);

bool button_change_text(Button *self, SDL_Renderer *renderer, TTF_Font *font, char *text, int text_size, SDL_Color text_color);

void button_handle_event(Button *self, SDL_Event e, double scalex, double scaley);

void button_render(Button *self, SDL_Renderer *renderer);

bool button_isover(Button *self);
bool button_isselected(Button *self);

#endif
