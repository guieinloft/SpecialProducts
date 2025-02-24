#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "game.h"
#include "question.h"
#include "textbox.h"
#include "texture.h"
#include "colors.h"

struct question_t {
    int a;
    int b;
    int c;
    char q_text[TEXTBOX_TEXT_SIZE + 1];
    char a_text[TEXTBOX_TEXT_SIZE + 1];
    int points;
};

Question *question_create(int a, int b, int c, char *q_text, char *a_text) {
    Question *self = (Question*)malloc(sizeof(Question));
    if (self == NULL) return NULL;

    self->a = a;
    self->b = b;
    self->c = c;

    memcpy(self->q_text, q_text, TEXTBOX_TEXT_SIZE);
    memcpy(self->a_text, a_text, TEXTBOX_TEXT_SIZE);

    self->points = QUESTION_POINTS;
    
    return self;
}

void question_free(Question *self) {
    free(self);
}

bool question_compare(Question *self, char *a_text) {
    bool correct = (strcmp(self->a_text, a_text) == 0);
    if (!correct && self->points > 0) {
        self->points -= QUESTION_PUNISH;
    }
    return correct;
    //return true; //for debugging
}

char *question_get_q_text(Question *self) {
    return self->q_text;
}

char *question_get_a_text(Question *self) {
    return self->a_text;
}

int question_get_points(Question *self) {
    return self->points;
}

//QUESTIONS {{{1

Question *question_1_7_create(bool inverted) {
    int a = rand() % 8 + 2;
    int b = 0;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(x+%d)^2", a);
    sprintf(text2, "x^2+%dx+%d", a*2, a*a);

    if (inverted) {
        return question_create(a, b, c, text2, text1);
    }
    return question_create(a, b, c, text1, text2);
}

Question *question_2_8_create(bool inverted) {
    int a = rand() % 8 + 2;
    int b = 0;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(%d+x)^2", a);
    sprintf(text2, "%d+%dx+x^2", a*a, a*2);

    if (inverted) {
        return question_create(a, b, c, text2, text1);
    }
    return question_create(a, b, c, text1, text2);
}

Question *question_3_9_create(bool inverted) {
    int a = rand() % 8 + 2;
    int b = 0;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(y+%d)^2", a);
    sprintf(text2, "y^2+%dy+%d", a*2, a*a);

    if (inverted) {
        return question_create(a, b, c, text2, text1);
    }
    return question_create(a, b, c, text1, text2);
}

Question *question_4_10_create(bool inverted) {
    int a = (rand() % 4 + 1) * 2 + 1;
    int b = 0;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(x/2+%d/2)^2", a);
    sprintf(text2, "x^2/4+%dx+%d/4", a, a*a);

    if (inverted) {
        return question_create(a, b, c, text2, text1);
    }
    return question_create(a, b, c, text1, text2);
}

Question *question_5_11_create(bool inverted) {
    int a = rand() % 3 + 2;
    int b = rand() % 8 + 2;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(%dx+%d)^2", a, b);
    sprintf(text2, "%dx^2+%dx+%d", a*a, 2*a*b, b*b);

    if (inverted) {
        return question_create(a, b, c, text2, text1);
    }
    return question_create(a, b, c, text1, text2);
}

Question *question_6_12_create(bool inverted) {
    int a = rand() % 8 + 2;
    int b = rand() % 8 + 2;
    int c = 0;

    char text1[TEXTBOX_TEXT_SIZE];
    char text2[TEXTBOX_TEXT_SIZE];

    sprintf(text1, "(%dx+%dy)^2", a, b);
    sprintf(text2, "%dx^2+%dxy+%dy^2", a*a, 2*a*b, b*b);

    if (inverted) {
        return question_create(a, b, c, text2, text1);
    }
    return question_create(a, b, c, text1, text2);
}

Question *question_n_create(int n) {
    switch(n) {
        case 0:
        return question_1_7_create(false);
        case 1:
        return question_2_8_create(false);
        case 2:
        return question_3_9_create(false);
        case 3:
        return question_4_10_create(false);
        case 4:
        return question_5_11_create(false);
        case 5:
        return question_6_12_create(false);
        case 6:
        return question_1_7_create(true);
        case 7:
        return question_2_8_create(true);
        case 8:
        return question_3_9_create(true);
        case 9:
        return question_4_10_create(true);
        case 10:
        return question_5_11_create(true);
        case 11:
        return question_6_12_create(true);
        default:
        return NULL;
    }
}

