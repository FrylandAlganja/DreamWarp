#include "dreamwarp.h"

Map Map_create(int w, int h) {
    Map map;
    map.w = w;
    map.h = h;
    map.tiles = malloc(sizeof(Entity) * (w * h));
    Entity tile;
    tile.active = false;
    for (int i = 0; i < w * h; i++) {
        map.tiles[i] = tile;
    }
    map.being_count = 0;
    map.room_count = 0;
    return map;
};

Entity *Map_addBeing(Map *map) {
    Entity being = Entity_create();
    map->being_count++;
    map->beings[map->being_count - 1] = being;

    return &map->beings[map->being_count - 1];
}

Entity *Map_addRoom(Map *map) {
    Entity room = Entity_create();
    map->room_count++;
    map->rooms[map->room_count - 1] = room;
    return &map->rooms[map->room_count - 1];
}

void Map_free(Map *map) {
    free(map->tiles);
}

void Map_setTile(Map *map, int x, int y, int type) {
    Entity tile;
    tile.x = x * Game.tile_size;
    tile.y = y * Game.tile_size;
    tile.w = Game.tile_size;
    tile.h = Game.tile_size;
    tile.tile_x = x;
    tile.tile_y = y;
    tile.type = type;
    tile.active = true;
    tile.spr = SPR_FLOOR;
    switch (type) {
        case 1:
            tile.spr = SPR_FLOOR;
            break;
        case 2:
            tile.spr = SPR_WALL;
            break;
    }
    map->tiles[(y * map->w) + x] = tile;
}

Entity *Map_findVacantTile(Map *map) {
    Entity *tile;
    bool tile_found = false;
    int x = 0;
    int y = 0;

    while (!tile_found) {
        int x = rand() % map->w;
        int y = rand() % map->h;
        int tile_id = y * map->w + x;
        tile = &(map->tiles[tile_id]);

        if (tile->active && tile->type == 1) {
            tile_found = true;
        }
    }

    return tile;
}
