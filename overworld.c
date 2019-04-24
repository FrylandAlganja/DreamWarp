#include "dreamwarp.h"
#include <time.h>

#define OPEN true
#define CLOSED true

typedef struct RoomDraftStruct RoomDraft;
struct RoomDraftStruct {
    int x, y;
    bool north, east, south, west;
    bool used;
};

// Map structure defines rooms and their connections
typedef struct MapDraftStruct MapDraft;

struct MapDraftStruct {
    int w, h;
    RoomDraft *cells;
    int room_count;
};

int MapDraft_createRoom(MapDraft *map_draft, int x, int y, int entrance_dir) {
    RoomDraft *cell = &map_draft->cells[y * map_draft->w + x];
    if (cell->used) {
        return 0;
    }
    cell->used = true;
    if (entrance_dir != NORTH && y > 0 && rand() % 2 == 0) {
        cell->north = MapDraft_createRoom(map_draft, x, y - 1, SOUTH);
    }
    if (entrance_dir != EAST && x < map_draft->w - 1 && rand() % 2 == 0) {
        cell->east = MapDraft_createRoom(map_draft, x + 1, y, WEST);
    }
    if (entrance_dir != SOUTH && y < map_draft->h - 1 && rand() % 2 == 0) {
        cell->south = MapDraft_createRoom(map_draft, x, y + 1, NORTH);
    }
    if (entrance_dir != WEST && x > 0 && rand() % 2 == 0) {
        cell->west = MapDraft_createRoom(map_draft, x - 1, y, EAST);
    }
    return 1;
}

MapDraft MapDraft_create(int w, int h) {
    srand(time(NULL));
    MapDraft draft;
    draft.w = w;
    draft.h = h;
    draft.cells = malloc((w * h) * sizeof(RoomDraft));
    draft.room_count = 0;
    RoomDraft room_draft;
    room_draft.north = OPEN;
    room_draft.east = OPEN;
    room_draft.south = OPEN;
    room_draft.west = OPEN;
    room_draft.used = false;

    for (int i = 0; i < w * h; i++) {
        draft.cells[i] = room_draft;
    }

    return draft;
}

void MapDraft_free(MapDraft *draft) {
    free(draft->cells);
}

int main() {
    MapDraft draft;
    draft = MapDraft_create(10, 10);
    MapDraft_createRoom(&draft, 5, 5, 0);
    for (int y = 0; y < draft.h; y++) {
        for (int x = 0; x < draft.w; x++) {
            if (draft.cells[y * draft.w + x].used) {
                printf("#");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n");
    return 0;
}
