#include <SDL2/SDL.h>
#include <helper.h>
#include <player.h>

typedef struct Enemy
{

  SDL_Texture *texture;

  SDL_Rect pos;

  Vec2d vel;

  int hp, maxHp;
  char isAlive;

  SDL_Rect collider;

  SDL_Point targetPoint;

  int msSinceStartedTracking;

  double angle;

} Enemy;
typedef struct EnemyGroup
{

  Enemy *enemies;
  int activeCount;

  int msSinceLastRespawn;

} EnemyGroup;

extern EnemyGroup EG;
extern int currentMaxEnemies;
extern int msSinceMaxEnemiesIncrease;

// Enemy
void initEnemy(SDL_Point pos, Enemy *enemy);

int createEnemy(SDL_Point pos);

void updateEnemy(int enemyId, Player *player);

void renderEnemy(int enemyId);

void moveEnemy(int enemyId, Player *player);

int dealDamageToEnemyAndGetXp(int enemyId, int damage);

void destroyEnemy(int enemyId);

void enemyTrackToRandomPoint(int enemyId);

void enemyTrackToPoint(int enemyId, SDL_Point target);

int checkEnemyObsCollision(int enemyId);

int checkCollisionBetweenEnemies(int enemyId);

int checkPlayerCollision(int enemyId, Player *player);

void serializeEnemy(int enemyId, char *buffer);

void deserializeEnemy(int enemyId, char *buffer);

// Enemy group

void spawnRandomEnemy();

void serializeEnemyGroup(char *buffer);

void deserializeEnemyGroup(char *buffer);

void updateEnemyGroup(Player *player);

void renderEnemyGroup();

void increaseMaxEnemies();