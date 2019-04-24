#include "dreamwarp.h"
#include <time.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

#define OPEN true
#define CLOSED false

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

int MapDraft_createRoom(MapDraft *map_draft, int x, int y, int entrance_dir, int chance) {
    RoomDraft *cell = &map_draft->cells[y * map_draft->w + x];
    chance = chance + 1;
    if (cell->used) {
        return 0;
    }
    cell->used = true;

    if (entrance_dir == NORTH) {
        cell->north = OPEN;
    }
    if (entrance_dir == EAST) {
        cell->east = OPEN;
    }
    if (entrance_dir == SOUTH) {
        cell->south = OPEN;
    }
    if (entrance_dir == WEST) {
        cell->west = OPEN;
    }
    if (entrance_dir != NORTH && y > 0 && rand() % chance == 0) {
        cell->north = MapDraft_createRoom(map_draft, x, y - 1, SOUTH, chance);
    }
    if (entrance_dir != EAST && x < map_draft->w - 1 && rand() % chance == 0) {
        cell->east = MapDraft_createRoom(map_draft, x + 1, y, WEST, chance);
    }
    if (entrance_dir != SOUTH && y < map_draft->h - 1 && rand() % chance == 0) {
        cell->south = MapDraft_createRoom(map_draft, x, y + 1, NORTH, chance);
    }
    if (entrance_dir != WEST && x > 0 && rand() % chance == 0) {
        cell->west = MapDraft_createRoom(map_draft, x - 1, y, EAST, chance);
    }
    return OPEN;
}

MapDraft MapDraft_create(int w, int h) {
    srand(time(NULL));
    MapDraft draft;
    draft.w = w;
    draft.h = h;
    draft.cells = malloc((w * h) * sizeof(RoomDraft));
    draft.room_count = 0;
    RoomDraft room_draft;
    room_draft.north = CLOSED;
    room_draft.east = CLOSED;
    room_draft.south = CLOSED;
    room_draft.west = CLOSED;
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
    //setlocale(LC_CTYPE, "");
    MapDraft draft;
    draft = MapDraft_create(10, 10);
    MapDraft_createRoom(&draft, 5, 5, 0, 1);
    wchar_t rep;
    for (int y = 0; y < draft.h; y++) {
        for (int x = 0; x < draft.w; x++) {
            RoomDraft *cell = &draft.cells[y * draft.w + x];
            int n = cell->north;
            int e = cell->east;
            int s = cell->south;
            int w = cell->west;
            if (cell->used) {
                /*if (n && e && s && w) {
                    rep = 0x253c;
                } else if (n && e && s) {
                    rep = 0x251c;
                } else if (e && s && w) {
                    rep = 0x252c;
                } else if (s && w && n) {
                    rep = 0x2524;
                } else if (w && n && e) {
                    rep = 0x2534;
                } else if (n && e) {
                    rep = 0x2514;
                } else if (e && s) {
                    rep = 0x250c;
                } else if (s && w) {
                    rep = 0x2510;
                } else if (w && n) {
                    rep = 0x2518;
                } else if (w && e) {
                    rep = 0x2500;
                } else if (n && s) {
                    rep = 0x2502;
                } else {
                    rep = 0x256c;
                }*/
                printf("#");
                //wprintf(L"%lc", rep);
            } else {
                printf(" ");
                //wprintf(L"%lc", 0x0020);
            }
        }
        printf("\n");
        //wprintf(L"%lc", 0x0085);
    }
    printf("\n");
    //wprintf(L"%lc", 0x0085);
    return 0;
}
