#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "dreamwarp.h"

const int FRAME_RATE = 60;

void draw_entity(Entity *entity, SDL_Renderer *renderer, SDL_Texture *texture,
                 SDL_Rect *dst) {
    Entity_dst(dst, entity);
    SDL_RenderCopy(renderer, texture, &sprites[entity->spr], dst);
}

int main(int argc, char ** argv)
{
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

  Map map = create_map(10, 10);
  for (int y = 0; y < map.h; y++) {
      for (int x = 0; x < map.w; x++) {
          if (x == 0 || y == 0 || x == map.w - 1 || y == map.h - 1) {
              set_tile(&map, x, y, 2);
          } else {
              set_tile(&map, x, y, 1);
          }
      }
  }

  Entity u = {.x = 0, .y = 0,
              .w = 48, .h = 48,
              .vx = 8, .vy = 8,
              .spr = SPR_WIZARD};
  Entity chick = {.x = 10, .y = 10, .w = 24, .h = 22, .spr = SPR_CHICKEN};

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

    if (Game.up) {
      u.y -= u.vy;
    }
    if (Game.down) {
      u.y += u.vy;
    }
    if (Game.left) {
      u.x -= u.vx;
    }
    if (Game.right) {
      u.x += u.vx;
    }
    center_camera(&u);

    SDL_RenderClear(renderer);
    for (int i = 0; i < (map.w * map.h); i++) {
        draw_entity(&(map.tiles[i]), renderer, texture, &dst);
    }
    draw_entity(&u, renderer, texture, &dst);
    draw_entity(&chick, renderer, texture, &dst);
    SDL_RenderPresent(renderer);
    int end_ticks = SDL_GetTicks() - start_ticks;
    if (end_ticks < 1000 / FRAME_RATE) {
      SDL_Delay((1000 / FRAME_RATE) - end_ticks);
    }
  }

  free_map(&map);
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(image);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();

  return 0;
}
