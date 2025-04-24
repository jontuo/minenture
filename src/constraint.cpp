

#include "constraint.hpp"












    //TODO: Do 'owner' really need to be public?
    //TODO: Make default constructor as "= default;" ?
Constraint::Constraint() {}

Constraint::~Constraint()
{
    if(owner == true)
    {
        cpConstraintFree(handle);
    }
}

Constraint::Constraint(Constraint&& constraint)
{
    handle = constraint.handle;
    owner = constraint.owner;
    constraint.handle = nullptr;
    constraint.owner = false;
}

Constraint& Constraint::operator=(Constraint&& constraint)
{
    handle = constraint.handle;
    owner = constraint.owner;
    constraint.handle = nullptr;
    constraint.owner = false;

    return(*this);
}





    //TODO: Define and declare 'phase' and 'ratio' parameters for 'cpGearJointNew()' somewhere else?

Gear::Gear(const Body& sourceBody)
{
    cpFloat phase = 0.0;
    cpFloat ratio = 1.0;
    handle = cpGearJointNew(sourceBody.handle, NULL, phase, ratio);
    owner = true;
}



    //TODO: Change 'sourceAnchor' parameter in constructor to constant and reference?



Pivot::Pivot(const Body& sourceBody, cpVect sourceAnchor)
{
    handle = cpPivotJointNew2(sourceBody.handle, NULL, sourceAnchor, cpvzero);
    owner = true;
}



//TODO: Const and ref on sourceAnchor?
//TODO: Change 'sourceAnchor' parameter in constructor to constant and reference?

Pin::Pin(const Body& sourceBody, cpVect sourceAnchor)
{
    handle = cpPinJointNew(sourceBody.handle, NULL, sourceAnchor, cpvzero);
    owner = true;
}



//TODO: Change 'sourceGroove' and 'targetGroove' parameter in constructor to constant and reference?


Groove::Groove(const Body& sourceBody, cpVect sourceGroove, cpVect targetGroove)
{
    handle = cpGrooveJointNew(sourceBody.handle, NULL, sourceGroove, targetGroove, cpvzero);
    owner = true;
}



Motor::Motor(const Body& sourceBody)
{
    handle = cpSimpleMotorNew(sourceBody.handle, NULL, 0.0);
    owner = true;
}

