#include <stdbool.h>
#include "dreamwarp.h"

bool collides (Entity *e1, Entity *e2) {
    return (e1->x < e2->x + e2->w &&
           e2->x < e1->x + e1->w &&
           e1->y < e2->y + e2->h &&
           e2->y < e1->y + e1->h);
}

int bottom (Entity *e) {
    return e->y + e->h - 1;
}

int right (Entity *e) {
    return e->x + e->w - 1;
}

Point direction_delta(int direction) {
    Point delta;
    if (direction == NORTH) {
        delta.x = 0;
        delta.y = -1;
    } else if (direction == EAST) {
        delta.x = 1;
        delta.y = 0;
    } else if (direction == SOUTH) {
        delta.x = 0;
        delta.y = 1;
    } else if (direction == WEST) {
        delta.x = -1;
        delta.y = 0;
    } else {
        printf("%d is not a direction", direction);
    }
    return delta;
}
