
#include "body.hpp"

Body::Body(): handle(NULL) {}

//Body::Body(): handle(cpBodyNew(0.0, 0.0)) {}

//Body::Body(const BodyDefinition& definition): handle(cpBodyNew(0.0, 0.0)) {}

//Body::Body(const BodyDefinition& definition, const cpVect& position): handle(cpBodyNew(0.0, 0.0))
//{
//    setPosition(position); 
//}

Body::Body(const cpVect& position)
{
    handle = cpBodyNew(0.0, 0.0);
    setPosition(position);
}

Body::Body(const cpVect& position, cpBodyType type)
{
    handle = cpBodyNew(0.0, 0.0);
    cpBodySetPosition(handle, position);
    cpBodySetType(handle, type);
}


/*
Body::Body(const Body& body)
{}

Body& Body::operator=(const Body& body)
{
    return(*this);
}*/


Body::Body(Body&& body)
{
    handle = body.handle;
    body.handle = NULL;
}

Body& Body::operator=(Body&& body)
{
    handle = body.handle;
    body.handle = NULL;

    return(*this);
}






Body::~Body()
{
    if(handle != NULL)
    {
        cpBodyFree(handle);
    }
}

void Body::setPosition(const cpVect& position)
{
    cpBodySetPosition(handle, position);
}







double Body::getAngle()
{
    return(handle->a);
}



