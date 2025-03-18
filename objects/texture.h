#ifndef TEXTURE_H
#define TEXTURE_H

#include "game.h"

typedef struct texture_t Texture;

Texture *texture_create(void);
void texture_free(Texture *self);

bool texture_load_from_file(Texture *self, SDL_Renderer *renderer, char *path);
bool texture_load_from_text(Texture *self, SDL_Renderer *renderer, TTF_Font *font, char *text, SDL_Color textcolor);
bool texture_create_blank(Texture *self, SDL_Renderer *renderer, int w, int h, SDL_TextureAccess access);
void texture_clear(Texture *self);

void texture_set_color_mod(Texture *self, SDL_Color color, SDL_BlendMode blend);

void texture_render(Texture *self, SDL_Renderer *renderer, int x, int y, SDL_Rect *clip);
void texture_render_scaled(Texture *self, SDL_Renderer *renderer, int x, int y, SDL_Rect *clip, double scalex, double scaley, double angle, SDL_Point *center);
bool texture_set_as_render_target(Texture *self, SDL_Renderer *renderer);

int texture_getw(Texture *self);
int texture_geth(Texture *self);

#endif
