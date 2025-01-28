#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "texture.h"

struct texture_t {
    SDL_Texture *texture;
    int w;
    int h;
};

Texture *texture_create(void) {
    Texture *self = (Texture*)malloc(sizeof(Texture));
    self->texture = NULL;
    self->w = 0;
    self->h = 0;
    return self;
}

void texture_free(Texture *self) {
    texture_clear(self);
    free(self);
}

bool texture_load_from_file(Texture *self, SDL_Renderer *renderer, char *path) {
    //clear previous texture
    texture_clear(self);
    //load image
    SDL_Surface *newsurface = IMG_Load(path);
    if (newsurface == NULL) {
        return false;
    }
    SDL_Texture *newtexture = SDL_CreateTextureFromSurface(renderer, newsurface);
    self->w = newsurface->w;
    self->h = newsurface->h;
    SDL_FreeSurface(newsurface);
    if (newtexture == NULL) {
        return false;
    }
    self->texture = newtexture;
    if (self->texture == NULL) {
        return false;
    }
    return true;
}

bool texture_load_from_text(Texture *self, SDL_Renderer *renderer, TTF_Font *font, char *text, SDL_Color textcolor) {
    //clear previous texture
    texture_clear(self);
    //load image
    SDL_Surface *newsurface = TTF_RenderUTF8_Solid(font, text, textcolor);
    if (newsurface == NULL) {
        return false;
    }
    SDL_Texture *newtexture = SDL_CreateTextureFromSurface(renderer, newsurface);
    self->w = newsurface->w;
    self->h = newsurface->h;
    SDL_FreeSurface(newsurface);
    if (newtexture == NULL) {
        return false;
    }
    self->texture = newtexture;
    if (self->texture == NULL) {
        return false;
    }
    return true;
    
}

void texture_clear(Texture *self) {
    if (self->texture != NULL) SDL_DestroyTexture(self->texture);
}

void texture_render(Texture *self, SDL_Renderer *renderer, int x, int y, SDL_Rect *clip) {
    if (self->texture == NULL) return;
    SDL_Rect rquad = {x, y, self->w, self->h};
    if (clip != NULL) {
        rquad.w = clip->w;
        rquad.h = clip->h;
    }
    SDL_RenderCopy(renderer, self->texture, clip, &rquad);
}

int texture_getw(Texture *self) {
    return self->w;
}

int texture_geth(Texture *self) {
    return self->h;
}
