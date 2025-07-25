#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <SDL_image.h>
#include <helper.h>
#include <game.h>

extern SDL_Texture *normalTile;
extern SDL_Texture *borderTile;

void initMap(SDL_Renderer *renderer);

void drawMap(SDL_Renderer *renderer, SDL_Rect *camera);

#endif