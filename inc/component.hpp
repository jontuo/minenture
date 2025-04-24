
#ifndef __COMPONENT_HPP_INCLUDED__
#define __COMPONENT_HPP_INCLUDED__

#include <array>                        // std::array<>
#include <vector>                       // std::vector<>
#include <string>                       // std::string
#include <cstdint>                      // std::uint16_t, std::u

#include <glm/glm.hpp>                  // glm::fvec2<>, glm::dvec2<>

#include "SFML/Window/Keyboard.hpp"     // sf::Keyboard::* for key codes.
#include "entt/entity/entity.hpp"
#include "material.hpp"                 // Material
#include "type.hpp"                 // Entity
#include "chipmunk/chipmunk_structs.h" // cpBody, cpShape and cpConstraint.
#include <iostream>                     // std::cout









struct Magnet
{
    bool activated;
    cpFloat radius;
    cpFloat force;
};





/*
    Before populating Action, check if the action is whitelisted in this component first.
*/
struct Control
{

};

/*
    Contains actions that should be applied or distributed along children(?).
    Either the sfml event or AI will populate this list.
    temporary componnent? removed when actions has been done?
*/
struct Action
{
};








struct Anchor
{

    double strength; // If motor's force is larger than strength, the joint will disconnect.
};



struct Joint
{
    //std::vector<Constraint> constraints;
    Entity entity;
};




struct Parent
{
    Entity entity;
    cpVect position;
    //std::vector<Constraint> constraints;
};

struct Child
{
    Entity entity;
    cpVect position;

    Child(const cpVect& position): entity(entt::null), position(position) {}
    Child(Entity entity, const cpVect& position): entity(entity), position(position) {}
};

struct Children
{
    std::vector<Child> children;
};


/*




What if you would fuse parts together more often than using constraints? For welds...
Fuse in a motor in a vehicles anchor? fuse additional building blocks to make more complex shapes?
fuse together larger chunks/boulders of multiple stone blocks?
they would still be multiple entities but the root entity has the only body component. so its easy to separate them.








Vehicle and Tire:
Vehicles decides if there should be a Motor or Gear and its maxForce,
also where the "anchors" are positioned,
but Tire owns the Motor or Gear, plus it decides and owns Groove and Spring.

Vehicle and Cylinder:
Vehicle decides 


It would have been really cool to attach items to anchors which decides what constraints it will have.
Imagine to have a container full of different motor and servos, take one, put it on an anchor.
Then you also put a Piston on the anchor.





The bind settings will decide which constraints between two entities will have.
For example, attach a Tire to a Vehicle. Only a Pivot will be created/spawned.
Enter Vehicle's binding, set D key to send Rotate action to Tire. It adds a Gear to it.
Kinda like "It needs 'power' to function like a force".
Same with Cylinder and Rod, a Groove will be spawned as default.
So the Rod will move freely (between min and max of course) until it has "power" (a bind).

Hmm, sure the above sounds great. But Pistons should not really have any gear, like in IRL. Only Pivots on both ends.
Sure, the Player will have it, but thats an exception.
But I dont think this will work so great with computer physics... The forces are too weird.

What if the entities had all the available constraints already BUT NOT added to Space until it was bound to a key?






Vehicle body decides if there should be a Motor or Gear and its maxForce and similar.
The tire decide Groove and Spring.



An "anchor" can hold multiple entities. EDIT: What did i think of here?


What about Physics, Appearence and Logic components?
BodyPhysics
ShapePhysics
JointPhysics
ImageAppearence
AnimationAppearence
ControlLogic
ActionLogic


*/









/*




struct Base
{
    std::string name;

    Base(const std::string& name): name(name) {}

    std::ostream &operator<<(std::ostream &stream)
    { 
        return stream << name;
    }
};



    TODO: Do 'owner' really need to be public?

class Body
{
public:

    cpBody* handle;
    bool owner = false;



    Body()
    {
        std::cout << " DFLT CTOR" << std::endl;
    }

    //Body(cpVect position): owner(true)
    //{
    //    handle = cpBodyNew(0.0, 0.0);
    //    cpBodySetPosition(handle, position);
//
    //    std::cout << " CUST CTOR" << std::endl;
    //}

    //Body(cpVect position, cpBodyType type): owner(true)
    //{
    //    handle = cpBodyNew(0.0, 0.0);
    //    cpBodySetPosition(handle, position);
    //    cpBodySetType(handle, type);
    //}

    // Handle constructor.
    Body(cpBody* handle): handle(handle), owner(false)
    {
        std::cout << "53343" << std::endl;
    }

    // Copy constructor.
    Body(const Body& body)
    {
        std::cout << " COPY CTOR" << std::endl;
        handle = cpBodyNew(0.0, 0.0);
        owner = true;

        
    }

    // Copy operator.
    Body& operator=(const Body& body)
    {
        std::cout << " COPY OPER" << std::endl;
        handle = cpBodyNew(0.0, 0.0);
        owner = true;

        

        return(*this);
    }

    // Move constructor.
    Body(Body&& body)
    {
        std::cout << " MOVE CTOR1" << std::endl;
        handle = body.handle;
        owner = body.owner;

        body.handle = nullptr;
        body.owner = false;
        std::cout << " MOVE CTOR2" << std::endl;
        
    }

    // Move operator.
    Body& operator=(Body&& body)
    {
        std::cout << " MOVE OPER1" << std::endl;
        handle = body.handle;
        owner = body.owner;

        body.handle = nullptr;
        body.owner = false;
        std::cout << " MOVE OPER2" << std::endl;

        return(*this);
    }

    // Default destructor.
    ~Body()
    {
        if(owner == true)
        {
            cpBodyFree(handle);
        }
    }

    void setPosition(const cpVect& position)
    {
        cpBodySetPosition(handle, position);
    }

private:


};    












    TODO: Delete or set the default constructor to private?
    TODO: Change 'offset' parameter in 'Shape()' to const and reference?
    TODO: Define and declare radius parameter for 'cpPolyShapeNewRaw()' in constructor somewhere else.
    TODO: Do 'owner' really need to be public?

class Shape
{
public:

    cpShape* handle;
    bool owner;
    cpVect offset;  // TEMP: I couldnt find vertices in chipmunk's internal, so storing those here for now.
    std::array<cpVect, 4> vertices; // TEMP: I couldnt find vertices in chipmunk's internal, so storing those here for now.

    Shape()
    {

    }

    Shape(const Body& body, const Material& material, const cpVect& offset = cpvzero):
        owner(true),
        offset(offset),
        vertices({cpvadd(cpv(-8.0, -8.0), offset), cpvadd(cpv(8.0, -8.0), offset), cpvadd(cpv(8.0, 8.0), offset), cpvadd(cpv(-8.0, 8.0), offset)})
    {
        handle = cpPolyShapeNewRaw(body.handle, vertices.size(), vertices.data(), 0.1);
        setMaterial(material);
    }

    Shape(const Body& body, const Material& material, cpFloat radius, const cpVect& offset = cpvzero):
        owner(true),
        offset(offset),
        vertices({cpvadd(cpv(-8.0, -8.0), offset), cpvadd(cpv(8.0, -8.0), offset), cpvadd(cpv(8.0, 8.0), offset), cpvadd(cpv(-8.0, 8.0), offset)})
    {
        handle = cpCircleShapeNew(body.handle, radius, offset);
        setMaterial(material);
    }
    

    // What is this?
    //
    // Custom constructor.
    //Shape(const cpBody* bodyHandle, const cpVect& origin): owner(true)
    //{    
    //    cpVect verts[] =
    //    {
    //        cpv(8.0 + origin.x, 8.0 + origin.y),
    //        cpv(8.0 + origin.x, -8.0 + origin.y),
    //        cpv(-8.0 + origin.x, -8.0 + origin.y),
    //        cpv(-8.0 + origin.x, 8.0 + origin.y)
    //    };
    //    handle = cpPolyShapeNew(bodyHandle, 4, verts, cpTransformIdentity, 0.1);
    //    cpShapeSetMass(handle, 10.0f);
    //}

    // Custom constructor.
    Shape(cpShape* handle): handle(handle), owner(false)
    {

    }

    // Move constructor.
    Shape(Shape&& shape)
    {
        handle = shape.handle;
        owner = shape.owner;
        shape.handle = nullptr;
        shape.owner = false;
    }

    // Move operator.
    Shape& operator=(Shape&& shape)
    {
        handle = shape.handle;
        owner = shape.owner;
        shape.handle = nullptr;
        shape.owner = false;

        return(*this);
    }

    // Default destructor.
    ~Shape()
    {
        if(owner == true)
        {
            cpShapeFree(handle);
        }
    }

    void setMaterial(const Material& material)
    {
        setDensity(material.density);
        setFriction(material.friction);
        setElasticity(material.elasticity);
    }

    cpFloat getMass()
    {
        return(cpShapeGetMass(handle));
    }

    void setMass(cpFloat mass)
    {
        cpShapeSetMass(handle, mass);
    }

    void setDensity(cpFloat density)
    {
        cpShapeSetDensity(handle, density);
    }

    void setFriction(cpFloat friction)
    {
        cpShapeSetFriction(handle, friction);
    }

    void setElasticity(cpFloat elasticity)
    {
        cpShapeSetElasticity(handle, elasticity);
    }

    void setEntity(Entity entity)
    {
        cpShapeSetUserData(handle, reinterpret_cast<void*>(static_cast<std::uintptr_t>(entity)));
    }

    Entity getEntity()
    {
        return(static_cast<Entity>(reinterpret_cast<std::uintptr_t>(cpShapeGetUserData(handle))));
    }

    cpBody* getBodyHandle()
    {
        return(cpShapeGetBody(handle));
    }

private:

};














    TODO: Do 'owner' really need to be public?
    TODO: Make default constructor as "= default;" ?

class Constraint
{
public:

    cpConstraint* handle;
    bool owner;

    Constraint() {}

    ~Constraint()
    {
        if(owner == true)
        {
            cpConstraintFree(handle);
        }
    }

    Constraint(Constraint&& constraint)
    {
        handle = constraint.handle;
        owner = constraint.owner;
        constraint.handle = nullptr;
        constraint.owner = false;
    }

    Constraint& operator=(Constraint&& constraint)
    {
        handle = constraint.handle;
        owner = constraint.owner;
        constraint.handle = nullptr;
        constraint.owner = false;

        return(*this);
    }


private:

    
};







    TODO: Define and declare 'phase' and 'ratio' parameters for 'cpGearJointNew()' somewhere else?

class Gear: Constraint
{
public:

    Gear(const Body& sourceBody)
    {
        cpFloat phase = 0.0;
        cpFloat ratio = 1.0;
        handle = cpGearJointNew(sourceBody.handle, NULL, phase, ratio);
        owner = true;
    }

private:

};



    TODO: Change 'sourceAnchor' parameter in constructor to constant and reference?

class Pivot: Constraint
{
public:

    Pivot(const Body& sourceBody, cpVect sourceAnchor)
    {
        handle = cpPivotJointNew2(sourceBody.handle, NULL, sourceAnchor, cpvzero);
        owner = true;
    }

private:

};



    TODO: Const and ref on sourceAnchor?
    TODO: Change 'sourceAnchor' parameter in constructor to constant and reference?

class Pin: Constraint
{
public:

    //cpFloat distanceMultiplier; 

    Pin(const Body& sourceBody, cpVect sourceAnchor)
    {
        handle = cpPinJointNew(sourceBody.handle, NULL, sourceAnchor, cpvzero);
        owner = true;
    }

private:

};



    TODO: Change 'sourceGroove' and 'targetGroove' parameter in constructor to constant and reference?

class Groove: Constraint
{
public:

    Groove(const Body& sourceBody, cpVect sourceGroove, cpVect targetGroove)
    {
        handle = cpGrooveJointNew(sourceBody.handle, NULL, sourceGroove, targetGroove, cpvzero);
        owner = true;
    }

private:

};



class Motor: Constraint
{
public:

    //bool autobrake;

    Motor(const Body& sourceBody)
    {
        handle = cpSimpleMotorNew(sourceBody.handle, NULL, 0.0);
        owner = true;
    }

private:

};



    TODO: Switch place of "offset" and "identifier" parameters in constructor.

struct Texture
{
    std::array<glm::fvec2, 6> vertices;
    std::array<glm::fvec2, 6> coordinates; // Leave this somewhere else, to reduce memory. it will still be cache friendly.
    glm::vec4 color;
    std::uint16_t identifier;
    //Layer layer; // TODO: Sort before rendering.

    Texture(const glm::vec2& offset, std::uint16_t identifier): vertices({glm::vec2(-8.0f, -8.0f) + offset, glm::vec2(8.0f, -8.0f) + offset, glm::vec2(8.0f, 8.0f) + offset, glm::vec2(8.0f, 8.0f) + offset, glm::vec2(-8.0f, 8.0f) + offset, glm::vec2(-8.0f, -8.0f) + offset}), coordinates({glm::vec2(0.0f, 0.0f) ,glm::vec2(1.0f, 0.0f) ,glm::vec2(1.0f, 1.0f) ,glm::vec2(1.0f, 1.0f) ,glm::vec2(0.0f, 1.0f) ,glm::vec2(0.0f, 0.0f)}), color(1.0f, 1.0f, 1.0f, 1.0f), identifier(identifier) {}
    Texture(std::uint16_t identifier): vertices({glm::vec2(-8.0f, -8.0f), glm::vec2(8.0f, -8.0f), glm::vec2(8.0f, 8.0f), glm::vec2(8.0f, 8.0f), glm::vec2(-8.0f, 8.0f), glm::vec2(-8.0f, -8.0f)}), coordinates({glm::vec2(0.0f, 0.0f) ,glm::vec2(1.0f, 0.0f) ,glm::vec2(1.0f, 1.0f) ,glm::vec2(1.0f, 1.0f) ,glm::vec2(0.0f, 1.0f) ,glm::vec2(0.0f, 0.0f)}), color(1.0f, 1.0f, 1.0f, 1.0f), identifier(identifier) {}
};





struct Parent
{
    Entity entity;

    Parent(): entity(entt::null) {}
    Parent(Entity entity): entity(entity) {}
};

struct Children
{
    std::vector<Entity> entities;
};


struct Anchor
{
    Entity entity;

    Anchor(): entity(entt::null) {}
    Anchor(Entity entity): entity(entity) {}
};







struct Mouse
{
    
};
*/




#endif




/*
struct Base
{
    std::string name;


    Base(const std::string& name): name(name) {}
};

struct Body: cp::Body
{
    Body(): cp::Body() {}
    Body(cp::Vector position): cp::Body(0.0, 0.0, position) {}
};    

struct Shape: cp::Shape
{

    Shape(const Body& body, const std::vector<cpVect>& vertices, cp::Float radius, const Material& material): cp::Shape(body, vertices, radius)
    {
        setMaterial(material);
    }

    Shape(const Body& body, cp::Float radius, const Material& material): cp::Shape(body, radius, cpvzero)
    {
        setMaterial(material);
    }

    void setMaterial(const Material& material)
    {
        setDensity(material.density);
        setFriction(material.friction);
        setElasticity(material.elasticity);
    }
};


struct Gear: cp::Gear
{
    Gear(const Body& bodyA): cp::Gear(bodyA) {}
    Gear(const Body& bodyA, const Body& bodyB): cp::Gear(bodyA, bodyB) {} // Remove this later.
};

struct Pivot: cp::Pivot
{
    Pivot(): cp::Pivot() {} // Remove this later.
    Pivot(const Body& bodyA, cpVect anchorA): cp::Pivot(bodyA, anchorA) {}
    Pivot(const Body& bodyA, const Body& bodyB, cpVect anchorA, cpVect anchorB): cp::Pivot(bodyA, bodyB, anchorA, anchorB) {} // Remove this later.
};

struct Pin: cp::Pin
{
    cpFloat distanceMultiplier;

    Pin(): cp::Pin() {} // Remove this later.
    Pin(const Body& bodyA, cpVect anchorA): cp::Pin(bodyA, anchorA) {}
    Pin(const Body& bodyA, const Body& bodyB, cpVect anchorA, cpVect anchorB): cp::Pin(bodyA, bodyB, anchorA, anchorB) {} // Remove this later.
};

struct Groove: cp::Groove
{
    Groove(): cp::Groove() {} // Remove this later.
    Groove(const Body& bodyA, cpVect grooveA, cpVect grooveB): cp::Groove(bodyA, grooveA, grooveB) {}
    Groove(const Body& bodyA, const Body& bodyB, cpVect grooveA, cpVect grooveB, cpVect anchorB): cp::Groove(bodyA, bodyB, grooveA, grooveB, anchorB) {} // Remove this later.
};

struct Motor: cp::Motor
{
    bool autobrake;

    Motor(const Body& bodyA): cp::Motor(bodyA) {}
    Motor(const Body& bodyA, double rate): cp::Motor(bodyA, rate) {}
};

struct Sprite
{
    std::array<glm::vec2, 6> vertices;
    std::array<glm::vec2, 6> coordinates; // Leave this somewhere else, to reduce memory. it will still be cache friendly.
    std::uint16_t identifier;
    //Layer layer; // TODO: Sort before rendering.

    Sprite(const glm::vec2& offset, std::uint16_t identifier): vertices({glm::vec2(-8.0f, -8.0f) + offset, glm::vec2(8.0f, -8.0f) + offset, glm::vec2(8.0f, 8.0f) + offset, glm::vec2(8.0f, 8.0f) + offset, glm::vec2(-8.0f, 8.0f) + offset, glm::vec2(-8.0f, -8.0f) + offset}), coordinates({glm::vec2(0.0f, 0.0f) ,glm::vec2(1.0f, 0.0f) ,glm::vec2(1.0f, 1.0f) ,glm::vec2(1.0f, 1.0f) ,glm::vec2(0.0f, 1.0f) ,glm::vec2(0.0f, 0.0f)}), identifier(identifier) {}
};

struct Model
{
    std::vector<Sprite> sprites;

    template<typename Container>Model(const Container& sprites): sprites(sprites.begin(), sprites.end()) {}
};











enum Action
{
    ROTATE_TO_MOUSE,
    KISS,
    ROTTA,
    TOGGLE_FURNACE,
    ROTATE_CLOCKWISE,
    ROTATE_ANTICLOCKWISE,
    ATTRACT_PIN,
    RETRACT_PIN
};



struct Input
{
    std::vector<sf::Event::EventType> types;
    std::vector<sf::Keyboard::Key> keys;
    std::vector<sf::Mouse::Button> buttons;
    std::vector<Action> actions;

    void bind(sf::Event::EventType type, Action action)
    {
        types.push_back(type);
        keys.push_back(sf::Keyboard::Unknown);
        buttons.push_back(sf::Mouse::Left);
        actions.push_back(action);
    }

    void bind(sf::Event::EventType type, sf::Keyboard::Key key, Action action)
    {
        types.push_back(type);
        keys.push_back(key);
        buttons.push_back(sf::Mouse::Left);
        actions.push_back(action);
    }    

    void bind(sf::Event::EventType type, sf::Mouse::Button button, Action action)
    {
        types.push_back(type);
        keys.push_back(sf::Keyboard::Unknown);
        buttons.push_back(button);
        actions.push_back(action);
    }
};

struct Apply
{
    std::vector<Action> actions;
};
*/

























/*
enum Button
{
    BOARD_Q_BUTTON,
    MOUSE_LEFT_BUTTON,
    STICK_X_BUTTON
};

enum Axis
{
    X_AXIS,
    Y_AXIS
};

enum Action
{
    BAJS_ACTION
};

enum State
{
    KISS_STATE
};

enum Range
{
    CYLINDER_FOLLOW_MOUSE_RANGE
};

struct Input
{
    std::unordered_map<Button, Action> actions;
    std::unordered_map<Button, State> states;
    std::unordered_map<Axis, Range> ranges;
};

struct Apply
{
    std::vector<Action> actions;
    std::map<State, bool> states;
    std::map<Range, float> ranges;
};
*/


/*

sf::Event::EventType type = sf::Event::KeyPressed;
sf::Keyboard::Key key = sf::Keyboard::Q;



sf::Event::EventType type = sf::Event::MouseButtonReleased;
sf::Mouse::Button button = sf::Mouse::Left;


sf::Event::EventType type = sf::Event::MouseWheelScrolled;



sf::Event::EventType type = sf::Event::MouseMoved;

*/

/*
struct Control // TODO: Change name to Player. Control will be used for entities that can control other entities, for example a stearing wheel or control panel.
{

};







struct Anchor
{
    Entity entity;
    cpVect position;

    Anchor(cpVect position): entity(entt::null), position(position) {}
    Anchor(Entity entity, cpVect position): entity(entity), position(position) {}
};

struct Anchors
{
    std::vector<Anchor> anchors; // Maybe change to unordered_map later? Key can be either name or position. Name for input events and position for attaching. Name needs the speed tho!

    Anchors() { anchors.emplace_back(cpv(16, 0)); }
    Anchors(const std::vector<Anchor>& anchors): anchors(anchors) {}
};

*/









/*

struct Base
{
    std::string name;

    Base(const std::string& name): name(name) {}

    std::ostream &operator<<(std::ostream &stream)
    { 
        return stream << name;
    }
};





struct Body: cp::Body
{
    Body(): cp::Body() {}
    Body(cp::Vector position): cp::Body(0.0, 0.0, position) {}
};    



struct Shape: cp::Shape
{
    cpVect offset;  // TEMP: I couldnt find vertices in chipmunk's internal, so storing those here for now.
    std::array<cpVect, 4> vertices; // TEMP: I couldnt find vertices in chipmunk's internal, so storing those here for now.

    Shape(const Body& body, const Material& material, const cpVect& offset = cpvzero): cp::Shape(body, std::vector<cpVect>({cpvadd(cpv(-8.0, -8.0), offset), cpvadd(cpv(8.0, -8.0), offset), cpvadd(cpv(8.0, 8.0), offset), cpvadd(cpv(-8.0, 8.0), offset)}), 0.1), vertices({cpvadd(cpv(-8.0, -8.0), offset), cpvadd(cpv(8.0, -8.0), offset), cpvadd(cpv(8.0, 8.0), offset), cpvadd(cpv(-8.0, 8.0), offset)}), offset(offset)
    {
        setMaterial(material);
    }

    void setMaterial(const Material& material)
    {
        setDensity(material.density);
        setFriction(material.friction);
        setElasticity(material.elasticity);
    }
};



struct Gear: cp::Gear
{
    Gear(const Body& bodyA): cp::Gear(bodyA) {}
    Gear(const Body& bodyA, const Body& bodyB): cp::Gear(bodyA, bodyB) {} // Remove this later.
};



struct Pivot: cp::Pivot
{
    Pivot(): cp::Pivot() {} // Remove this later.
    Pivot(const Body& bodyA, cpVect anchorA): cp::Pivot(bodyA, anchorA) {}
    Pivot(const Body& bodyA, const Body& bodyB, cpVect anchorA, cpVect anchorB): cp::Pivot(bodyA, bodyB, anchorA, anchorB) {} // Remove this later.
};



struct Pin: cp::Pin
{
    cpFloat distanceMultiplier;

    Pin(): cp::Pin() {} // Remove this later.
    Pin(const Body& bodyA, cpVect anchorA): cp::Pin(bodyA, anchorA) {}
    Pin(const Body& bodyA, const Body& bodyB, cpVect anchorA, cpVect anchorB): cp::Pin(bodyA, bodyB, anchorA, anchorB) {} // Remove this later.
};



struct Groove: cp::Groove
{
    Groove(): cp::Groove() {} // Remove this later.
    Groove(const Body& bodyA, cpVect grooveA, cpVect grooveB): cp::Groove(bodyA, grooveA, grooveB) {}
    Groove(const Body& bodyA, const Body& bodyB, cpVect grooveA, cpVect grooveB, cpVect anchorB): cp::Groove(bodyA, bodyB, grooveA, grooveB, anchorB) {} // Remove this later.
};



struct Motor: cp::Motor
{
    bool autobrake;

    Motor(const Body& bodyA): cp::Motor(bodyA) {}
    Motor(const Body& bodyA, double rate): cp::Motor(bodyA, rate) {}
};


struct Texture
{
    std::array<glm::fvec2, 6> vertices;
    std::array<glm::fvec2, 6> coordinates; // Leave this somewhere else, to reduce memory. it will still be cache friendly.
    glm::vec4 color;
    std::uint16_t identifier;
    //Layer layer; // TODO: Sort before rendering.

    Texture(const glm::vec2& offset, std::uint16_t identifier): vertices({glm::vec2(-8.0f, -8.0f) + offset, glm::vec2(8.0f, -8.0f) + offset, glm::vec2(8.0f, 8.0f) + offset, glm::vec2(8.0f, 8.0f) + offset, glm::vec2(-8.0f, 8.0f) + offset, glm::vec2(-8.0f, -8.0f) + offset}), coordinates({glm::vec2(0.0f, 0.0f) ,glm::vec2(1.0f, 0.0f) ,glm::vec2(1.0f, 1.0f) ,glm::vec2(1.0f, 1.0f) ,glm::vec2(0.0f, 1.0f) ,glm::vec2(0.0f, 0.0f)}), color(1.0f, 1.0f, 1.0f, 1.0f), identifier(identifier) {}
    Texture(std::uint16_t identifier): vertices({glm::vec2(-8.0f, -8.0f), glm::vec2(8.0f, -8.0f), glm::vec2(8.0f, 8.0f), glm::vec2(8.0f, 8.0f), glm::vec2(-8.0f, 8.0f), glm::vec2(-8.0f, -8.0f)}), coordinates({glm::vec2(0.0f, 0.0f) ,glm::vec2(1.0f, 0.0f) ,glm::vec2(1.0f, 1.0f) ,glm::vec2(1.0f, 1.0f) ,glm::vec2(0.0f, 1.0f) ,glm::vec2(0.0f, 0.0f)}), color(1.0f, 1.0f, 1.0f, 1.0f), identifier(identifier) {}
};





struct Parent
{
    Entity entity;

    Parent(): entity(entt::null) {}
    Parent(Entity entity): entity(entity) {}
};

struct Children
{
    std::vector<Entity> entities;
};


struct Anchor
{
    Entity entity;

    Anchor(): entity(entt::null) {}
    Anchor(Entity entity): entity(entity) {}
};







struct Mouse
{
    
};









#endif
*/
/*



cpCollisionType: Unique identifier for collision shape types. Defaults to unsigned int. Defined type must support the == operator.
cpGroup: Unique identifier for collision groups. Defaults to unsigned int. A CP_NO_GROUP value is defined that can be used when you don’t want to specify a group. Defined type must support the equality == operator.
cpBitmask: Type used for collision filter categories and masks. Defaults to unsigned int. Defined type must support the bitwise AND & operator.

Chipmunk has two primary means of ignoring collisions: groups and category masks.
Groups are used to ignore collisions between parts on a complex object. 
Category masks allow you to mark which categories an object belongs to and which categories it collidies with.



cpShapeSetFilter (cpShape *shape, cpShapeFilter filter)
cpShapeSetCollisionType (cpShape *shape, cpCollisionType collisionType)
cpShapeFilterNew (cpGroup group, cpBitmask categories, cpBitmask mask)




*/