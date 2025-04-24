#ifndef __BODY_HPP_INCLUDED__
#define __BODY_HPP_INCLUDED__

#include "chipmunk/chipmunk_structs.h"


/*
    PROP: Maybe add a BodyDefinition, which is used for entt::Prototype only. It might help flipping feature later on?
*/

struct Body
{
    cpBody* handle;

    Body();

    //Body(const BodyDefinition& definition);
    //Body(const BodyDefinition& definition, const cpVect& position);

    Body(const cpVect& position);
    Body(const cpVect& position, cpBodyType type); // CP_BODY_TYPE_STATIC, CP_BODY_TYPE_DYNAMIC, CP_BODY_TYPE_KINEMATIC

    Body(Body&& body);
    Body& operator=(Body&& body);

    /*Body(const Body& body);
    Body& operator=(const Body& body);
*/
    ~Body();


    void setPosition(const cpVect& position);
    double getAngle();
};






#endif


