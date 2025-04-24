
#ifndef __MESH_HPP_INCLUDED__
#define __MESH_HPP_INCLUDED__

#include <array>
#include "chipmunk/chipmunk_types.h" // FIX: Or is it "chipmunk/vect.h"?

struct Mesh
{
    std::array<cpVect, 6> vertices;

    Mesh(const cpVect& size, const cpVect& origin): vertices({cpv(-size.x / 2.0 + origin.x, -size.y / 2.0 + origin.y), cpv(size.x / 2.0 + origin.x, -size.y / 2.0 + origin.y), cpv(size.x / 2.0 + origin.x, size.y / 2.0 + origin.y), cpv(size.x / 2.0 + origin.x, size.y / 2.0 + origin.y), cpv(-size.x / 2.0 + origin.x, size.y / 2.0 + origin.y), cpv(-size.x / 2.0 + origin.x, -size.y / 2.0 + origin.y)}){}
};



/*
const std::array<glm::vec2, 6> SQUARE_VERTICES({glm::vec2(-8.0f, -8.0f), glm::vec2(8.0f, -8.0f), glm::vec2(8.0f, 8.0f), glm::vec2(8.0f, 8.0f), glm::vec2(-8.0f, 8.0f), glm::vec2(-8.0f, -8.0f)});

const std::array<glm::vec2, 6> SQUARE_COORDINATES({glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});

enum Texture: unsigned int
{
    STONE_TEXTURE = 45,
    CYLINDER_LEFT_TEXTURE = 172,
    CYLINDER_MIDDLE_TEXTURE = 173,
    CYLINDER_RIGHT_TEXTURE = 174,
    PISTON_LEFT_TEXTURE = 212,
    PISTON_MIDDLE_TEXTURE = 213,
    PISTON_RIGHT_TEXTURE = 214
};



struct Mesh
{
    std::array<glm::vec2, 6> vertices;
    std::array<glm::vec2, 6> coordinates;
    Texture texture;
    glm::vec2 offset;
    size_t size;

    Mesh(const std::array<glm::vec2, 6> vertices, const std::array<glm::vec2, 6> coordinates, Texture texture):
        vertices(vertices), coordinates(coordinates), texture(texture), offset(0.0f, 0.0f), size(6)
    {
        
    }

    Mesh(const std::array<glm::vec2, 6> vertices, const std::array<glm::vec2, 6> coordinates, Texture texture, glm::vec2 offset):
        vertices(vertices), coordinates(coordinates), texture(texture), offset(offset), size(6)
    {

    }
};
*/

/*
    std::array<Vertex, 6> vertices;

    Sprite(float positionX, float positionY, float sizeWidth, float sizeHeight, unsigned int textureIdentifier)
    {
        float halfWidth = sizeWidth / 2.0f;
        float halfHeight = sizeHeight / 2.0f;

        vertices[0] = {positionX - halfWidth, positionY - halfHeight, 0.0f, 0.0f, textureIdentifier};
        vertices[1] = {positionX + halfWidth, positionY - halfHeight, 1.0f, 0.0f, textureIdentifier};
        vertices[2] = {positionX + halfWidth, positionY + halfHeight, 1.0f, 1.0f, textureIdentifier};
        vertices[3] = {positionX - halfWidth, positionY - halfHeight, 0.0f, 0.0f, textureIdentifier};
        vertices[4] = {positionX + halfWidth, positionY + halfHeight, 1.0f, 1.0f, textureIdentifier};
        vertices[5] = {positionX - halfWidth, positionY + halfHeight, 0.0f, 1.0f, textureIdentifier};
    }
*/

#endif
