#include <enemy.h>
#include <healthbar.h>
#include <helper.h>
#include <obstacle.h>
#include <player.h>
#include <stdlib.h>

EnemyGroup EG;
int msSinceMaxEnemiesIncrease;
int currentMaxEnemies = 5;

void initEnemy(SDL_Point pos, Enemy *enemy)
{

    enemy->pos.w = 128;
    enemy->pos.h = 128;

    enemy->pos.x = pos.x + SCREEN_WIDTH / 2;
    enemy->pos.y = pos.y + SCREEN_HEIGHT / 2;

    enemy->collider = enemy->pos;

    enemy->maxHp = 250;

    enemy->hp = enemy->maxHp;
    enemy->isAlive = 1;

    enemy->angle = 0;

    enemy->msSinceStartedTracking = 0;

    enemy->texture = textureManager[EnemyImage];
}

int createEnemy(SDL_Point pos)
{

    int lastIndex = EG.activeCount - 1;

    Enemy *tempEnemy = &EG.enemies[lastIndex];

    initEnemy(pos, tempEnemy);

    if (!checkEnemyObsCollision(lastIndex) && !checkCollisionBetweenEnemies(lastIndex) &&
        !checkCollision(camera, tempEnemy->collider))
    {
        return 1;
    }
    return 0;
}

void updateEnemy(int enemyId, Player *player)
{
    if (!EG.enemies[enemyId].isAlive)
        return;

    Enemy *enemy = &EG.enemies[enemyId];

    SDL_Point playerPos = {.x = player->pos.x, .y = player->pos.y};

    if (SDL_GetTicks() - enemy->msSinceStartedTracking > ENEMY_TRACKING_INTERVAL)
    {
        enemy->msSinceStartedTracking = SDL_GetTicks();

        if (checkCollision(camera, enemy->collider))
        {
            enemyTrackToPoint(enemyId, playerPos);
        }
        else
        {
            int random = rand() % 100;
            if (random < 70)
                enemyTrackToRandomPoint(enemyId);
            else
                enemyTrackToPoint(enemyId, playerPos);
        }
    }

    moveEnemy(enemyId, player);
    enemy->angle += 1;
}

void renderEnemy(int enemyId)
{

    Enemy *enemy = &EG.enemies[enemyId];

    if (enemy->isAlive && checkCollision(camera, enemy->collider))
    {
        renderSprite(enemy->texture, enemy->pos, camera, enemy->angle, renderer);

        if (SHOW_BOX)
            renderRect(enemy->collider, camera, renderer);

        if (enemy->hp < enemy->maxHp)
        {
            SDL_Point pos = {.x = enemy->pos.x + enemy->pos.w / 2, .y = enemy->pos.y - 50};

            renderHealthBar(renderer, (float)enemy->hp / enemy->maxHp, pos, camera);
        }
    }
}

void moveEnemy(int enemyId, Player *player)
{

    Enemy *enemy = &EG.enemies[enemyId];

    enemy->pos.x += enemy->vel.x;
    enemy->collider.x = enemy->pos.x;

    if ((enemy->pos.x < SCREEN_WIDTH / 2) ||
        (enemy->pos.x + enemy->pos.w > LEVEL_WIDTH + SCREEN_WIDTH / 2) ||
        (checkEnemyObsCollision(enemyId)) || (checkPlayerCollision(enemyId, player)) ||
        (checkCollisionBetweenEnemies(enemyId)))

    {
        enemy->pos.x -= enemy->vel.x;
        enemy->collider.x = enemy->pos.x;
        enemyTrackToRandomPoint(enemyId);
    }

    enemy->pos.y += enemy->vel.y;
    enemy->collider.y = enemy->pos.y;

    if ((enemy->pos.y < SCREEN_HEIGHT / 2) ||
        (enemy->pos.y + enemy->pos.h > LEVEL_HEIGHT + SCREEN_HEIGHT / 2) ||
        (checkEnemyObsCollision(enemyId)) || (checkPlayerCollision(enemyId, player)) ||
        (checkCollisionBetweenEnemies(enemyId)))
    {
        enemy->pos.y -= enemy->vel.y;
        enemy->collider.y = enemy->pos.y;
        enemyTrackToRandomPoint(enemyId);
    }
}

int dealDamageToEnemyAndGetXp(int enemyId, int damage)
{

    if ((EG.enemies[enemyId].hp -= damage) <= 0)
    {
        EG.enemies[enemyId].isAlive = 0;

        return 1000;
    }
    return 0;
}

void destroyEnemy(int enemyId)
{

    EG.enemies[enemyId] = EG.enemies[EG.activeCount - 1];

    EG.enemies = realloc(EG.enemies, --EG.activeCount * sizeof(Enemy));
}

void enemyTrackToRandomPoint(int enemyId)
{
    SDL_Point target;

    target.x = rand() % (LEVEL_WIDTH - 200) + SCREEN_WIDTH / 2;
    target.y = rand() % (LEVEL_HEIGHT - 200) + SCREEN_HEIGHT / 2;

    enemyTrackToPoint(enemyId, target);
}

void enemyTrackToPoint(int enemyId, SDL_Point target)
{
    Vec2d dir;

    Enemy *enemy = &EG.enemies[enemyId];

    dir = (Vec2d){.x = target.x - enemy->pos.x, .y = target.y - enemy->pos.y};

    dir = normalizeVec(dir);

    enemy->vel.x = dir.x * ENEMY_SPEED;
    enemy->vel.y = dir.y * ENEMY_SPEED;
}

int checkEnemyObsCollision(int enemyId)
{

    Enemy *enemy = &EG.enemies[enemyId];

    for (int i = 0; i < obstacleGroup.activeCount; i++)
    {
        if (checkCollision(enemy->collider, obstacleGroup.obstacles[i].collider))
        {
            dealDmgToObstacleAndGetXp(&obstacleGroup.obstacles[i], 50);
            return 1;
        }
    }
    return 0;
}

int checkPlayerCollision(int enemyId, Player *player)
{
    Enemy *enemy = &EG.enemies[enemyId];

    if (checkCollision(enemy->collider, player->collider))
    {
        dealDmgToPlayer(player, ENEMY_DAMAGE);
        updateCurrentScore(player, dealDamageToEnemyAndGetXp(enemyId, 100), dealtDamageToEnemy);
        giveKnockback(player, 0.7 * enemy->vel.x, 0.7 * enemy->vel.y);
        return 1;
    }
    else
        return 0;
}

void serializeEnemy(int enemyId, char *buffer)
{
    Enemy *enemy = &EG.enemies[enemyId];

    memcpy(buffer, &enemy->pos.x, sizeof(int));
    buffer += sizeof(int);
    memcpy(buffer, &enemy->pos.y, sizeof(int));
    buffer += sizeof(int);
    memcpy(buffer, &enemy->isAlive, sizeof(char));
    buffer += sizeof(char);
    memcpy(buffer, &enemy->hp, sizeof(int));
    buffer += sizeof(int);
}

void deserializeEnemy(int enemyId, char *buffer)
{

    Enemy *enemy = &EG.enemies[enemyId];

    SDL_Point pos;

    memcpy(&pos.x, buffer, sizeof(int));
    buffer += sizeof(int);
    memcpy(&pos.y, buffer, sizeof(int));
    buffer += sizeof(int);

    pos.x -= SCREEN_WIDTH / 2;
    pos.y -= SCREEN_HEIGHT / 2;

    initEnemy(pos, enemy);
    memcpy(&enemy->isAlive, buffer, sizeof(char));
    buffer += sizeof(char);
    memcpy(&enemy->hp, buffer, sizeof(int));
    buffer += sizeof(int);
}

// Enemy group
int checkCollisionBetweenEnemies(int enemyId)
{
    Enemy *enemy = &EG.enemies[enemyId];

    for (int i = 0; i < EG.activeCount; i++)
    {
        if (EG.enemies[i].isAlive && i != enemyId)
        {
            if (checkCollision(EG.enemies[i].collider, enemy->collider))
                return 1;
        }
    }
    return 0;
}

void spawnRandomEnemy()
{

    if (SDL_GetTicks() - EG.msSinceLastRespawn > ENEMY_RESPAWN_INTERVAL)
    {
        EG.msSinceLastRespawn = SDL_GetTicks();

        if (EG.activeCount >= currentMaxEnemies)
            return;

        SDL_Point randomPos;

        EG.enemies = realloc(EG.enemies, ++EG.activeCount * sizeof(Enemy));

        do
        {
            randomPos = (SDL_Point){
                .x = rand() % (LEVEL_WIDTH - 200),
                .y = rand() % (LEVEL_HEIGHT - 200),
            };
        } while (!createEnemy(randomPos));
    }
}

void serializeEnemyGroup(char *buffer)
{

    memcpy(buffer, &EG.activeCount, sizeof(int));
    buffer += sizeof(int);
    memcpy(buffer, &currentMaxEnemies, sizeof(int));
    buffer += sizeof(int);

    for (int i = 0; i < EG.activeCount; i++)
    {
        serializeEnemy(i, buffer);
        buffer += 13;
    }
}

void deserializeEnemyGroup(char *buffer)
{

    memcpy(&EG.activeCount, buffer, sizeof(int));
    buffer += sizeof(int);

    EG.enemies = malloc(sizeof(Enemy) * EG.activeCount);

    memcpy(&currentMaxEnemies, buffer, sizeof(int));
    buffer += sizeof(int);

    for (int i = 0; i < EG.activeCount; i++)
    {
        deserializeEnemy(i, buffer);
        buffer += 13;
    }
}

void updateEnemyGroup(Player *player)
{

    spawnRandomEnemy();
    increaseMaxEnemies();

    for (int i = 0; i < EG.activeCount; i++)
    {
        updateEnemy(i, player);
    }
    for (int i = 0; i < EG.activeCount; i++)
    {
        if (!EG.enemies[i].isAlive)
            destroyEnemy(i);
    }
}

void increaseMaxEnemies()
{

    if (SDL_GetTicks() - msSinceMaxEnemiesIncrease > MAX_ENEMIES_INCREASE_INTERVAL &&
        currentMaxEnemies < MAX_ENEMIES)
    {
        currentMaxEnemies++;
        msSinceMaxEnemiesIncrease = SDL_GetTicks();
    }
}

void renderEnemyGroup()
{
    for (int i = 0; i < EG.activeCount; i++)
    {
        renderEnemy(i);
    }
}