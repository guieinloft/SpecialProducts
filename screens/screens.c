#include "screens.h"
#include "../objects/window.h"
#include "titlescreen.h"
#include "testscreen1.h"
#include "menu.h"
#include "quiz1.h"

Screen screens_redirect(Screen screen, Window *window) {
    switch (screen) {
        case SCREEN_TITLESCREEN:
        return screens_titlescreen(window);
        case SCREEN_TESTSCREEN1:
        return screens_testscreen1(window);
        case SCREEN_MENU:
        return screens_menu(window);
        case SCREEN_QUIZ1:
        return screens_quiz1(window);
        default:
        return screens_testscreen1(window);
    }
}
