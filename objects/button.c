#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "button.h"
#include "texture.h"

#define BUTTON_MARGIN 4
#define BUTTON_HEIGHT 24
#define BUTTON_BLOCK_SIZE 8

struct button_t {
    int x;
    int y;
    int width;

    Texture *tex_button;
    SDL_Rect clips[4][3];
    int current;

    Texture *tex_text;

    bool over;
    bool selected;
    bool pressed;
    bool centered;
    bool sticky;
};

Button *button_create(Texture *tex_button) {
    Button *self = (Button*)malloc(sizeof(Button));
    if (self == NULL) return NULL;

    self->x = 0;
    self->y = 0;
    self->width = 1;
    self->tex_button = tex_button;
    if (self->tex_button == NULL) return NULL;

    self->tex_text = texture_create();
    if (self->tex_text == NULL) return NULL;

    //initialize clips
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            self->clips[i][j].x = (i * 3 + j) * BUTTON_BLOCK_SIZE;
            self->clips[i][j].y = 0;
            self->clips[i][j].w = BUTTON_BLOCK_SIZE;
            self->clips[i][j].h = BUTTON_HEIGHT;
        }
    }

    self->current = 0;
    self->over = false;
    self->selected = false;
    self->pressed = false;
    self->sticky = false;

    return self;
}

void button_free(Button *self) {
    texture_free(self->tex_text);
    free(self);
}

bool button_change_text(Button *self, SDL_Renderer *renderer, TTF_Font *font, char *text, int width, SDL_Color text_color, bool centered) {
    self->centered = centered;
    self->width = width;
    if(!texture_load_from_text(self->tex_text, renderer, font, text, text_color)) {
        return false;
    }
    return true;
}

void button_change_position(Button *self, int x, int y) {
    self->x = x;
    self->y = y;
}

void button_handle_event(Button *self, SDL_Event e, double scalex, double scaley) {
    if (e.type != SDL_MOUSEMOTION && e.type != SDL_MOUSEBUTTONDOWN && e.type != SDL_MOUSEBUTTONUP) return;
    int mousex, mousey;
    SDL_GetMouseState(&mousex, &mousey);

    //check if mouse is over button
    self->over = true;

    if (mousex < (self->x - BUTTON_MARGIN) * scalex) self->over = false;
    if (mousex > (self->x + (self->width + 2) * BUTTON_BLOCK_SIZE - BUTTON_MARGIN) * scalex) self->over = false;
    if (mousey < (self->y - BUTTON_MARGIN) * scaley) self->over = false;
    if (mousey > (self->y + BUTTON_HEIGHT - BUTTON_MARGIN) * scaley) self->over = false;

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (self->over) {
            self->selected = true;
            self->pressed = true;
        }
        else {
            self->selected = false;
            self->pressed = false;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONUP) {
        if (!self->sticky) {
            self->selected = false;
            self->pressed = false;
        }
    }

    self->current = 0 + self->over + (self->selected << 1);
}

void button_render(Button *self, SDL_Renderer *renderer) {
    texture_render(self->tex_button, renderer, self->x, self->y, &(self->clips[self->current][0]));
    texture_render_scaled(self->tex_button, renderer, self->x + BUTTON_BLOCK_SIZE, self->y, &(self->clips[self->current][1]), self->width, 1, 0, NULL);
    texture_render(self->tex_button, renderer, self->x + (self->width + 1) * BUTTON_BLOCK_SIZE, self->y, &(self->clips[self->current][2]));
    
    int text_w, text_h;
    if (self->centered) text_w = (((self->width + 2) * BUTTON_BLOCK_SIZE) - texture_getw(self->tex_text)) / 2;
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

bool button_ispressed(Button *self) {
    bool pressed = self->pressed;
    self->pressed = false;
    return pressed;
}

bool button_issticky(Button *self) {
    return self->sticky;
}

int button_get_w(Button *self) {
    return self->width;
}

int button_get_real_w(Button *self) {
    return (self->width + 2) * BUTTON_BLOCK_SIZE;
}

void button_set_sticky(Button *self, bool sticky) {
    self->sticky = sticky;
}
