
#ifndef __TEXTURE_HPP_INCLUDED__
#define __TEXTURE_HPP_INCLUDED__

#include <array>
#include <vector>
#include <glm/glm.hpp>



struct Texture
{
    std::array<glm::fvec2, 6> vertices;
    std::uint16_t identifier;

    //Texture(const TextureDefinition& definition);
    Texture(std::uint16_t identifier, const glm::vec2& offset);
    Texture(std::uint16_t identifier);

};



struct Textures
{
    std::vector<Texture> textures;

    Textures() = default;
    //Textures(const std::vector<TextureDefinition>& definitions);
};



#endif
