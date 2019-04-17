#include "dreamwarp.h"
#include <math.h>
#include <SDL2/SDL.h>

void init_camera() {
  camera.x = 0;
  camera.y = 0;
}

void center_camera(Entity *entity) {
  camera.x = entity->x + (entity->w / 2) - floor(Game.window_width / 2);
  camera.y = entity->y + (entity->h / 2) - floor(Game.window_height / 2);
}

void Entity_dst(SDL_Rect *dst, Entity *entity) {
    dst->x = entity->x - camera.x;
    dst->y = entity->y - camera.y;
    dst->w = entity->w;
    dst->h = entity->h;
}
