
#ifndef __VERTEX_HPP_INCLUDED__
#define __VERTEX_HPP_INCLUDED__

#include <glm/glm.hpp>


struct EntityVertex
{
    glm::vec2 position;
    glm::vec2 coordinate;
    glm::vec4 color;
    unsigned int texture;

    EntityVertex(const glm::vec2& position, const glm::vec2& coordinate, unsigned int texture): position(position), coordinate(coordinate), color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), texture(texture) {}
};

struct ShapeVertex
{
    glm::fvec2 position;
    glm::fvec4 color;

    ShapeVertex(const glm::fvec2& position, const glm::fvec4& color): position(position), color(color) {}
};




#endif
