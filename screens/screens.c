#include "screens.h"

#include "../objects/game.h"

#include "titlescreen.h"
#include "testscreen1.h"
#include "menu.h"
#include "demo1.h"
#include "quiz1.h"
#include "demo2.h"
#include "quiz2.h"
#include "demo3.h"
#include "quiz3.h"
#include "demo4.h"
#include "quiz4.h"

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
        case SCREEN_DEMO2:
        return screens_demo2(game);
        case SCREEN_QUIZ2:
        return screens_quiz2(game);
        case SCREEN_DEMO3:
        return screens_demo3(game);
        case SCREEN_QUIZ3:
        return screens_quiz3(game);
        case SCREEN_DEMO4:
        return screens_demo4(game);
        case SCREEN_QUIZ4:
        return screens_quiz4(game);
        default:
        return screens_testscreen1(game);
    }
}
