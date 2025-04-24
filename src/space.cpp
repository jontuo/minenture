
#include "space.hpp"
#include <iostream>
#include "setting.hpp"
#include "chipmunk/chipmunk_structs.h"

Space::Space(const cpVect& gravity)
{
    handle = cpSpaceNew();

    setGravity(gravity);
    setDamping(SPACE_DAMPING);
    setIterations(SPACE_ITERATION);
    setCollisionSlop(SPACE_COLLISION_SLOP);
    setCollisionBias(SPACE_COLLISION_BIAS);
    setIdleSpeedThreshold(SPACE_IDLE_SPEED_THRESHOLD);
    setSleepTimeThreshold(SPACE_SLEEP_TIME_THRESHOLD);

    if(SPACE_SPATIAL_HASH_ENABLED == true)
    {
        cpSpaceUseSpatialHash(handle, SPACE_SPATIAL_HASH_DIMENSION, SPACE_SPATIAL_HASH_COUNT);
    }


}

Space::~Space()
{
    if(handle != NULL)
    {
        cpSpaceEachConstraint(handle, (cpSpaceConstraintIteratorFunc)postRemoveConstraintHandle, handle);
        cpSpaceEachShape(handle, (cpSpaceShapeIteratorFunc)postRemoveShapeHandle, handle);
        cpSpaceEachBody(handle, (cpSpaceBodyIteratorFunc)postRemoveBodyHandle, handle);

        cpSpaceFree(handle);
    }
}



void Space::add(const Body& body)
{
    cpSpaceAddBody(handle, body.handle);
}

void Space::add(const Shape& shape)
{
    cpSpaceAddShape(handle, shape.handle);
}

void Space::add(const Constraint& constraint)
{
    cpSpaceAddConstraint(handle, constraint.handle);
}



cpBool Space::contains(const Body& body)
{
    return(cpSpaceContainsBody(handle, body.handle));
}

cpBool Space::contains(const Shape& shape)
{
    return(cpSpaceContainsShape(handle, shape.handle));
}

cpBool Space::contains(const Constraint& constraint)
{   
    return(cpSpaceContainsConstraint(handle, constraint.handle));
}



void Space::remove(const Body& body)
{
    cpSpaceRemoveBody(handle, body.handle);
}

void Space::remove(const Shape& shape)
{
    cpSpaceRemoveShape(handle, shape.handle);
}

void Space::remove(const Constraint& constraint)
{
    cpSpaceRemoveConstraint(handle, constraint.handle);
}



void Space::wrapRemoveConstraintHandle(cpSpace* spaceHandle, cpConstraint* ConstraintHandle, void* unknown)
{
    cpSpaceRemoveConstraint(spaceHandle, ConstraintHandle);
}

void Space::wrapRemoveShapeHandle(cpSpace* spaceHandle, cpShape* shapeHandle, void* unknown)
{
    cpSpaceRemoveShape(spaceHandle, shapeHandle);
}

void Space::wrapRemoveBodyHandle(cpSpace* spaceHandle, cpBody* bodyHandle, void* unknown)
{
    cpSpaceRemoveBody(spaceHandle, bodyHandle);
}



void Space::postRemoveConstraintHandle(cpConstraint* ConstraintHandle, cpSpace* spaceHandle)
{
    cpSpaceAddPostStepCallback(spaceHandle, (cpPostStepFunc)wrapRemoveShapeHandle, ConstraintHandle, NULL);
}

void Space::postRemoveShapeHandle(cpShape* shapeHandle, cpSpace* spaceHandle)
{
    cpSpaceAddPostStepCallback(spaceHandle, (cpPostStepFunc)wrapRemoveShapeHandle, shapeHandle, NULL);
}

void Space::postRemoveBodyHandle(cpBody* bodyHandle, cpSpace* spaceHandle)
{
    cpSpaceAddPostStepCallback(spaceHandle, (cpPostStepFunc)wrapRemoveBodyHandle, bodyHandle, NULL);
}






void Space::update(cpFloat deltaTime)
{
    cpSpaceStep(handle, deltaTime);
}

cpTimestamp Space::getTimeStamp()
{
    return(handle->stamp);
}



cpVect Space::getGravity()
{
    return(cpSpaceGetGravity(handle));
}

void Space::setGravity(const cpVect& gravity)
{
    cpSpaceSetGravity(handle, gravity);
}

void Space::setGravity(cpFloat vertical)
{
    setGravity(cpv(0.0, vertical));
}


/*
void Space::step(cpFloat deltaTime)
{
    cpSpaceStep(handle, deltaTime);
}

cpFloat Space::getStep()
{
    return(cpSpaceGetCurrentTimeStep(handle));
}*/



cpFloat Space::getDamping()
{
    return(cpSpaceGetDamping(handle));
}

void Space::setDamping(cpFloat damping)
{
    cpSpaceSetDamping(handle, damping);
}

int Space::getIterations()
{
    return(cpSpaceGetIterations(handle));
}

void Space::setIterations(int iterations)
{
    cpSpaceSetIterations(handle, iterations);
}

cpFloat Space::getIdleSpeedThreshold()
{
    return(cpSpaceGetIdleSpeedThreshold(handle));
}

void Space::setIdleSpeedThreshold(cpFloat idleSpeedThreshold)
{
    cpSpaceSetIdleSpeedThreshold(handle, idleSpeedThreshold);
}

cpFloat Space::getSleepTimeThreshold()
{
    return(cpSpaceGetSleepTimeThreshold(handle));
}

void Space::setSleepTimeThreshold(cpFloat sleepTimeThreshold)
{
    cpSpaceSetSleepTimeThreshold(handle, sleepTimeThreshold);
}

cpFloat Space::getCollisionSlop()
{
    return(cpSpaceGetCollisionSlop(handle));
}

void Space::setCollisionSlop(cpFloat collisionSlop)
{
    cpSpaceSetCollisionSlop(handle, collisionSlop);
}

cpFloat Space::getCollisionBias()
{
    return(cpSpaceGetCollisionBias(handle));
}

void Space::setCollisionBias(cpFloat collisionBias)
{
    cpSpaceSetCollisionBias(handle, collisionBias);
}

unsigned int Space::getStaticBodyCount()
{
    return(handle->staticBodies->num);
}

unsigned int Space::getDynamicBodyCount()
{
    return(handle->dynamicBodies->num);
}

unsigned int Space::getRousedBodyCount()
{
    return(handle->rousedBodies->num);
}

unsigned int Space::getSleepingBodyCount()
{
    return(handle->sleepingComponents->num);
}

unsigned int Space::getBodyCount()
{
    return(getStaticBodyCount() + getDynamicBodyCount() + getRousedBodyCount() + getSleepingBodyCount());
}



unsigned int Space::getConstraintCount()
{
    return(handle->constraints->num);
}



unsigned int Space::getShapeCount()
{
    return(getStaticShapeCount() + getDynamicShapeCount());
}

unsigned int Space::getStaticShapeCount()
{
    return(handle->staticShapes->klass->count(handle->staticShapes));
}

unsigned int Space::getDynamicShapeCount()
{
    return(handle->dynamicShapes->klass->count(handle->dynamicShapes));
}


