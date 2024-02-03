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
#define SPR_SWORDW1 7
#define SPR_SWORDW2 8
#define SPR_SWORDW3 9
#define SPR_GOBLIN 10
#define SPR_SEAGULL 11
#define SPR_SWORDN 12
#define SPR_SWORDS 13
#define SPR_SWORDE 14
#define SPR_SWORDW 15

#define MAX_BEINGS 500
#define MAX_ROOMS 50

#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4

#define STAND 1
#define JUMP 2
#define MOVE 3
#define ATTACK 4

void init_sprites(SDL_Rect *sprites);

typedef struct PointStruct Point;
struct PointStruct {
    int x, y;
};

typedef struct BoxStruct Box;
struct BoxStruct {
    int x, y;
    int w, h;
};

typedef struct GameStruct GameS;
struct GameStruct {
    bool up, down, left, right, attack;
    int window_width;
    int window_height;
    int tile_size;
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
  int action;
  int action_duration;
  int move_distance;
  int facing;
  void (*update)();
};

Entity Entity_create();
Entity Chicken_create();

int bottom(Entity *entity);
int right(Entity *entity);
bool collides(Entity *a, Entity *b);
Point direction_delta(int direction);

typedef struct CameraStruct Camera;
struct CameraStruct {
  int x, y;
  bool bounded;
  int x_speed;
  int y_speed;
  int transitioning;
  Entity *room;
  Entity *old_room;
};

void init_camera();
void center_camera(Camera *camera, Entity *entity, GameS *Game);
void Entity_dst(SDL_Rect *dst, Entity *entity, Camera *camera, SDL_Rect *sprites);

typedef struct MapStruct Map;

struct MapStruct {
    int w, h;
    Entity *tiles;
    int being_count;
    Entity beings[MAX_BEINGS];
    int room_count;
    Entity rooms[MAX_ROOMS];
};

void Map_digRoom(Map *map, int x, int y, int w, int h, GameS *Game);

Map Map_create(int w, int h);
Map Map_createDungeon(int room_count, GameS *Game);
Map Map_createWorld();
void Map_free();
void Map_setTile(Map *map, int x, int y, int type, GameS *Game);
Entity *Map_findVacantTile(Map *map);
Entity *Map_addBeing(Map *map);
Entity *Map_addRoom(Map *map);

#endif
