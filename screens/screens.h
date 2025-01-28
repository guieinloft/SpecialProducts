#ifndef SCREENS_H
#define SCREENS_H

#include "../objects/window.h"

typedef enum screen_e {
    SCREEN_QUIT,
    SCREEN_TITLESCREEN,
    SCREEN_TESTSCREEN1,
    SCREEN_TESTSCREEN2,
    SCREEN_TOTAL,
    SCREEN_ERROR = -1
} Screen;

Screen screens_redirect(Screen screen, Window *window);

#endif
