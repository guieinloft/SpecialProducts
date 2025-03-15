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
    char a1_text[TEXTBOX_TEXT_SIZE + 1];
    char a2_text[TEXTBOX_TEXT_SIZE + 1];
    int points;
};

Question *question_create(int a, int b, int c, char *q_text, char *a1_text, char *a2_text, bool bonus) {
    Question *self = (Question*)malloc(sizeof(Question));
    if (self == NULL) return NULL;

    self->a = a;
    self->b = b;
    self->c = c;

    memcpy(self->q_text, q_text, TEXTBOX_TEXT_SIZE);
    memcpy(self->a1_text, a1_text, TEXTBOX_TEXT_SIZE);
    memcpy(self->a2_text, a2_text, TEXTBOX_TEXT_SIZE);

    self->points = QUESTION_POINTS + QUESTION_POINTS * bonus;
    
    return self;
}

void question_free(Question *self) {
    free(self);
}

bool question_compare(Question *self, char *a_text) {
    bool correct = (strcmp(self->a1_text, a_text) == 0) | (strcmp(self->a2_text, a_text) == 0);
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
    return self->a1_text;
}

int question_get_points(Question *self) {
    return self->points;
}

