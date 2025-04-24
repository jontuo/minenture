
#ifndef __DEFINITION_HPP_INCLUDED__
#define __DEFINITION_HPP_INCLUDED__

/*
    TODO: Move declaration in to a ".cpp" file, also the "#include material.hpp" plus add forwarding of "Material".
*/

#include <string>
#include <vector>
#include <experimental/optional>        // std::experimental::optional<T>
#include <glm/glm.hpp>

#include "material.hpp"

#include <iostream>                     // TEMP: std::cout


/*

*/
struct Definition
{
    /*

    */
    struct Base
    {
        std::string name;

        Base(const std::string& name):
            name(name)
        {}
    };

    /*
        TODO: Add CP_BODY_TYPE?
    */
    struct Body
    {
        Body() = default;
    };

    /*
        TODO: Change "vertices" and "bevel" in to a "Model" or "Form" structure in constructor parameter, not variable members tho?
    */
    struct Shape
    {
        std::vector<cpVect> vertices;
        cpFloat bevel;
        cpFloat density;
        cpFloat friction;
        cpFloat elasticity;

        Shape(const std::vector<cpVect>& vertices, cpFloat bevel, const Material& material):
            vertices(vertices),
            bevel(bevel),
            density(material.density),
            friction(material.friction),
            elasticity(material.elasticity)
        {
        }
    };
  
    /*

    */
    struct Texture
    {
        std::uint16_t identifier;
        glm::vec2 position;

        Texture() {}
        Texture(std::uint16_t identifier): identifier(identifier), position(0.0f, 0.0f) {}
        Texture(std::uint16_t identifier, const glm::vec2& position): identifier(identifier), position(position) {}
    };
    
    /*
        TODO: Replace std::vector<JointType> with std::bitfield. Or multiple std::optional?
    */
    /*struct Joint
    {
        cpVect position;
        std::vector<JointType> types; // Types to be created when Attach.
        cpFloat maxForce;
    };*/



    std::experimental::optional<Base> base;
    std::experimental::optional<Body> body;
    std::experimental::optional<Shape> shape;
    std::vector<Texture> textures; // NOTE: In system: if more than one, use Textures, else Texture.
    //std::experimental::optional<Joint> joint; // TODO: Change allocation order with std::vector<Texture>?
    std::vector<Definition> definitions;



    Definition() = default;

    Definition(const std::string& name, const std::vector<cpVect>& vertices, cpFloat bevel, const Material& material, std::uint16_t texture):
        base(std::experimental::in_place, name),
        body(std::experimental::in_place),
        shape(std::experimental::in_place, vertices, bevel, material),
        textures({{texture}})
    {}

    Definition(const std::string& name, const std::vector<cpVect>& vertices, cpFloat bevel, const Material& material, std::vector<Texture> textures):
        base(std::experimental::in_place, name),
        body(std::experimental::in_place),
        shape(std::experimental::in_place, vertices, bevel, material),
        textures(textures)
    {}

    
};

#endif
