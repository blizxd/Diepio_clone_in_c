#include <UI.h>
#include <obstacle.h>
#include <player.h>
#include <scoreboard.h>
#include <string.h>
SDL_Point mousePos;

void serializePlayer(Player *player, char *buffer)
{

    memcpy(buffer, &player->pos.x, sizeof(int));
    buffer += sizeof(int);

    memcpy(buffer, &player->pos.y, sizeof(int));
    buffer += sizeof(int);

    memcpy(buffer, &player->isAlive, sizeof(char));
    buffer += sizeof(char);

    memcpy(buffer, &player->hp, sizeof(int));
    buffer += sizeof(int);

    memcpy(buffer, &player->currentScore, sizeof(int));
    buffer += sizeof(int);

    memcpy(buffer, &player->availableUpgrades, sizeof(int));
    buffer += sizeof(int);

    memcpy(buffer, &player->xpSinceLastUpgradePoint, sizeof(int));
    buffer += sizeof(int);

    memcpy(buffer, &player->bulletDamageMult, sizeof(float));
    buffer += sizeof(float);

    memcpy(buffer, &player->speedMultiplier, sizeof(float));
    buffer += sizeof(float);

    memcpy(buffer, &player->maxHp, sizeof(int));
    buffer += sizeof(float);

    memcpy(buffer, &player->speedLevel, sizeof(int));
    buffer += sizeof(int);

    memcpy(buffer, &player->healthLevel, sizeof(int));
    buffer += sizeof(int);

    memcpy(buffer, &player->bulletDamageLevel, sizeof(int));
    buffer += sizeof(int);

    memcpy(buffer, &player->killCount, sizeof(int));
    buffer += sizeof(int);

    memcpy(buffer, &player->reloadInterval, sizeof(int));
    buffer += sizeof(int);

    memcpy(buffer, &player->reloadSpeedLevel, sizeof(int));
    buffer += sizeof(int);
}

void deserializePlayer(Player *player, char *buffer)
{

    SDL_Point pos;

    memcpy(&pos.x, buffer, sizeof(int));
    buffer += sizeof(int);
    memcpy(&pos.y, buffer, sizeof(int));
    buffer += sizeof(int);

    pos.x -= SCREEN_WIDTH / 2;
    pos.y -= SCREEN_HEIGHT / 2;
    createPlayer(player, renderer, pos);

    memcpy(&player->isAlive, buffer, sizeof(char));
    buffer += sizeof(char);

    memcpy(&player->hp, buffer, 4);
    buffer += sizeof(int);

    memcpy(&player->currentScore, buffer, 4);
    buffer += sizeof(int);

    memcpy(&player->availableUpgrades, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy(&player->xpSinceLastUpgradePoint, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy(&player->bulletDamageMult, buffer, sizeof(float));
    buffer += sizeof(float);

    memcpy(&player->speedMultiplier, buffer, sizeof(float));
    buffer += sizeof(float);

    memcpy(&player->maxHp, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy(&player->speedLevel, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy(&player->healthLevel, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy(&player->bulletDamageLevel, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy(&player->killCount, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy(&player->reloadInterval, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy(&player->reloadSpeedLevel, buffer, sizeof(int));
    buffer += sizeof(int);
}

void rotatePlayerAtTarget(Player *player, SDL_Point target, SDL_Rect camera)
{
    SDL_Point src;
    src.x = player->pos.x - camera.x + player->pos.w / 2;
    src.y = player->pos.y - camera.y + player->pos.h / 2;
    player->angle = getAngle(target, src);
}

void movePlayer(Player *player)
{

    player->vel = normalizeVec(player->vel);

    Vec2d movement;

    movement.x = player->vel.x * PLAYER_VEL * player->speedMultiplier + player->knockback.x;
    movement.y = player->vel.y * PLAYER_VEL * player->speedMultiplier + player->knockback.y;

    player->pos.x += (int)movement.x;
    player->collider.x = player->pos.x;

    if ((player->pos.x < SCREEN_WIDTH / 2) ||
        (player->pos.x + player->pos.w > LEVEL_WIDTH + SCREEN_WIDTH / 2) ||
        (checkObstacleCollisions(player)))
    {
        player->pos.x -= (int)movement.x;
        player->collider.x = player->pos.x;
        player->vel.x = 0;
        giveKnockback(player, -0.7 * movement.x, 0);
    }

    player->pos.y += (int)movement.y;
    player->collider.y = player->pos.y;

    if ((player->pos.y < SCREEN_HEIGHT / 2 - 1) ||
        (player->pos.y + player->pos.h > LEVEL_HEIGHT + SCREEN_HEIGHT / 2 - 1) ||
        (checkObstacleCollisions(player)))
    {
        player->pos.y -= (int)movement.y;
        player->collider.y = player->pos.y;
        player->vel.y = 0;

        giveKnockback(player, 0, -0.7 * movement.y);
    }
}

void createPlayer(Player *player, SDL_Renderer *renderer, SDL_Point pos)
{

    player->speed = PLAYER_VEL;

    player->pos.w = 128;
    player->pos.h = 128;

    player->pos.x = pos.x + SCREEN_WIDTH / 2;
    player->pos.y = pos.y + SCREEN_HEIGHT / 2;

    player->collider.x = player->pos.x;
    player->collider.y = player->pos.y;

    player->collider.w = 128;
    player->collider.h = 128;

    player->maxHp = 800;
    player->hp = player->maxHp;

    player->currentScore = 0;
    player->availableUpgrades = 3;

    player->texture = textureManager[PlayerImage];

    player->isAlive = 1;

    player->vel = (Vec2d){0, 0};
    player->knockback = (Vec2d){0, 0};
    player->isInKnockback = 0;

    player->projGroup.owner = player;

    player->reloadInterval = 400;

    player->bulletDamageMult = 1;
    player->speedMultiplier = 1;
}

void handlePlayerInput(Player *player, SDL_Event e)
{

    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_a:
            player->vel.x = -1;
            break;
        case SDLK_d:
            player->vel.x = 1;
            break;
        case SDLK_w:
            player->vel.y = -1;
            break;
        case SDLK_s:
            player->vel.y = 1;
            break;
        case SDLK_SPACE:
            player->isShooting = 1;
            break;
        }
    }
    else if (e.type == SDL_KEYUP)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_a:
            if (player->vel.x < 0)
                player->vel.x = 0;
            break;
        case SDLK_d:
            if (player->vel.x > 0)
                player->vel.x = 0;
            break;
        case SDLK_w:
            if (player->vel.y < 0)
                player->vel.y = 0;
            break;
        case SDLK_s:
            if (player->vel.y > 0)
                player->vel.y = 0;
            break;
        }
    }
    else if (e.type == SDL_MOUSEMOTION)
    {
        SDL_GetMouseState(&mousePos.x, &mousePos.y);
    }
}

void renderPlayer(Player *player, SDL_Rect camera, SDL_Renderer *renderer)
{
    renderSprite(player->texture, player->pos, camera, player->angle, renderer);

    if (SHOW_BOX)
        renderRect(player->collider, camera, renderer);
    if (player->hp != player->maxHp)
        renderHealthBar(renderer, (float)player->hp / player->maxHp,
                        (SDL_Point){player->pos.x + player->pos.w / 2, player->pos.y}, camera);

    // renderHealthBar(renderer, player->hp, (SDL_Point){player->pos.x, player->pos.y});

    // Vec2d dir;
    //
    // dir.x = 20 * sin(player->angle * (M_PI / 180)) + player->pos.x + player->pos.w / 2;
    // dir.y = -20 * cos(player->angle * (M_PI / 180)) + player->pos.y + player->pos.h / 2;
    //
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // SDL_RenderDrawLine(renderer, player->pos.x + player->pos.w / 2 - camera.x,
    //                    player->pos.y + player->pos.h / 2 - camera.y,
    //                    dir.x - camera.x,
    //                    dir.y - camera.y);
    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void playerShootProjectile(Player *player, SDL_Renderer *renderer)
{
    player->isShooting = 0;
    if (SDL_GetTicks() - player->msSinceLastShot < player->reloadInterval)
        return;

    Vec2d startVel;
    startVel.x = sin(player->angle * (M_PI / 180)) * PROJ_SPEED + player->vel.x * PLAYER_VEL;
    startVel.y = -cos(player->angle * (M_PI / 180)) * PROJ_SPEED + player->vel.y * PLAYER_VEL;

    SDL_Point startPos = {
        player->pos.x + player->pos.w / 2 + sin(player->angle * (M_PI / 180)) * 64,
        player->pos.y + player->pos.h / 2 - cos(player->angle * (M_PI / 180)) * 64};

    if (spawnProjectile(&player->projGroup, renderer, startPos, startVel))
    {
        player->msSinceLastShot = SDL_GetTicks();

        giveKnockback(player, -sin(player->angle * (M_PI / 180)) * PROJ_SPEED * (1.0f / 10),
                      cos(player->angle * (M_PI / 180)) * PROJ_SPEED * (1.0f / 10));
    };
}

void updatePlayer(Player *player, SDL_Rect camera, SDL_Renderer *renderer)
{
    if (!player->isAlive)
    {
        writeScoreToFile();
        switchToMainMenu();
        return;
    }

    movePlayer(player);
    rotatePlayerAtTarget(player, mousePos, camera);

    if (player->isShooting)
    {
        playerShootProjectile(player, renderer);
    }
    if (player->isInKnockback)
    {
        updateKnockback(player);
    }

    regenerateHp(player);

    getUpgradePoint(player);
}

void updateKnockback(Player *player)
{

    if (SDL_GetTicks() - player->msSinceKnockbackEffect > KNOCKBACK_DURATION)
    {
        player->isInKnockback = 0;
        player->knockback.x = 0;
        player->knockback.y = 0;
    }
}

void giveKnockback(Player *player, float x_speed, float y_speed)
{
    player->isInKnockback = 1;

    player->knockback.x = x_speed;

    player->knockback.y = y_speed;

    player->msSinceKnockbackEffect = SDL_GetTicks();
}

int checkObstacleCollisions(Player *player)
{

    for (int i = 0; i < obstacleGroup.activeCount; i++)
    {

        if (checkCollision(player->collider, obstacleGroup.obstacles[i].collider))
        {
            updateCurrentScore(player, dealDmgToObstacleAndGetXp(&obstacleGroup.obstacles[i], 20),
                               dealtDamageToObstacle);
            dealDmgToPlayer(player, 40);
            return 1;
        }
    }

    return 0;
}

void dealDmgToPlayer(Player *player, int damage)
{
    player->msSinceTakenDamage = SDL_GetTicks();
    player->hp -= damage;
    if (player->hp <= 0)
    {
        player->hp = 0;
        player->isAlive = 0;
    }
}

void choosePlayerUpgrade(Player *player, enum Upgrades type)
{
    if (player->availableUpgrades < 1)
        return;

    switch (type)
    {
    case speedUpgrade:
        if (player->speedLevel >= 10)
        {
            setMessage("Max level of speed reached");
            break;
        }
        player->speedMultiplier += 0.05;
        player->speedLevel++;
        player->availableUpgrades--;
        break;

    case maxHpUpgrade:
        if (player->healthLevel >= 10)
        {
            setMessage("Max level of hp reached");
            break;
        }
        player->maxHp = player->maxHp * 1.15;
        player->hp = player->maxHp;
        player->healthLevel++;
        player->availableUpgrades--;
        break;

    case bulletDamageUpgrade:
        if (player->bulletDamageLevel >= 10)
        {
            setMessage("Max level of bullet damage reached");
            break;
        }
        player->bulletDamageMult += 0.1;
        player->bulletDamageLevel++;
        player->availableUpgrades--;
        break;

    case reloadUpgrade:
        if (player->reloadSpeedLevel >= 10)
        {
            setMessage("Max level of reload reached");
            break;
        }
        player->reloadSpeedLevel++;
        player->reloadInterval = 400 - player->reloadSpeedLevel * 30;
        player->availableUpgrades--;
        break;
    }
}

void regenerateHp(Player *player)
{

    if (SDL_GetTicks() - player->msSinceTakenDamage > HP_REGEN_COOLDOWN &&
        player->hp < player->maxHp)
    {
        player->hp += 2;
        if (player->hp > player->maxHp)
            player->hp = player->maxHp;
    }
}

void updateCurrentScore(Player *player, int score, enum XpEvent xpEvent)
{
    player->xpSinceLastUpgradePoint += score;

    getUpgradePoint(player);

    player->currentScore += score;

    if (score != 0 && xpEvent == dealtDamageToEnemy)
        player->killCount++;
};

void getUpgradePoint(Player *player)
{

    int total_upgrades = player->bulletDamageLevel + player->speedLevel + player->reloadSpeedLevel +
                         player->healthLevel + player->availableUpgrades;

    while (player->xpSinceLastUpgradePoint >= 3000 && total_upgrades < 40)
    {
        player->availableUpgrades += 1;

        player->xpSinceLastUpgradePoint -= 3000;

        total_upgrades += 1;
    }

    if (total_upgrades >= 40)
    {
        setMessage("Max upgrade count reached");
        return;
    }
}
