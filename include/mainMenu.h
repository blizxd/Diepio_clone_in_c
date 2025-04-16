#ifndef MAINMENU_H
#define MAINMENU_H
#include <game.h>
#include <menuBackground.h>

int initMainMenuBackground();

int handleMenuInput(SDL_Event e);

int updateMenuBackground();

void renderMenuBackground();

int renderMenu();

extern char saveFileExists;

#endif