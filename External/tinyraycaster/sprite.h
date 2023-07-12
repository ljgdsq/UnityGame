//
// Created by ljgdsq on 2023/7/12.
//

#ifndef SPRITE_H
#define SPRITE_H
#include <cstdlib>
struct Sprite{
    float x,y;
    size_t tex_id;
    float player_dist;
    bool operator <(const Sprite&other){
        return player_dist > other.player_dist;
    }
};

#endif //SPRITE_H
