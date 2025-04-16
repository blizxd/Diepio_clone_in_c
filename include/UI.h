#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <player.h>
#include <helper.h>
#include <savegame.h>

void displayScore(SDL_Renderer *renderer, Player *player);

void displayKillCount(Player *player);

void setMessage(char *str);

void displayMessage();

void displayAvailableUpgrades(SDL_Renderer *renderer, Player *player);

void handleUIInput(SDL_Event e, Player *player);

extern int msSinceSavegameMessage;
#endif