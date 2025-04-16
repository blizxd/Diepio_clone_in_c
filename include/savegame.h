#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <stdlib.h>
#include <stdio.h>
#include <obstacle.h>
#include <player.h>

// extern FILE *savefile;

// void openFile();

// void closeFile();

char saveGamefileExists();

void saveGamedataToFile(Player *player, ObstacleGroup *obsg);

void getGamedataFromFile(Player *player, ObstacleGroup *obsg);

#endif