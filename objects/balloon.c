#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdlib.h>
#include <stdio.h> //for testing only
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "texture.h"
#include "colors.h"
#include "balloon.h"
#include "game.h"

struct balloon_t {
    int num_lines;
    char lines[MAX_LINES][BALLOON_TEXT_SIZE + 1];
    char *line_buf;
    size_t buf_size;
    int num_anim;
    bool read_more;

    int btarget_y;
    double bcurrent_y;
    int breal_y;
    int atarget_y;
    double acurrent_y;
    int areal_y;
    
    Texture *tex_balloon;
    Texture *tex_text[MAX_LINES];
    Texture *tex_anim;
    SDL_Rect anim_clips[3];

    FILE *file;
};

Balloon *balloon_create(Texture *tex_balloon, Texture *tex_anim, char *path, bool read_more) {
    Balloon *self = (Balloon*)malloc(sizeof(Balloon));
    if (self == NULL) return NULL;

    self->tex_balloon = tex_balloon;
    self->tex_anim = tex_anim;
    self->btarget_y = SCREEN_H;
    self->bcurrent_y = SCREEN_H;
    self->atarget_y = SCREEN_H;
    self->acurrent_y = SCREEN_H;

    self->file = fopen(path, "r");
    if (self->file == NULL) {
        printf("Couldnt open file %s\n", path);
        return NULL;
    }

    for (int i = 0; i < MAX_LINES; i++) {
        self->tex_text[i] = texture_create();
        if (self->tex_text[i] == NULL) return NULL;
    }

    self->read_more = read_more;
    self->num_lines = 0;
    self->num_anim = 0;
    
    self->buf_size = BALLOON_TEXT_SIZE + 1;
    self->line_buf = (char*)malloc(sizeof(char) * self->buf_size);
    if (self->line_buf == NULL) return NULL;

    self->anim_clips[0].x = 0;
    self->anim_clips[0].y = 0;
    self->anim_clips[0].w = 192;
    self->anim_clips[0].h = 256;

    self->anim_clips[1].x = 192;
    self->anim_clips[1].y = 0;
    self->anim_clips[1].w = 192;
    self->anim_clips[1].h = 256;

    self->anim_clips[2].x = 384;
    self->anim_clips[2].y = 0;
    self->anim_clips[2].w = 192;
    self->anim_clips[2].h = 256;

    return self;
}

void balloon_free(Balloon *self) {
    if (self->file != NULL) fclose(self->file);
    for (int i = 0; i < MAX_LINES; i++) {
        texture_free(self->tex_text[i]);
    }
    free(self);
}

bool balloon_load_new_file(Balloon *self, char *path, bool read_more) {
    if (self->file != NULL) fclose(self->file);
    self->file = fopen(path, "r");
    if (self->file == NULL) {
        printf("Couldnt open file %s\n", path);
        return false;
    }
    self->read_more = read_more;
    return true;
}

bool balloon_read_text(Balloon *self, SDL_Renderer *renderer, TTF_Font *font) {
    if (fscanf(self->file, "%d %d\n", &self->num_lines, &self->num_anim) == EOF) {
        self->btarget_y = SCREEN_H;
        self->atarget_y = SCREEN_H;
        return false;
    }
    self->num_anim %= 3;
    printf("%d %d\n", self->num_lines, self->num_anim);
    for (int i = 0; i < self->num_lines; i++) {
        getline(&self->line_buf, &self->buf_size, self->file);
        self->line_buf[strcspn(self->line_buf, "\r\n")] = '\0';
        if (i < MAX_LINES) {
            memcpy(self->lines[i], self->line_buf, BALLOON_TEXT_SIZE);
            self->lines[i][BALLOON_TEXT_SIZE] = '\0';
            if (!texture_load_from_text(self->tex_text[i], renderer, font, self->lines[i], COLOR_TEXT_DEFAULT)) return false;
        }
    } 

    self->num_lines = self->num_lines * (self->num_lines <= MAX_LINES) + MAX_LINES * (self->num_lines > MAX_LINES);
    self->btarget_y = SCREEN_H - (self->num_lines * 8 + 16) * (self->num_lines > 0);
    self->atarget_y = SCREEN_H - texture_geth(self->tex_anim) * (self->num_lines > 0);

    self->read_more = false;
    return true;
}

void balloon_handle_event(Balloon *self, SDL_Event e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) self->read_more = true;
}

void balloon_render(Balloon *self, SDL_Renderer *renderer) {
    texture_render(self->tex_anim, renderer, 640-192, self->acurrent_y, &self->anim_clips[self->num_anim]);
    texture_render(self->tex_balloon, renderer, 0, self->breal_y, NULL);
    for (int i = 0; i < self->num_lines; i++) {
        texture_render(self->tex_text[i], renderer, 8, self->breal_y + 8 + 8 * i, NULL);
    }
}

bool balloon_read_more(Balloon *self) {
    return self->read_more;
}

void balloon_calc_position(Balloon *self) {
    self->bcurrent_y = (self->bcurrent_y + self->btarget_y) / 2.0;
    self->breal_y = round(self->bcurrent_y);
    
    self->acurrent_y = (self->acurrent_y + self->atarget_y) / 2.0;
    self->areal_y = round(self->acurrent_y);
}

bool balloon_ishidden(Balloon *self) {
    return (self->breal_y >= SCREEN_H && self->areal_y >= SCREEN_H);
}
