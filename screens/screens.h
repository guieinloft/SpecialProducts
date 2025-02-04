#ifndef SCREENS_H
#define SCREENS_H

#include "../objects/window.h"

typedef enum screen_e {
    SCREEN_QUIT,
    SCREEN_TITLESCREEN,
    SCREEN_TESTSCREEN1,
    SCREEN_MENU,
    SCREEN_OPTIONS,
    SCREEN_DEMO1,
    SCREEN_DEMO2,
    SCREEN_DEMO3,
    SCREEN_DEMO4,
    SCREEN_QUIZ1,
    SCREEN_QUIZ2,
    SCREEN_QUIZ3,
    SCREEN_QUIZ4,
    SCREEN_TOTAL,
    SCREEN_ERROR = -1
} Screen;

Screen screens_redirect(Screen screen, Window *window);

#endif
