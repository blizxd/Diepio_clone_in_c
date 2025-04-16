#include <savegame.h>
#include <enemy.h>

char saveGamefileExists()
{

  FILE *file = fopen(PATH_TO_SAVEGAME, "rb");

  if (file != NULL)
  {

    fclose(file);
    return 1;
  }
  return 0;
}

void saveGamedataToFile(Player *player, ObstacleGroup *obsg)
{

  FILE *savefile = fopen(PATH_TO_SAVEGAME, "wb");

  int buffSize = (PLAYER_BUFFER) + (OBSGROUP_BUFFER) + (ENEMYGROUP_BUFFER);
  char *buffer = malloc(buffSize);
  serializePlayer(player, buffer);
  serializeObsGroup(obsg, buffer + PLAYER_BUFFER);
  serializeEnemyGroup(buffer + OBSGROUP_BUFFER + PLAYER_BUFFER);

  fwrite(buffer, 1, buffSize, savefile);

  free(buffer);
  fclose(savefile);
}

void getGamedataFromFile(Player *player, ObstacleGroup *obsg)
{
  FILE *savefile = fopen(PATH_TO_SAVEGAME, "rb");

  int buffSize = PLAYER_BUFFER + OBSGROUP_BUFFER + ENEMYGROUP_BUFFER;
  char *buffer = malloc(buffSize);

  fread(buffer, 1, buffSize, savefile);

  deserializePlayer(player, buffer);
  deserializeObsGroup(buffer + PLAYER_BUFFER, obsg, renderer);
  deserializeEnemyGroup(buffer + OBSGROUP_BUFFER + PLAYER_BUFFER);

  free(buffer);
  fclose(savefile);
}