#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <stdio.h>
#include <game.h>
#include <helper.h>

void writeScoreToFile();

void readScoreFromFile();

void switchToScoreboard();

void displayScoreboard();

void handleScoreBoardInput(SDL_Event e);

void sortScoreList();

#endif