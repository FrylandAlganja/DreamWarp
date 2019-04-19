#include "dreamwarp.h"

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
    entity.vx = 0;
    entity.vy = 0;
    entity.spr = 0;
    entity.type = 0;
    entity.active = true;
    
    return entity;
}
