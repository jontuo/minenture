
#include "shape.hpp"



//Shape::Shape(const Body& body, const ShapeDefinition& definition): Shape(body, definition.vertices, definition.bevel, definition.density, definition.friction, definition.elasticity) {}

Shape::Shape(): handle(NULL) {}

Shape::Shape(const Body& body, const std::vector<cpVect>& vertices, cpFloat bevel, cpFloat density, cpFloat friction, cpFloat elasticity)
{
    handle = cpPolyShapeNewRaw(body.handle, vertices.size(), vertices.data(), bevel);
    cpShapeSetDensity(handle, density);
    cpShapeSetFriction(handle, friction);
    cpShapeSetElasticity(handle, elasticity);
}


/*
Shape::Shape(const Shape& shape)
{}

Shape& Shape::operator=(const Shape& shape)
{
    return(*this);
}*/



Shape::Shape(Shape&& shape)
{
    handle = shape.handle;
    shape.handle = NULL;
}

Shape& Shape::operator=(Shape&& shape)
{
    handle = shape.handle;
    shape.handle = NULL;

    return(*this);
}

Shape::~Shape()
{
    if(handle != NULL)
    {
        cpShapeFree(handle);
    }
}



cpVect Shape::getSurfaceVelocity()
{
    return(cpShapeGetSurfaceVelocity(handle));
}

void Shape::setSurfaceVelocity(const cpVect& velocity)
{
    cpShapeSetSurfaceVelocity(handle, velocity);
}
