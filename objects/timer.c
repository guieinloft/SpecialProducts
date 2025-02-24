#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "timer.h"

struct timer_t {
    uint32_t start_ticks;
    uint32_t pause_ticks;

    bool started;
    bool paused;
};

Timer *timer_create() {
    Timer *self = (Timer*)malloc(sizeof(Timer));
    if (self == NULL) return NULL;
    self->start_ticks = 0;
    self->pause_ticks = 0;
    self->started = false;
    self->paused = false;

    return self;
}

void timer_free(Timer *self) {
    free(self);
}

void timer_start(Timer *self) {
    self->started = true;
    self->paused = false;
    self->start_ticks = SDL_GetTicks();
    self->pause_ticks = 0;
}

void timer_stop(Timer *self) {
    self->started = false;
    self->paused = false;
    self->start_ticks = 0;
    self->pause_ticks = 0;
}

void timer_pause(Timer *self) {
    if (!self->started || self->paused) return;
    self->paused = true;
    self->pause_ticks = SDL_GetTicks() - self->start_ticks;
    self->start_ticks = 0;
}

void timer_unpause(Timer *self) {
    if (!self->started || !self->paused) return;
    self->paused = false;
    self->start_ticks = SDL_GetTicks() - self->pause_ticks;
    self->pause_ticks = 0;
}

uint32_t timer_get_ticks(Timer *self) {
    if (!self->started) return 0;
    if (self->paused) return self->pause_ticks;
    return SDL_GetTicks() - self->start_ticks;
}

bool timer_isstarted(Timer *self) {
    return self->started;
}

bool timer_ispaused(Timer *self) {
    return self->paused;
}
