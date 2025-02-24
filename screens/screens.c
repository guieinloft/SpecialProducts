#include "screens.h"
#include "../objects/game.h"
#include "titlescreen.h"
#include "testscreen1.h"
#include "menu.h"
#include "quiz1.h"
#include "demo1.h"

Screen screens_redirect(Screen screen, Game *game) {
    switch (screen) {
        case SCREEN_TITLESCREEN:
        return screens_titlescreen(game);
        case SCREEN_TESTSCREEN1:
        return screens_testscreen1(game);
        case SCREEN_MENU:
        return screens_menu(game);
        case SCREEN_DEMO1:
        return screens_demo1(game);
        case SCREEN_QUIZ1:
        return screens_quiz1(game);
        default:
        return screens_testscreen1(game);
    }
}
