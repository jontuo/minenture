
#include "texture.hpp"




//Texture::Texture(const TextureDefinition& definition): Texture(definition.identifier, definition.position) {}

//Textures::Textures(const std::vector<TextureDefinition>& definitions)
//{
//    for(const TextureDefinition& definition: definitions)
//    {
//        textures.emplace_back(definition);
//    }
//}

Texture::Texture(std::uint16_t identifier, const glm::vec2& offset): vertices({glm::vec2(-8.0f, -8.0f) + offset, glm::vec2(8.0f, -8.0f) + offset, glm::vec2(8.0f, 8.0f) + offset, glm::vec2(8.0f, 8.0f) + offset, glm::vec2(-8.0f, 8.0f) + offset, glm::vec2(-8.0f, -8.0f) + offset}), identifier(identifier)
{

}

Texture::Texture(std::uint16_t identifier): vertices({glm::vec2(-8.0f, -8.0f), glm::vec2(8.0f, -8.0f), glm::vec2(8.0f, 8.0f), glm::vec2(8.0f, 8.0f), glm::vec2(-8.0f, 8.0f), glm::vec2(-8.0f, -8.0f)}), identifier(identifier)
{
    
}



