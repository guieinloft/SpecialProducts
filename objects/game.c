#include "game.h"

#define TICKS_PER_FRAME 1000/60

struct game_t {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *target;
    SDL_Texture *target_mouse;
    SDL_Texture *mouse;

    int w;
    int h;

    int points[4];
    char name[31];
    int position;
    
    int points_rank[99];
    char name_rank[99][31];
    int score_num;

    bool mfocus;
    bool kfocus;
    bool fullscreen;
    bool minimized;

    bool options[OPTION_TOTAL];
    bool saved;
};

Game *game_init(void) {
    //create Game struct
    Game *self = (Game*)malloc(sizeof(Game));
    assert(self != NULL);

    //initialize SDL
    assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) >= 0);

    //create SDL self
    self->window = SDL_CreateWindow("JOOJ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    assert(self->window != NULL);

    //create renderer
    self->renderer = SDL_CreateRenderer(self->window, -1, SDL_RENDERER_ACCELERATED);
    assert(self->renderer != NULL);
    SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, 255);

    //init subsystems
    assert(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG);
    assert(TTF_Init() != -1);
    assert(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) >= 0);

    SDL_ShowCursor(SDL_DISABLE);

    SDL_GetWindowSize(self->window, &self->w, &self->h);
    printf("%d %d\n", self->w, self->h);

    self->mfocus = 1;
    self->kfocus = 1;
    self->fullscreen = 0;
    self->minimized = 0;
    
    self->saved = 0;

    //OPEN SAVE FILE
    self->score_num = 0;
    self->position = 0;

    FILE *save = fopen("./save.bin", "rb");
    if (save != NULL) {
        printf("Lendo arquivo...\n");
        fread(self->options, sizeof(bool), OPTION_TOTAL, save);
        for (int i = 0; i < 99; i++) {
            fread(&self->points_rank[i], sizeof(int), 1, save);
            fread(self->name_rank[i], sizeof(char), 31, save);
            if (self->points_rank[i] > 0) self->score_num++;
        }
    }
    else {
        printf("Arquivo não pôde ser lido\n");
        self->options[OPTION_IGNORE_RATIO] = 0;
        self->options[OPTION_INTEGER_SCALING] = 0;
        self->options[OPTION_MUTE_MUSIC] = 0;
        self->options[OPTION_MUTE_SOUND] = 0;
        self->options[OPTION_BILINEAR_FILTERING] = 0;
        for (int i = 0; i < 99; i++) {
            self->points_rank[i] = 0;
            strcpy(self->name_rank[i], "");
        }
    }

    if (self->options[OPTION_MUTE_MUSIC])
        Mix_VolumeMusic(0);
    else
        Mix_VolumeMusic(MIX_MAX_VOLUME);

    if (self->options[OPTION_MUTE_SOUND])
        Mix_Volume(-1, 0);
    else
        Mix_Volume(-1, MIX_MAX_VOLUME);

    if (self->options[OPTION_BILINEAR_FILTERING])
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    else
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    self->points[0] = 0;
    self->points[1] = 0;
    self->points[2] = 0;
    self->points[3] = 0;
    strcpy(self->name, "");

    //create target texture
    self->target = SDL_CreateTexture(self->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_W, SCREEN_H);
    assert(self->target != NULL);
    SDL_SetTextureBlendMode(self->target, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(self->target, 255);
    
    self->target_mouse = SDL_CreateTexture(self->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_W, SCREEN_H);
    assert(self->target_mouse != NULL);
    SDL_SetTextureBlendMode(self->target_mouse, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(self->target_mouse, 255);

    SDL_Surface *mouse_surface = IMG_Load("img/MouseTexture.png");
    assert(mouse_surface != NULL);
    self->mouse = SDL_CreateTextureFromSurface(self->renderer, mouse_surface);
    assert(self->mouse != NULL);
    free(mouse_surface);
    SDL_SetTextureBlendMode(self->mouse, SDL_BLENDMODE_BLEND);
    
    //SDL_SetRenderTarget(self->renderer, self->target);
    //SDL_RenderClear(self->renderer);
    SDL_SetRenderDrawBlendMode(self->renderer, SDL_BLENDMODE_BLEND);

    return self;
}

void game_close(Game *self) {
    //destroy renderer and self
    SDL_DestroyRenderer(self->renderer);
    SDL_DestroyWindow(self->window);

    //destroy textures
    SDL_DestroyTexture(self->target);
    SDL_DestroyTexture(self->target_mouse);
    SDL_DestroyTexture(self->mouse);

    //quit subsystems
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    
    SDL_ShowCursor(SDL_ENABLE);
    
    //free self struct
    free(self);
}

SDL_Renderer *game_get_renderer(Game *self) {
    return self->renderer;
}

double game_get_scalex(Game *self) {
    double scalex = (double)self->w / SCREEN_W;
    double scaley = (double)self->h / SCREEN_H;

    if (self->options[OPTION_INTEGER_SCALING]) {
        scalex = (double)(int)scalex;
        scaley = (double)(int)scaley;
    }

    if (self->options[OPTION_IGNORE_RATIO]) {
        return scalex;
    }
    return (scalex * (scalex <= scaley) + scaley * (scalex > scaley));
}

double game_get_scaley(Game *self) {
    double scalex = (double)self->w / SCREEN_W;
    double scaley = (double)self->h / SCREEN_H;
    
    if (self->options[OPTION_INTEGER_SCALING]) {
        scalex = (double)(int)scalex;
        scaley = (double)(int)scaley;
    }

    if (self->options[OPTION_IGNORE_RATIO]) {
        return scaley;
    }
    return (scalex * (scalex <= scaley) + scaley * (scalex > scaley));
}

int game_get_screenx(Game *self) {
    return (self->w - SCREEN_W * game_get_scalex(self))/2;
}

int game_get_screeny(Game *self) {
    return (self->h - SCREEN_H * game_get_scaley(self))/2;
}

void game_render(Game *self, uint8_t alpha) {
    uint32_t ticks_start = SDL_GetTicks();
    //set target texture alpha
    SDL_SetTextureAlphaMod(self->target, alpha);
    //set render target to mouse
    SDL_SetRenderTarget(self->renderer, self->target_mouse);
    //render target texture
    SDL_RenderCopy(self->renderer, self->target, NULL, NULL);
    //render mouse
    int mousex, mousey;
    SDL_GetMouseState(&mousex, &mousey);
    mousex = (mousex - game_get_screenx(self)) / game_get_scalex(self);
    mousey = (mousey - game_get_screeny(self)) / game_get_scaley(self);
    SDL_Rect mouse_rect = {mousex, mousey, 16, 16};
    SDL_RenderCopy(self->renderer, self->mouse, NULL, &mouse_rect);
    //reset render target
    SDL_SetRenderTarget(self->renderer, NULL);
    //render target texture
    SDL_Rect rect = {game_get_screenx(self), game_get_screeny(self), SCREEN_W * game_get_scalex(self), SCREEN_H * game_get_scaley(self)};
    SDL_RenderCopy(self->renderer, self->target_mouse, NULL, &rect);
    //render to screen
    SDL_RenderPresent(self->renderer);
    //clear screen
    SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, 255);
    SDL_RenderClear(self->renderer);
    uint32_t ticks_now = SDL_GetTicks();
    if (TICKS_PER_FRAME > ticks_now - ticks_start) {
        SDL_Delay(TICKS_PER_FRAME + ticks_start - ticks_now);
    }
}

void game_clear_screen(Game *self) {
    //set target_mouse texture as render target
    SDL_SetRenderTarget(self->renderer, self->target_mouse);
    //clear target texture
    SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, 255);
    SDL_RenderClear(self->renderer);
    //set target texture as render target
    SDL_SetRenderTarget(self->renderer, self->target);
    //clear target texture
    SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, 255);
    SDL_RenderClear(self->renderer);
}

void game_set_points(Game *self, Points p, int points) {
    int index = (p == POINTS_1) * 0 + (p == POINTS_2) * 1 + (p == POINTS_3) * 2 + (p == POINTS_4) * 3;
    if (points > self->points[index])
        self->points[index] = points;
}

int game_get_points(Game *self, Points p) {
    int points = 0;
    points += self->points[0] * ((p & POINTS_1) == POINTS_1);
    points += self->points[1] * ((p & POINTS_2) == POINTS_2);
    points += self->points[2] * ((p & POINTS_3) == POINTS_3);
    points += self->points[3] * ((p & POINTS_4) == POINTS_4);
    return points;
}

int game_get_rank_points(Game *self, int n) {
    if (n < 100 && n > 0) return self->points_rank[n-1];
    return 0;
}

void game_set_name(Game *self, char *name) {
    memcpy(self->name, name, 31);
}

char *game_get_name(Game *self) {
    return self->name;
}

char *game_get_rank_name(Game *self, int n) {
    if (n < 100 && n > 0) return self->name_rank[n-1];
    return "";
}

int game_get_position(Game *self) {
    return self->position;
}

void game_calc_position(Game *self) {
    int points = self->points[0] + self->points[1] + self->points[2] + self->points[3];
    if (points == 0) return;
    if (self->position == 0 && self->score_num < 99) {
        self->score_num++;
        self->position = self->score_num - 1;
    }
    else self->position--;
    printf("%d %d %s\n", self->position, self->points_rank[self->position], self->name_rank[self->position]);
    printf("%d %s\n", points, self->name);
    self->points_rank[self->position] = 0;
    strcpy(self->name_rank[self->position], "");
    self->position--;

    while (self->position >= 0 && points > self->points_rank[self->position]) {
        self->points_rank[self->position+1] = self->points_rank[self->position];
        strcpy(self->name_rank[self->position+1], self->name_rank[self->position]);
        self->position--;
    }
    self->position++;
    self->points_rank[self->position] = points;
    strcpy(self->name_rank[self->position], self->name);
    self->position++;
}

void game_reset_render_target(Game *self) {
    SDL_SetRenderTarget(self->renderer, self->target);
}

bool game_handle_event(Game *self, SDL_Event e) {
    if (e.type == SDL_WINDOWEVENT) {
        switch (e.window.event) {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            self->w = e.window.data1;
            self->h = e.window.data2;
            printf("%d %d\n", self->w, self->h);
            break;

            case SDL_WINDOWEVENT_ENTER:
            self->mfocus = true;
            break;

            case SDL_WINDOWEVENT_LEAVE:
            self->mfocus = false;
            break;

            case SDL_WINDOWEVENT_FOCUS_GAINED:
            self->kfocus = true;
            break;

            case SDL_WINDOWEVENT_FOCUS_LOST:
            self->kfocus = false;
            break;

            case SDL_WINDOWEVENT_MINIMIZED:
            self->minimized = true;
            break;

            case SDL_WINDOWEVENT_MAXIMIZED:
            self->minimized = false;
            break;

            case SDL_WINDOWEVENT_RESTORED:
            self->minimized = false;
            break;
        }
        return false;
    }
    else if (e.type == SDL_KEYDOWN && ((e.key.keysym.sym == SDLK_RETURN && SDL_GetModState() & (KMOD_LALT | KMOD_RALT)) || e.key.keysym.sym == SDLK_F11)) {
        if (self->fullscreen) {
            SDL_SetWindowFullscreen(self->window, 0);
            self->fullscreen = false;
        }
        else {
            SDL_SetWindowFullscreen(self->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            self->fullscreen = true;
        }
        return false;
    }
    else if ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) || e.type == SDL_QUIT)
        return true;
    
    return false;
}

bool game_change_option(Game *self, Option option) {
    switch (option) {
        case OPTION_IGNORE_RATIO:
        self->options[OPTION_IGNORE_RATIO] = !self->options[OPTION_IGNORE_RATIO];
        return self->options[OPTION_IGNORE_RATIO];
        case OPTION_INTEGER_SCALING:
        self->options[OPTION_INTEGER_SCALING] = !self->options[OPTION_INTEGER_SCALING];
        return self->options[OPTION_INTEGER_SCALING];
        case OPTION_MUTE_MUSIC:
        self->options[OPTION_MUTE_MUSIC] = !self->options[OPTION_MUTE_MUSIC];
        return self->options[OPTION_MUTE_MUSIC];
        case OPTION_MUTE_SOUND:
        self->options[OPTION_MUTE_SOUND] = !self->options[OPTION_MUTE_SOUND];
        return self->options[OPTION_MUTE_SOUND];
        case OPTION_BILINEAR_FILTERING:
        self->options[OPTION_BILINEAR_FILTERING] = !self->options[OPTION_BILINEAR_FILTERING];
        return self->options[OPTION_BILINEAR_FILTERING];
        default:
        return false;
    }
}

bool game_get_option(Game *self, Option option) {
    switch (option) {
        case OPTION_IGNORE_RATIO:
        return self->options[OPTION_IGNORE_RATIO];
        case OPTION_INTEGER_SCALING:
        return self->options[OPTION_INTEGER_SCALING];
        case OPTION_MUTE_MUSIC:
        return self->options[OPTION_MUTE_MUSIC];
        case OPTION_MUTE_SOUND:
        return self->options[OPTION_MUTE_SOUND];
        case OPTION_BILINEAR_FILTERING:
        return self->options[OPTION_BILINEAR_FILTERING];
        default:
        return false;
    }
}

bool game_save(Game *self, bool complete) {
    if (self->saved && !complete) return false;
    FILE *save = fopen("./save.bin", "wb");
    if (save == NULL) return false;
    fwrite(self->options, sizeof(bool), OPTION_TOTAL, save);
    for (int i = 0; i < 99; i++) {
        fwrite(&self->points_rank[i], sizeof(int), 1, save);
        fwrite(self->name_rank[i], sizeof(char), 31, save);
    }
    fclose(save);

    int points = self->points[0] + self->points[1] + self->points[2] + self->points[3];

    FILE *log = fopen("./score_log.txt", "a+");
    if (log == NULL) return false;
    if (points > 0) {
        fprintf(log, "%s: %d\n", self->name, points);
    }
    fclose(log);
    self->saved = true;
    return true;
}
