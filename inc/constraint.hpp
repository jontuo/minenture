
#ifndef __CONSTRAINT_HPP_INCLUDED__
#define __CONSTRAINT_HPP_INCLUDED__

#include <cstdint>
#include "body.hpp"






/*
enum JointType: std::uint16_t
{
    JOINT_TYPE_BAJS;
};*/










    //TODO: Do 'owner' really need to be public?
    //TODO: Make default constructor as "= default;" ?

class Constraint
{
public:

    cpConstraint* handle;
    bool owner;

    Constraint();
    ~Constraint();
    Constraint(Constraint&& constraint);
    Constraint& operator=(Constraint&& constraint);


private:

    
};






    //TODO: Define and declare 'phase' and 'ratio' parameters for 'cpGearJointNew()' somewhere else?

class Gear: Constraint
{
public:

    Gear(const Body& sourceBody);

private:

};



    //TODO: Change 'sourceAnchor' parameter in constructor to constant and reference?

class Pivot: Constraint
{
public:

    Pivot(const Body& sourceBody, cpVect sourceAnchor);

private:

};



    //TODO: Const and ref on sourceAnchor?
    //TODO: Change 'sourceAnchor' parameter in constructor to constant and reference?

class Pin: Constraint
{
public:

    //cpFloat distanceMultiplier; 

    Pin(const Body& sourceBody, cpVect sourceAnchor);

private:

};



    //TODO: Change 'sourceGroove' and 'targetGroove' parameter in constructor to constant and reference?

class Groove: Constraint
{
public:

    Groove(const Body& sourceBody, cpVect sourceGroove, cpVect targetGroove);

private:

};



class Motor: Constraint
{
public:

    //bool autobrake;

    Motor(const Body& sourceBody);

private:

};



#endif
