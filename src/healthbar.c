#include <healthbar.h>

void renderHealthBar(SDL_Renderer *renderer, float healtValue, SDL_Point pos, SDL_Rect camera)
{
    SDL_Texture *healthbarBaseTex = textureManager[HealthbarBase];
    SDL_Texture *healthbarFullTex = textureManager[HealthbarFull];

    SDL_Rect src, dest;

    dest.w = 80;
    dest.h = 29;
    dest.x = pos.x - 40;
    dest.y = pos.y - 20;

    renderSprite(healthbarBaseTex, dest, camera, 0, renderer);

    dest.w = dest.w * healtValue;
    src.h = 29;
    src.w = dest.w;
    src.x = 0;
    src.y = 0;

    renderSpriteWithSrc(healthbarFullTex, src, dest, camera, 0, renderer);
}