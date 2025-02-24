#ifndef QUESTION_H
#define QUESTION_H

#include "textbox.h"
#include "texture.h"
#include "colors.h"

#define QUESTION_POINTS 25
#define QUESTION_PUNISH 5

typedef struct question_t Question;

Question *question_create(int a, int b, int c, char *q_text, char *a_text);
void question_free(Question *self);
bool question_compare(Question *self, char *a_text);
char *question_get_q_text(Question *self);
char *question_get_a_text(Question *self);
int question_get_points(Question *self);

//SPECIFIC QUESTIONS
Question *question_n_create(int n);
Question *question_1_7_create(bool inverted);
Question *question_2_8_create(bool inverted);
Question *question_3_9_create(bool inverted);
Question *question_4_10_create(bool inverted);
Question *question_5_11_create(bool inverted);
Question *question_6_12_create(bool inverted);

#endif
