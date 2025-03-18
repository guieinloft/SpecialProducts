#include "game.h"
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
    SDL_SetTextureBlendMode(self->texture, SDL_BLENDMODE_NONE);
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

bool texture_create_blank(Texture *self, SDL_Renderer *renderer, int w, int h, SDL_TextureAccess access) {
    texture_clear(self);

    self->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, access, w, h);
    if (self->texture == NULL) return false;
    self->w = w;
    self->h = h;
    return self->texture != NULL;
}

void texture_clear(Texture *self) {
    if (self->texture != NULL) SDL_DestroyTexture(self->texture);
}

void texture_set_color_mod(Texture *self, SDL_Color color, SDL_BlendMode blend) {
    SDL_SetTextureBlendMode(self->texture, blend);
    SDL_SetTextureColorMod(self->texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(self->texture, color.a);
}

void texture_render(Texture *self, SDL_Renderer *renderer, int x, int y, SDL_Rect *clip) {
    if (self == NULL || self->texture == NULL) return;
    SDL_Rect rquad = {x, y, self->w, self->h};
    if (clip != NULL) {
        rquad.w = clip->w;
        rquad.h = clip->h;
    }
    SDL_RenderCopy(renderer, self->texture, clip, &rquad);
}

bool texture_set_as_render_target(Texture *self, SDL_Renderer *renderer) {
    if (self == NULL) return false;
    SDL_SetRenderTarget(renderer, self->texture);
    return true;
}

void texture_render_scaled(Texture *self, SDL_Renderer *renderer, int x, int y, SDL_Rect *clip, double scalex, double scaley, double angle, SDL_Point *center) {
    if (self == NULL || self->texture == NULL) return;
    SDL_Rect rquad = {x, y, self->w, self->h};
    if (clip != NULL) {
        rquad.w = clip->w;
        rquad.h = clip->h;
    }
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (scalex < 0) {
        scalex *= -1.0;
        flip |= SDL_FLIP_HORIZONTAL;
    }
    if (scaley < 0) {
        scaley *= -1.0;
        flip |= SDL_FLIP_VERTICAL;
    }
    rquad.w *= scalex;
    rquad.h *= scaley;
    SDL_RenderCopyEx(renderer, self->texture, clip, &rquad, angle, center, flip);
}

int texture_getw(Texture *self) {
    return self->w;
}

int texture_geth(Texture *self) {
    return self->h;
}
