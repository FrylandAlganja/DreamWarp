#include "dreamwarp.h"
#include <math.h>
#include <SDL2/SDL.h>

void init_camera(Camera *camera) {
  camera->x = 0;
  camera->y = 0;
  camera->x_speed = 32;
  camera->y_speed = 24;
  camera->bounded = false;
  camera->transitioning = 0;
}

void center_camera(Camera *camera, Entity *entity, GameS *Game) {
  if (!camera->transitioning) {
      camera->x = entity->x + (entity->w / 2) - floor(Game->window_width / 2);
      camera->y = entity->y + (entity->h / 2) - floor(Game->window_height / 2);
  }

  if (camera->bounded == true) {
      if (camera->transitioning == NORTH) {
          if (camera->y <= camera->old_room->y - Game->window_height + camera->y_speed) {
              camera->y = entity->y + (entity->h / 2) - floor(Game->window_height / 2);
              camera->transitioning = 0;
          } else {
              camera->y -= camera->y_speed;
          }
      } else if (camera->transitioning == EAST) {
          if (camera->x >= right(camera->old_room) - camera->x_speed) {
              camera->x = entity->x + (entity->w / 2) - floor(Game->window_width / 2);
              camera->transitioning = 0;
          } else {
              camera->x += camera->x_speed;
          }
      } else if (camera->transitioning == SOUTH) {
          if (camera->y >= bottom(camera->old_room) - camera->y_speed) {
              camera->y = entity->y + (entity->h / 2) - floor(Game->window_height / 2);
              camera->transitioning = 0;
          } else {
              camera->y += camera->y_speed;
          }
      } else if (camera->transitioning == WEST) {
          if (camera->x <= camera->old_room->x - Game->window_width + camera->x_speed) {
              camera->x = entity->x + (entity->w / 2) - floor(Game->window_width / 2);
              camera->transitioning = 0;
          } else {
              camera->x -= camera->x_speed;
          }
      }
      if (!camera->transitioning) {
          if (camera->x < camera->room->x) {
              camera->x = camera->room->x;
          }
          if (camera->y < camera->room->y) {
              camera->y = camera->room->y;
          }
          if (camera->x > right(camera->room) - Game->window_width) {
              camera->x = right(camera->room) - Game->window_width;
          }
          if (camera->y > bottom(camera->room) - Game->window_height) {
              camera->y = bottom(camera->room) - Game->window_height;
          }
      }
  }
}

void Entity_dst(SDL_Rect *dst, Entity *entity, Camera *camera, SDL_Rect *sprites) {
    dst->x = entity->x - camera->x;
    dst->y = entity->y - camera->y;
    dst->w = sprites[entity->spr].w;
    dst->h = sprites[entity->spr].h;
}
