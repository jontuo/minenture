#ifndef __SHAPE_HPP_INCLUDED__
#define __SHAPE_HPP_INCLUDED__

#include <vector>
#include "body.hpp"
#include "material.hpp"




struct Shape
{
    cpShape* handle;

    Shape();
    Shape(const Body& body, const std::vector<cpVect>& vertices, cpFloat bevel, cpFloat density, cpFloat friction, cpFloat elasticity);

    //Shape(const Shape& shape);
    //Shape& operator=(const Shape& shape);

    Shape(Shape&& shape);
    Shape& operator=(Shape&& shape);

    cpVect getSurfaceVelocity(); // TODO: Possible to return reference here? Requires const to be safe?
    void setSurfaceVelocity(const cpVect& velocity);

    ~Shape();
};





#endif


