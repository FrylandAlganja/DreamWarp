#include "dreamwarp.h"

typedef struct DungeonValuesStruct DungeonValues;

struct DungeonValuesStruct {
    int room_width;
    int room_height;
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    int max_rooms;
    int room_count;
    Entity rooms[20];
};

void add_room(DungeonValues *vals, Entity room) {
    vals->room_count++;
    vals->rooms[vals->room_count - 1] = room;
}

void update_min_max(DungeonValues *vals, Entity *new_room) {
    if (new_room->x < vals->min_x) {
        vals->min_x = new_room->x;
    }
    if (new_room->y < vals->min_y) {
        vals->min_y = new_room->y;
    }
    if (right(new_room) > vals->max_x) {
        vals->max_x = right(new_room);
    }
    if (bottom(new_room) > vals->max_y) {
        vals->max_y = bottom(new_room);
    }
}

void dig_room(Map *map, int x, int y, int w, int h) {
    for (int j = y; j < y + h; j++) {
        for (int i = x; i < x + w; i++) {
            printf("%d, %d, %d, %d\n", i, j, map->w, map->h);
            if (j == y || i == x || j == y + h - 1 || i == x + w - 1) {
                set_tile(map, i, j, 2);
            } else {
                set_tile(map, i, j, 1);
            }
        }
    }
}

Map create_dungeon(int max_rooms) {
    DungeonValues vals;
    vals.room_width = 15;
    vals.room_height = 9;
    vals.min_x = 0;
    vals.min_y = 0;
    vals.max_x = 0;
    vals.max_y = 0;
    vals.max_rooms = max_rooms;
    vals.room_count = 0;

    for (int i = 0; i < vals.max_rooms; i++) {
        Entity room;
        int tries = 0;
        int max_tries = 20;
        bool success = false;
        while (tries < max_tries) {
            room.w = vals.room_width * ((rand() % 2) + 1);
            room.h = vals.room_height * ((rand() % 2) + 1);
            if (i == 0) {
                room.x = 0;
                room.y = 0;
                update_min_max(&vals, &room);
                add_room(&vals, room);
                break;
            }

            Entity *parent_room = &vals.rooms[rand() % i];
            int chance = rand() % 4;
            if (chance == 0) {
                room.x = parent_room->x;
                room.y = parent_room->y - room.h;
            } else if (chance == 1) {
                room.x = right(parent_room) + 1;
                room.y = parent_room->y;
            } else if (chance == 2) {
                room.x = parent_room->x;
                room.y = bottom(parent_room) + 1;
            } else if (chance == 3) {
                room.x = parent_room->x - room.w;
                room.y = parent_room->y;
            }

            int good = true;
            if (i > 0) {
                for (int j = 0; j < i; j++) {
                    Entity *r = &vals.rooms[j];
                    if (collides(&room, r)) {
                        good = false;
                        break;
                    }
                }
            }
            if (good) {
                update_min_max(&vals, &room);
                add_room(&vals, room);
                tries = max_tries;
            }
        }
    }
    Map dungeon = create_map(1 + vals.max_x - vals.min_x,
                             1 + vals.max_y - vals.min_y);
    int room_id = 0;
    while (room_id < vals.room_count) {
        Entity *r = &(vals.rooms[room_id]);
        r->x -= vals.min_x;
        r->y -= vals.min_y;
        printf("x: %d, y: %d, w: %d, h: %d\n", r->x, r->y, r->w, r->h);
        dig_room(&dungeon, r->x, r->y, r->w, r->h);
        room_id++;
    }

    //free(vals.rooms);
    return dungeon;
}
