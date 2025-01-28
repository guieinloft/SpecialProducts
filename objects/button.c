#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdlib.h>
#include <stdbool.h>

#include "button.h"
#include "texture.h"

#define BUTTON_MARGIN 4
#define BUTTON_HEIGHT 24

struct button_t {
    int x;
    int y;
    int width;

    Texture *tex_button;
    SDL_Rect clips[4];
    int current;

    Texture *tex_text;

    bool over;
    bool selected;
    bool centered;
};

Button *button_create(int x, int y, int size, Texture *tex_button) {
    Button *self = (Button*)malloc(sizeof(Button));
    if (self == NULL) return NULL;

    self->x = x;
    self->y = y;
    self->width = size;
    self->tex_button = tex_button;

    self->tex_text = texture_create();

    //initialize clips
    self->clips[0].x = 0;
    self->clips[0].y = 0;
    self->clips[0].w = size;
    self->clips[0].h = BUTTON_HEIGHT;

    self->clips[1].x = 0;
    self->clips[1].y = 24;
    self->clips[1].w = size;
    self->clips[1].h = BUTTON_HEIGHT;

    self->clips[2].x = 0;
    self->clips[2].y = 48;
    self->clips[2].w = size;
    self->clips[2].h = BUTTON_HEIGHT;

    self->clips[3].x = 0;
    self->clips[3].y = 72;
    self->clips[3].w = size;
    self->clips[3].h = BUTTON_HEIGHT;

    self->over = false;
    self->selected = false;

    return self;
}

void button_free(Button *self) {
    texture_free(self->tex_text);
    free(self);
}

bool button_change_text(Button *self, SDL_Renderer *renderer, TTF_Font *font, char *text, SDL_Color text_color, bool centered) {
    if(!texture_load_from_text(self->tex_text, renderer, font, text, text_color)) return false;
    self->centered = centered;
    return true;
}

void button_handle_event(Button *self, SDL_Event e, double scalex, double scaley) {
    if (e.type != SDL_MOUSEMOTION && e.type != SDL_MOUSEBUTTONDOWN && e.type != SDL_MOUSEBUTTONUP) return;
    int mousex, mousey;
    SDL_GetMouseState(&mousex, &mousey);

    //check if mouse is over button
    self->over = true;

    if (mousex < (self->x - BUTTON_MARGIN) * scalex) self->over = false;
    if (mousex > (self->x + self->width - BUTTON_MARGIN) * scalex) self->over = false;
    if (mousey < (self->y - BUTTON_MARGIN) * scaley) self->over = false;
    if (mousey > (self->y + BUTTON_HEIGHT - BUTTON_MARGIN) * scaley) self->over = false;

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (self->over) self->selected = true;
        else self->selected = false;
    }

    self->current = 0 + self->over + (self->selected << 1);
}

void button_render(Button *self, SDL_Renderer *renderer) {
    texture_render(self->tex_button, renderer, self->x, self->y, &(self->clips[self->current]));
    int text_w, text_h;
    if (self->centered) text_w = (self->width - texture_getw(self->tex_text)) / 2;
    else text_w = 8;
    text_h = (BUTTON_HEIGHT - texture_geth(self->tex_text)) / 2;

    texture_render(self->tex_text, renderer, self->x + text_w, self->y + text_h, NULL);
}

bool button_isover(Button *self) {
    return self->over;
}

bool button_isselected(Button *self) {
    return self->selected;
}
