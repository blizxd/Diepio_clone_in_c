#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <config.h>

enum Scenes
{
  GAME,
  MAIN_MENU,
  SCOREBOARD,
};

int initGame();

int RestartGameLogic();

int loadResources();

int runGame();

void shutdownGame();

void createNewGame();

void loadGameFromFile();

void switchToMainMenu();

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern char isRunning;
extern SDL_Rect camera;
extern enum Scenes currentScene;

#endif