#include "dreamwarp.h"

void set_sprite(int index, int x, int y, int w, int h, SDL_Rect *sprites) {
  SDL_Rect sprite = {x, y, w, h};
  sprites[index] = sprite;
}

void init_sprites(SDL_Rect *sprites) {
  set_sprite(SPR_GRASS, 0, 0, 48, 48, sprites);
  set_sprite(SPR_FLOOR, 48, 0, 48, 48, sprites);
  set_sprite(SPR_WALL, 96, 0, 48, 48, sprites);
  set_sprite(SPR_STAIR, 192, 0, 48, 48, sprites);
  set_sprite(SPR_WIZARD, 240, 0, 28, 42, sprites);
  set_sprite(SPR_CHICKEN, 268, 0, 22, 29, sprites);
  set_sprite(SPR_SWORDW1, 290, 0, 17, 39, sprites);
  set_sprite(SPR_SWORDW2, 307, 0, 24, 39, sprites);
  set_sprite(SPR_SWORDW3, 331, 0, 21, 39, sprites);
  set_sprite(SPR_GOBLIN, 352, 0, 24, 44, sprites);
  set_sprite(SPR_SEAGULL, 376, 0, 28, 24, sprites);
  set_sprite(SPR_SWORDN, 404, 0, 10, 47, sprites);
  set_sprite(SPR_SWORDS, 414, 0, 10, 47, sprites);
  set_sprite(SPR_SWORDE, 424, 0, 47, 10, sprites);
  set_sprite(SPR_SWORDW, 471, 0, 47, 10, sprites);
}
