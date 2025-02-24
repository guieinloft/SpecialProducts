#ifndef TIMER_H
#define TIMER_H

#define TICKS_TOTAL 600000

typedef struct timer_t Timer;

Timer *timer_create();
void timer_free(Timer *self);

void timer_start(Timer *self);
void timer_stop(Timer *self);
void timer_pause(Timer *self);
void timer_unpause(Timer *self);

uint32_t timer_get_ticks(Timer *self);
bool timer_isstarted(Timer *self);
bool timer_ispaused(Timer *self);

#endif
