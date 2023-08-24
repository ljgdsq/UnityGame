#ifndef TINYRAYCASTER_H
#define TINYRAYCASTER_H
#include "framebuffer.h"
#include "map.h"
#include "player.h"
#include "sprite.h"
#include "textures.h"
#include <vector>
void render(FrameBuffer &fb, Map &map, Player &player, std::vector<Sprite> &sprites, Texture &tex_walls, Texture &tex_monst);

#endif // TINYRAYCASTER_H
