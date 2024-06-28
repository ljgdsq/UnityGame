//
// Created by zhengshulin on 2024/6/27.
//

#ifndef OPENGLPROJ_COMMON_H
#define OPENGLPROJ_COMMON_H
#include <glm/glm.hpp>
#include <string>
using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

struct Vertex {
    vec3 position;
    vec2 texCoords;

    Vertex(const vec3 &position, const vec2 &texCoords) : position(position), texCoords(texCoords) {}

    Vertex(float x, float y, float z, float u, float v) : position(x, y, z), texCoords(u, v) {}

};

#endif //OPENGLPROJ_COMMON_H
