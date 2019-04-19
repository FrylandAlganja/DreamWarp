#ifndef DREAMWARP_H
#define DREAMWARP_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#define SPR_GRASS 0
#define SPR_FLOOR 1
#define SPR_WALL 2
#define SPR_STAIR 4
#define SPR_WIZARD 5
#define SPR_CHICKEN 6

SDL_Rect sprites[7];
void init_sprites();

typedef struct PointStruct Point;
struct PointStruct {
    int x, y;
};

typedef struct EntityStruct Entity;

struct EntityStruct {
  int x, y;
  int tile_x, tile_y;
  int w, h;
  int tile_w, tile_h;
  int vx, vy;
  int spr;
  int type;
  bool active;
};

int bottom(Entity *entity);
int right(Entity *entity);
bool collides(Entity *a, Entity *b);

void Entity_dst(SDL_Rect *dst, Entity *entity);

struct Camera {
  int x, y;
} camera;

void init_camera();
void center_camera(Entity *entity);

typedef struct MapStruct Map;

struct MapStruct {
    int w, h;
    Entity *tiles;
};

Map create_map(int w, int h);
Map create_dungeon(int room_count);
void free_map();
void set_tile(Map *map, int x, int y, int type);
Entity *find_vacant_tile(Map *map);

struct GameStruct {
    bool up, down, left, right;
    int window_width;
    int window_height;
    int tile_size;
} Game;

#endif
