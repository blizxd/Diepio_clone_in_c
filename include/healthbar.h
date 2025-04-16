#ifndef HEALTHBAR_H
#define HEALTHBAR_H
#include <SDL2/SDL.h>
#include <helper.h>

void renderHealthBar(SDL_Renderer *renderer, float healtValue, SDL_Point pos, SDL_Rect camera);

#endif