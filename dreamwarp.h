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

#define MAX_BEINGS 100
#define MAX_ROOMS 50

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
  int speed;
  int spr;
  int type;
  bool active;
};

Entity Entity_create();

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
    int being_count;
    Entity beings[MAX_BEINGS];
    int room_count;
    Entity rooms[MAX_ROOMS];
};

Map Map_create(int w, int h);
Map Map_createDungeon(int room_count);
void Map_free();
void Map_setTile(Map *map, int x, int y, int type);
Entity *Map_findVacantTile(Map *map);
Entity *Map_addBeing(Map *map);
Entity *Map_addRoom(Map *map);

struct GameStruct {
    bool up, down, left, right;
    int window_width;
    int window_height;
    int tile_size;
} Game;

#endif
