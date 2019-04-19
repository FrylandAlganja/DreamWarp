#include "dreamwarp.h"

Map create_map(int w, int h) {
    Map map;
    map.w = w;
    map.h = h;
    map.tiles = malloc(sizeof(Entity) * (w * h));
    Entity tile;
    tile.active = false;
    for (int i = 0; i < w * h; i++) {
        map.tiles[i] = tile;
    }
    return map;
};

void free_map(Map *map) {
    free(map->tiles);
}

void set_tile(Map *map, int x, int y, int type) {
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

Entity *find_vacant_tile(Map *map) {
    Entity *tile;
    bool tile_found = false;
    int x = 0;
    int y = 0;

    while (!tile_found) {
        int x = rand() % map->w;
        int y = rand() % map->h;
        tile = &(map->tiles[y * map->w + x]);

        if (tile->active && tile->type == 1) {
            tile_found = true;
        }
    }

    return tile;
}
