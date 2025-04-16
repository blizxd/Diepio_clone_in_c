#include <stdio.h>
#include <SDL2/SDL.h>
#include <helper.h>
#include <stdlib.h>
#include <config.h>
#include <assert.h>
#include <obstacle.h>
#include <enemy.h>
#include <game.h>

int main(int argc, char *argv[])
{

  // checkCollision();

  initGame();

  printf("Running tests\n");

  assert(checkCollision((SDL_Rect){100, 200, 30, 40}, (SDL_Rect){50, 100, 30, 40}) == 0);
  assert(checkCollision((SDL_Rect){100, 200, 30, 40}, (SDL_Rect){100, 100, 30, 40}) == 0);
  assert(checkCollision((SDL_Rect){100, 200, 30, 40}, (SDL_Rect){80, 200, 30, 40}) == 1);
  assert(checkCollision((SDL_Rect){0, 0, 60, 150}, (SDL_Rect){50, 100, 30, 40}) == 1);

  // dealDmgToObstacleAndGetXp

  assert(dealDmgToObstacleAndGetXp(&obstacleGroup.obstacles[0], 30) == 0);
  assert(dealDmgToObstacleAndGetXp(&obstacleGroup.obstacles[0], 3000) != 0);

  // dealDamageToEnemyAndGetXp

  while (EG.activeCount == 0)
    spawnRandomEnemy();

  assert(dealDamageToEnemyAndGetXp(0, 100) == 0);
  assert(dealDamageToEnemyAndGetXp(0, 100) == 0);
  assert(dealDamageToEnemyAndGetXp(0, 100) != 0);

  // dealDmgToPlayer

  dealDmgToPlayer(&player, 50);
  assert(player.isAlive == 1);
  dealDmgToPlayer(&player, 5000);
  assert(player.isAlive == 0);

  // getUpgradePoint

  updateCurrentScore(&player, 500, dealtDamageToEnemy);
  assert(player.availableUpgrades == 3);
  updateCurrentScore(&player, 10000, dealtDamageToEnemy);
  assert(player.availableUpgrades == 6);

  printf("All tests passed");

  return 0;
}