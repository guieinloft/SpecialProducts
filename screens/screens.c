#include "screens.h"
#include "../objects/window.h"
#include "titlescreen.h"
#include "testscreen1.h"
#include "testscreen2.h"

Screen screens_redirect(Screen screen, Window *window) {
    switch (screen) {
        case SCREEN_TITLESCREEN:
        return screens_TitleScreen(window);
        case SCREEN_TESTSCREEN1:
        return screens_TestScreen1(window);
        case SCREEN_TESTSCREEN2:
        return screens_TestScreen2(window);
        default:
        return screen;
    }
}
