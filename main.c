#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <time.h>

#include "dreamwarp.h"
#include "libbmp.h"

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

void set_pixel(SDL_Surface* surface, int x, int y, int r, int g, int b) {
    unsigned char* pixels = (unsigned char*)surface->pixels;
    pixels[4 * (y * surface->w + x) + 0] = b;
    pixels[4 * (y * surface->w + x) + 1] = g;
    pixels[4 * (y * surface->w + x) + 2] = r;
    pixels[4 * (y * surface->w + x) + 3] = 255;
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

  Map map = Map_createDungeon(20);
  bmp_img img;
  bmp_img_init_df(&img, map.w * 2, map.h * 2);
  
  for (int y = 0; y < map.h; y++) {
      for (int x = 0; x < map.w; x++) {
          int r, g, b;
          Entity *tile = &map.tiles[y * map.w + x];
          if (tile->active) {
              if (tile->type == 1) {
                  r = g = b = 193;
              } else if (tile->type == 2) {
                  r = 22;
                  g = 40;
                  b = 81;
              }
          } else {
              r = g = b = 0;
          }
          bmp_pixel_init(&img.img_pixels[y * 2][x * 2], r, g, b);
          bmp_pixel_init(&img.img_pixels[y * 2][x * 2 + 1], r, g, b);
          bmp_pixel_init(&img.img_pixels[y * 2 + 1][x * 2], r, g, b);
          bmp_pixel_init(&img.img_pixels[y * 2 + 1][x * 2 + 1], r, g, b);
      }
  }
  bmp_img_write(&img, "test.bmp");
  bmp_img_free(&img);


  SDL_Surface *minimap_image = IMG_Load("test.bmp");
  SDL_Texture *minimap_texture = SDL_CreateTextureFromSurface(renderer, minimap_image);

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


  for (int i = 0; i < map.room_count; i++) {
      if (collides(u, &map.rooms[i])) {
          camera.bounded = true;
          camera.room = &map.rooms[i];
      }
  }

  for (int i = 0; i < 100; i++) {
      Entity *chick = Map_addBeing(&map);
      *chick = Chicken_create();
      Entity *chick_tile = Map_findVacantTile(&map);
      chick->x = chick_tile->x;
      chick->y = chick_tile->y;
  }

  SDL_Rect dst;
  SDL_Rect minimap_dst;
  minimap_dst.x = Game.window_width - (map.w * 2);
  minimap_dst.y = Game.window_height - (map.h * 2);
  minimap_dst.w = map.w * 2;
  minimap_dst.h = map.h * 2;
  

  while (!quit)
  {
    start_ticks = SDL_GetTicks();
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
        map.beings[i].update(&map.beings[i]);
        if (camera.transitioning) {
            break;
        }
        Entity *e = &map.beings[i];
        int tile_id = floor(e->y / Game.tile_size) *
            map.w + floor(e->x / Game.tile_size);
        Entity *e_tile = &map.tiles[tile_id];
        Entity new_e = *e;

        collision_list.tile_count = 0;
        new_e.x = e->x + e->vx;
        new_e.y = e->y + e->vy;

        for (int y = e_tile->tile_y - 2; y <= e_tile->tile_y + 2; y++) {
            for (int x = e_tile->tile_x - 2; x <= e_tile->tile_x + 2; x++) {
                if (y < 0 || x < 0 || y >= map.h || x >= map.w) {
                    continue;
                }

                Entity *tile = &map.tiles[y * map.w + x];
                if (!tile->active) {
                    continue;
                }
                if (tile->type == 2 && collides(&new_e, tile)) {
                    collision_list.tiles[collision_list.tile_count] = tile;
                    collision_list.tile_count++;
                }
            }
        }
        
        if (e->vx != 0) {
            e->x = e->x + e->vx;
            for (int j = 0; j < collision_list.tile_count; j++) {
                while (collides(e, collision_list.tiles[j])) {
                    if (e->vx > 0) {
                        e->x = e->x - 1;
                    } else if (e->vx < 0) {
                        e->x = e->x + 1;
                    }
                }
            }
        }

        if (e->vy != 0) {
            e->y = e->y + e->vy;
            for (int j = 0; j < collision_list.tile_count; j++) {
                while (collides(e, collision_list.tiles[j])) {
                    if (e->vy > 0) {
                        e->y = e->y - 1;
                    } else if (e->vy < 0) {
                        e->y = e->y + 1;
                    }
                }
            }
        }
    }
    
    if (!camera.transitioning) {
        for (int i = 0; i < map.room_count; i++) {
            Entity *new_room = &map.rooms[i];
            if (camera.room != new_room
                && collides(u, new_room)) {
                //camera.room = new_room;
                if (bottom(u) > bottom(new_room) && u->vy < 0) {
                    camera.transitioning = NORTH;
                    u->y = bottom(new_room) - u->h;
                } else if (u->x < new_room->x && u->vx > 0) {
                    camera.transitioning = EAST;
                    u->x = new_room->x;
                } else if (u->y < new_room->y && u->vy > 0) {
                    camera.transitioning = SOUTH;
                    u->y = new_room->y;
                } else if (right(u) > right(new_room) && u->vx < 0) {
                    camera.transitioning = WEST;
                    u->x = right(new_room) - u->w;
                }
                if (camera.transitioning) {
                    camera.old_room = camera.room;
                    camera.room = new_room;
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
    for (int i = 0; i < map.being_count; i++) {
        draw_entity(&map.beings[i], renderer, texture, &dst);
    }
    SDL_RenderCopy(renderer, minimap_texture, NULL, &minimap_dst);
    dst.x = floor(u->x / 24) + minimap_dst.x;
    dst.y = floor(u->y / 24) + minimap_dst.y;
    dst.w = 2;
    dst.h = 2;
    SDL_RenderCopy(renderer, texture, &sprites[1], &dst);
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
