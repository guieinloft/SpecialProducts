#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "question.h"
#include "textbox.h"
#include "texture.h"
#include "colors.h"

struct question_t {
    int a;
    int b;
    int c;
    char q_text[TEXTBOX_TEXT_SIZE];
    char a_text[TEXTBOX_TEXT_SIZE];
};

Question *question_create(int a, int b, int c, char *q_text, char *a_text) {
    Question *self = (Question*)malloc(sizeof(Question));
    if (self == NULL) return NULL;

    self->a = a;
    self->b = b;
    self->c = c;

    memcpy(self->q_text, q_text, TEXTBOX_TEXT_SIZE);
    memcpy(self->a_text, a_text, TEXTBOX_TEXT_SIZE);
    
    return self;
}

void question_free(Question *self) {
    free(self);
}

bool question_compare(Question *self, char *a_text) {
    return (strcmp(self->a_text, a_text) == 0);
}

char *question_get_q_text(Question *self) {
    return self->q_text;
}
