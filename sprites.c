#include "dreamwarp.h"

void set_sprite(int index, int x, int y, int w, int h) {
  SDL_Rect sprite = {x, y, w, h};
  sprites[index] = sprite;
}

void init_sprites() {
  set_sprite(SPR_GRASS, 0, 0, 48, 48);
  set_sprite(SPR_FLOOR, 48, 0, 48, 48);
  set_sprite(SPR_WALL, 96, 0, 48, 48);
  set_sprite(SPR_STAIR, 192, 0, 48, 48);
  set_sprite(SPR_WIZARD, 240, 0, 28, 42);
  set_sprite(SPR_CHICKEN, 268, 0, 22, 29);
  set_sprite(SPR_SWORDW1, 290, 0, 17, 39);
  set_sprite(SPR_SWORDW2, 307, 0, 24, 39);
  set_sprite(SPR_SWORDW3, 331, 0, 21, 39);
}
