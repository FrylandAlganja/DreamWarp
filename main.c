#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <time.h>

#include "dreamwarp.h"

#define MAX_COLLISIONS 50

const int FRAME_RATE = 60;

struct CollisionList {
    int tile_count;
    Entity *tiles[50];
} collision_list;

void draw_entity(Entity *entity, SDL_Renderer *renderer, SDL_Texture *texture,
                 SDL_Rect *dst) {
    Entity_dst(dst, entity);
    SDL_RenderCopy(renderer, texture, &sprites[entity->spr], dst);
}

int main(int argc, char ** argv)
{
  srand(time(0));
  int start_ticks;
  Game.up = Game.down = Game.left = Game.right = false;
  Game.tile_size = 48;
  int frame = 0;
  bool quit = false;
  Game.window_width = 720;
  Game.window_height = 432;
  init_sprites();
  init_camera();
  SDL_Event event;

  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG);

  SDL_Window *window = SDL_CreateWindow("Realm of Realms",
  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Game.window_width,
  Game.window_height, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_Surface *image = IMG_Load("tiles.png");
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,
  image);

  Map map = Map_createDungeon(16);
  Entity *u = Map_addBeing(&map);
  u->w = 48;
  u->h = 48,
  u->speed = 8;
  u->vx = 0;
  u->vy = 0;
  u->spr = SPR_WIZARD;
  Entity *ur_tile = Map_findVacantTile(&map);
  u->x = ur_tile->x;
  u->y = ur_tile->y;

  Entity *chick = Map_addBeing(&map);
  chick->w = 24;
  chick->h = 22;
  chick->spr = SPR_CHICKEN;
  Entity *chick_tile = Map_findVacantTile(&map);
  chick->x = chick_tile->x;
  chick->y = chick_tile->y;

  SDL_Rect dst;

  while (!quit)
  {
    start_ticks = SDL_GetTicks();
    SDL_Rect *src = &sprites[SPR_CHICKEN];
    SDL_PollEvent(&event);

    switch (event.type)
    {
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_UP:
            Game.up = true;
            break;
          case SDLK_DOWN:
            Game.down = true;
            break;
          case SDLK_LEFT:
            Game.left = true;
            break;
          case SDLK_RIGHT:
            Game.right = true;
            break;
          case SDLK_ESCAPE:
            quit = true;
            break;
        }
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
          case SDLK_UP:
            Game.up = false;
            break;
          case SDLK_DOWN:
            Game.down = false;
            break;
          case SDLK_LEFT:
            Game.left = false;
            break;
          case SDLK_RIGHT:
            Game.right = false;
            break;
          case SDLK_ESCAPE:
            quit = true;
            break;
        }
        break;
    }

    u->vx = u->vy = 0;

    if (Game.up) {
      u->vy = -u->speed;
    }
    if (Game.down) {
      u->vy = u->speed;
    }
    if (Game.left) {
      u->vx = -u->speed;
    }
    if (Game.right) {
      u->vx = u->speed;
    }

    for (int i = 0; i < map.being_count; i++) {
        collision_list.tile_count = 0;
        Entity *e = &map.beings[i];
        int tile_id = floor(e->y / Game.tile_size) *
            map.w + floor(e->x / Game.tile_size);
        Entity *e_tile = &map.tiles[tile_id];
        Entity new_e = *e;
        new_e.x = e->x + e->vx;
        new_e.y = e->y + e->vy;

        for (int y = e_tile->tile_y - 1; y <= e_tile->tile_y + 1; y++) {
            for (int x = e_tile->tile_x - 1; x <= e_tile->tile_x + 1; x++) {
                Entity *tile = &map.tiles[y * map.w + x];
                if (tile->active && tile->type == 2 && collides(tile, &new_e)) {
                    collision_list.tiles[collision_list.tile_count] = tile;
                    collision_list.tile_count++;
                }
            }
        }

        e->x = e->x + e->vx;
        for (int j = 0; j < collision_list.tile_count; j++) {
            while (collides(collision_list.tiles[j], e)) {
                if (e->vx > 0) {
                    e->x = e->x - 1;
                } else if (e->vx < 0) {
                    e->x = e->x + 1;
                } else {
                    printf("no velocity");
                    break;
                }
            }
        }
        e->y = e->y + e->vy;
        for (int j = 0; j < collision_list.tile_count; j++) {
            while (collides(collision_list.tiles[j], e)) {
                if (e->vy > 0) {
                    e->y = e->y - 1;
                } else if (e->vy < 0) {
                    e->y = e->y + 1;
                } else {
                    printf("no velocity");
                    break;
                }
            }
        }
    }
                    

    center_camera(u);

    SDL_RenderClear(renderer);
    for (int i = 0; i < (map.w * map.h); i++) {
        if (map.tiles[i].active) {
            draw_entity(&map.tiles[i], renderer, texture, &dst);
        }
    }
    draw_entity(u, renderer, texture, &dst);
    draw_entity(chick, renderer, texture, &dst);
    SDL_RenderPresent(renderer);
    int end_ticks = SDL_GetTicks() - start_ticks;
    if (end_ticks < 1000 / FRAME_RATE) {
      SDL_Delay((1000 / FRAME_RATE) - end_ticks);
    }
  }

  Map_free(&map);
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(image);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();

  return 0;
}
