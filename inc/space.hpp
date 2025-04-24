
#ifndef __SPACE_HPP_INCLUDED__
#define __SPACE_HPP_INCLUDED__

#include "body.hpp"
#include "shape.hpp"
#include "constraint.hpp"

class Space
{
public:

    cpSpace* handle;

    Space(const cpVect& gravity);
    ~Space();

    void add(const Body& body);
    void add(const Shape& shape);
    void add(const Constraint& constraint);

    cpBool contains(const Body& body);
    cpBool contains(const Shape& shape);
    cpBool contains(const Constraint& constraint);

    void remove(const Body& body);
    void remove(const Shape& shape);
    void remove(const Constraint& constraint);

    void update(cpFloat deltaTime);
    cpTimestamp getTimeStamp();

    cpVect getGravity();
    void setGravity(const cpVect& gravity);
    void setGravity(cpFloat vertical);


    cpFloat getDamping();
    void setDamping(cpFloat damping);

    int getIterations();
    void setIterations(int iterations);

    cpFloat getIdleSpeedThreshold();
    void setIdleSpeedThreshold(cpFloat idleSpeedThreshold);

    cpFloat getSleepTimeThreshold();
    void setSleepTimeThreshold(cpFloat sleepTimeThreshold);

    cpFloat getCollisionSlop();
    void setCollisionSlop(cpFloat collisionSlop);

    cpFloat getCollisionBias();
    void setCollisionBias(cpFloat collisionBias);


    unsigned int getBodyCount();
    unsigned int getStaticBodyCount();
    unsigned int getDynamicBodyCount();
    unsigned int getRousedBodyCount();
    unsigned int getSleepingBodyCount();

    unsigned int getShapeCount();
    unsigned int getStaticShapeCount();
    unsigned int getDynamicShapeCount();

    unsigned int getConstraintCount();



private:

    static void wrapRemoveConstraintHandle(cpSpace* spaceHandle, cpConstraint* constraintHandle, void* unknown);
    static void wrapRemoveShapeHandle(cpSpace* spaceHandle, cpShape* shapeHandle, void* unknown);
    static void wrapRemoveBodyHandle(cpSpace* spaceHandle, cpBody* bodyHandle, void* unknown);

    static void postRemoveConstraintHandle(cpConstraint* constraintHandle, cpSpace* spaceHandle);
    static void postRemoveShapeHandle(cpShape* shapeHandle, cpSpace* spaceHandle);
    static void postRemoveBodyHandle(cpBody* bodyHandle, cpSpace* spaceHandle);



};

#endif
