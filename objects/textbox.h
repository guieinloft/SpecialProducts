#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>

#include "button.h"
#include "texture.h"

#define TEXTBOX_TEXT_SIZE 30

typedef struct textbox_t Textbox;

Textbox *textbox_create (int x, int y, Texture *tex_button);
void textbox_free(Textbox *self);

void textbox_handle_event(Textbox *self, SDL_Event e, double scalex, double scaley);

void textbox_render(Textbox *self, SDL_Renderer *renderer, TTF_Font *font);

bool textbox_get_enter(Textbox *self);
void textbox_set_enter(Textbox *self, bool enter);

bool textbox_istypable(Textbox *self);
void textbox_set_typable(Textbox *self, bool typable);

char *textbox_get_text(Textbox *self);
void textbox_set_text(Textbox *self, char *text);
bool textbox_isempty(Textbox *self);

#endif
