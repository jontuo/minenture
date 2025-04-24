
#ifndef __MATERIAL_HPP_INCLUDED__
#define __MATERIAL_HPP_INCLUDED__

#include <string>



struct Material
{
    std::string name;
    double density;
    double friction;
    double elasticity;

    Material(const std::string& name, double density, double friction, double elasticity): name(name), density(density), friction(friction), elasticity(elasticity) {}
};

#endif
