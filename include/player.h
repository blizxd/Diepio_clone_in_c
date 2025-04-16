#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <config.h>
#include <game.h>
#include <healthbar.h>
#include <helper.h>
#include <obstacle.h>
#include <projectile.h>

typedef struct Player
{
    SDL_Texture *texture;

    SDL_Rect pos;
    int speed; // unused

    Vec2d vel;

    char isInKnockback;
    Vec2d knockback;
    int knockbackSpeed;
    int msSinceKnockbackEffect;

    int hp, maxHp;
    int msSinceTakenDamage;
    char isAlive;

    double angle;

    SDL_Rect collider;

    ProjectileGroup projGroup;
    char isShooting;
    int msSinceLastShot;

    int currentScore;
    int killCount;
    int availableUpgrades;

    int xpSinceLastUpgradePoint;

    float bulletDamageMult;
    float speedMultiplier;
    float hpMultiplier;

    int reloadInterval;

    int speedLevel, bulletDamageLevel, healthLevel, reloadSpeedLevel;

} Player;

extern Player player;

enum Upgrades
{
    speedUpgrade,
    maxHpUpgrade,
    bulletDamageUpgrade,
    reloadUpgrade
};

void serializePlayer(Player *player, char *buffer);

void deserializePlayer(Player *player, char *buffer);

void rotatePlayerAtTarget(Player *player, SDL_Point target, SDL_Rect camera);

void movePlayer(Player *player);

void createPlayer(Player *player, SDL_Renderer *renderer, SDL_Point pos);

void handlePlayerInput(Player *player, SDL_Event e);

void renderPlayer(Player *player, SDL_Rect camera, SDL_Renderer *renderer);

int checkObstacleCollisions(Player *player);

void updatePlayer(Player *player, SDL_Rect camera, SDL_Renderer *renderer);

void playerShootProjectile(Player *player, SDL_Renderer *renderer);

void giveKnockback(Player *player, float x_speed, float y_speed);

void updateKnockback(Player *player);

void dealDmgToPlayer(Player *player, int damage);

void regenerateHp(Player *player);

void choosePlayerUpgrade(Player *player, enum Upgrades type);

void updateCurrentScore(Player *player, int score, enum XpEvent xpEvent);

void getUpgradePoint(Player *player);

#endif