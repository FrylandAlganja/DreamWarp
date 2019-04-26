#include "dreamwarp.h"

void Entity_update(Entity *self) {}

Entity Entity_create() {
    Entity entity;
    entity.x = 0;
    entity.y = 0;
    entity.tile_x = 0;
    entity.tile_y = 0;
    entity.w = 0;
    entity.h = 0;
    entity.tile_w = 0;
    entity.tile_h = 0;
    entity.speed = 0;
    entity.vx = 0;
    entity.vy = 0;
    entity.spr = 0;
    entity.type = 0;
    entity.active = true;
    entity.action = 0;
    entity.action_duration = 0;
    entity.move_distance = 0;
    entity.facing = 1;
    entity.update = &Entity_update;
    
    return entity;
}

void Chicken_update(Entity *self) {
    self->vx = self->vy = 0;
    self->action_duration += 1;
    if (self->action_duration > 30) {
        self->action = (rand() % 4) + 1;
        self->action_duration = -1;
    } else if (self->action == MOVE) {
        if (self->action_duration == 0) {
            self->facing = (rand() % 4) + 1;
            self->move_distance = (rand() % 4) + 1;
        }
        if (self->action_duration < 8 * self->move_distance) {
            Point delta = direction_delta(self->facing);
            self->vx = delta.x * self->speed;
            self->vy = delta.y * self->speed;
        } else {
            self->vx = 0;
            self->vy = 0;
            self->action_duration = -1;
            self->action = (rand() % 4) + 1;
        }
    } else if (self->action == JUMP) {
        if (self->action_duration < 4) {
            self->vy = -1;
        } else if (self->action_duration < 8) {
            self->vy = 1;
        } else {
            self->action_duration = -1;
            self->action = (rand() % 4) + 1;
        }
    }
}
    

Entity Chicken_create() {
    Entity chicken = Entity_create();
    chicken.spr = SPR_CHICKEN;
    chicken.w = 22;
    chicken.h = 29;
    chicken.speed = 4;
    chicken.action = STAND;
    chicken.update = &Chicken_update;

    return chicken;
}
