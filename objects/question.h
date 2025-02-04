#ifndef QUESTION_H
#define QUESTION_H

#include "textbox.h"
#include "texture.h"
#include "colors.h"

typedef struct question_t Question;

typedef struct question_container_t {
    Question *question;
    Texture *texture;
    Textbox *textbox;
    bool correct;

    int x;
    int y;
} QuestionContainer;

Question *question_create(int a, int b, int c, char *q_text, char *a_text);
void question_free(Question *self);
bool question_compare(Question *self, char *a_text);
char *question_get_q_text(Question *self);

#endif
