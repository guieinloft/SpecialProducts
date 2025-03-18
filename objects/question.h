#ifndef QUESTION_H
#define QUESTION_H

#include "game.h"
#include "textbox.h"
#include "texture.h"
#include "colors.h"

#define QUESTION_POINTS 2500
#define QUESTION_PUNISH 500

typedef struct question_t Question;

Question *question_create(int a, int b, int c, char *q_text, char *a1_text, char *a2_text, bool bonus);
void question_free(Question *self);
bool question_compare(Question *self, char *a_text);
char *question_get_q_text(Question *self);
char *question_get_a_text(Question *self);
int question_get_points(Question *self);

#endif
