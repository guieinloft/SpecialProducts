#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef struct texture_t Texture;

Texture *texture_create(void);
void texture_free(Texture *self);

bool texture_load_from_file(Texture *self, SDL_Renderer *renderer, char *path);
#if defined(SDL_TTF_MAJOR_VERSION)
bool texture_load_from_text(Texture *self, SDL_Renderer *renderer, TTF_Font *font, char *text, SDL_Color textcolor);
#endif
void texture_clear(Texture *self);

void texture_set_color_mod(Texture *self, SDL_Color color, SDL_BlendMode blend);

void texture_render(Texture *self, SDL_Renderer *renderer, int x, int y, SDL_Rect *clip);
void texture_render_scaled(Texture *self, SDL_Renderer *renderer, int x, int y, SDL_Rect *clip, double scalex, double scaley, double angle, SDL_Point *center);

int texture_getw(Texture *self);
int texture_geth(Texture *self);

#endif
