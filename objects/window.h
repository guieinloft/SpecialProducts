#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

#define SCREEN_W 640
#define SCREEN_H 360

typedef struct window_t Window;

Window *window_init(void);
void window_close(Window *self);
void window_render(Window *self);

SDL_Renderer *window_get_renderer(Window *self);

#endif
