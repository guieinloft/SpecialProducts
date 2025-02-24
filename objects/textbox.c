#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdlib.h>
#include <stdio.h> //for testing only
#include <stdbool.h>

#include "button.h"
#include "texture.h"
#include "textbox.h"
#include "colors.h"

struct textbox_t {
    Button *button;
    char text[TEXTBOX_TEXT_SIZE + 1];
    int text_size;

    bool typable;
    bool enter;
    bool render;
};

Textbox *textbox_create (int x, int y, Texture *tex_button) {
    Textbox *self = (Textbox*)malloc(sizeof(Textbox));
    if (self == NULL) return NULL;

    self->button = button_create(tex_button);
    button_set_sticky(self->button, true);
    button_change_position(self->button, x, y);

    self->text_size = 0;
    self->text[0] = '\0';

    self->typable = true;
    self->render = true;
    self->enter = false;
}

void textbox_free(Textbox *self) {
    button_free(self->button);
    free(self);
}

bool textbox_handle_keypress(Textbox *self, SDL_Event e) {
    self->enter = false;
    switch (e.key.keysym.sym) {
        case SDLK_BACKSPACE:
        if (self->text_size > 0) {
            self->text_size--;
            while ((self->text[self->text_size] & 0xC0) == 0x80) {
                self->text[self->text_size] = '\0';
                self->text_size--;
            }
            self->text[self->text_size] = '\0';
            return true;
        }
        return false;

        case SDLK_RETURN:
        self->enter = true;
        return true;

        default:
        return false;
    }
}

bool textbox_handle_textinput(Textbox *self, SDL_Event e) {
    if (self->text_size < TEXTBOX_TEXT_SIZE && e.text.text[0] != ' ') {
        int i = 0;
        while (e.text.text[i] != '\0' && self->text_size < TEXTBOX_TEXT_SIZE) {
            self->text[self->text_size] = e.text.text[i];
            self->text_size++;
            i++;
        }
        self->text[self->text_size] = '\0';
        return true;
    }
    return false;
}

void textbox_handle_event(Textbox *self, SDL_Event e, double scalex, double scaley) {
    button_handle_event(self->button, e, scalex, scaley);
    if (!button_isselected(self->button)) return;
    if (!self->typable) return;

    self->render = false;
    if (e.type == SDL_KEYDOWN) self->render = textbox_handle_keypress(self, e);
    else if (e.type == SDL_TEXTINPUT) self->render = textbox_handle_textinput(self, e);
}

void textbox_render(Textbox *self, SDL_Renderer *renderer, TTF_Font *font) {
    if (self->render) {
        SDL_Color text_color = COLOR_TEXT_DEFAULT;
        if (!self->typable) {
            text_color = COLOR_TEXT_TEXTBOX_RIGHT;
        }
        else if (self->enter) {
            text_color = COLOR_TEXT_TEXTBOX_WRONG;
        }
        if (self->text_size == 0) {
            text_color = COLOR_TEXT_TEXTBOX_EMPTY;
            button_change_text(self->button, renderer, font, "Digite aqui", TEXTBOX_TEXT_SIZE, text_color, false);
        }
        else {
            button_change_text(self->button, renderer, font, self->text, TEXTBOX_TEXT_SIZE, text_color, false);
        }
        self->render = false;
    }
    button_render(self->button, renderer);
    self->enter = false;
}

bool textbox_get_enter(Textbox *self) {
    bool enter = self->enter;
    return enter;
}

void textbox_set_enter(Textbox *self, bool enter) {
    if (!self->typable) return;
    self->enter = enter;
    if (enter) self->render = true;
}

bool textbox_istypable(Textbox *self) {
    return self->typable;
}

void textbox_set_typable(Textbox *self, bool typable) {
    self->typable = typable;
    self->render = true;
}

char *textbox_get_text(Textbox *self) {
    return self->text;
}

void textbox_set_text(Textbox *self, char *text) {
    self->typable = false;
    self->text_size = 30;
    memcpy(self->text, text, 31);
}

bool textbox_isempty(Textbox *self) {
    return (self->text_size == 0);
}
