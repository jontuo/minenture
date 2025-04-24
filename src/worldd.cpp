
#include "world.hpp"
#include "chipmunk/chipmunk_types.h"
#include <iostream>





#include "model.hpp"
#include "body.hpp"
#include "parent.hpp"

#include "chipmunkpp/body.hpp"
#include "chipmunkpp/shape.hpp"
#include "chipmunkpp/pin.hpp"







World::World():
    space(),
    mouseBody(),
    mousePivot(mouseBody, cpvzero),
    mouseGear(mouseBody)
{
    name = "Test";
    space.setGravity(cpv(0.0, 9.8));

    mouseBody.setType(CP_BODY_TYPE_KINEMATIC);
    space.add(mouseBody);


    

}

World::~World()
{
    if(space.contains(mousePivot))
    {
        space.remove(mousePivot);
    }

    if(space.contains(mouseGear))
    {
        space.remove(mouseGear);
    }

    if(space.contains(mouseBody))
    {
        space.remove(mouseBody);
    }

    entt::View<Entity, cp::Pin> pinView = registry.view<cp::Pin>();
    for(Entity entityIdentifier: pinView)
    {
        cp::Pin& entityPin = pinView.get(entityIdentifier);
        if(space.contains(entityPin) == cpTrue)
        {
            space.remove(entityPin);
        }
    }

    entt::View<Entity, cp::Pivot> pivotView = registry.view<cp::Pivot>();
    for(Entity entityIdentifier: pivotView)
    {
        cp::Pivot& entityPivot = pivotView.get(entityIdentifier);
        if(space.contains(entityPivot) == cpTrue)
        {
            space.remove(entityPivot);
        }
    }

    entt::View<Entity, cp::Shape> shapeView = registry.view<cp::Shape>();
    for(Entity entityIdentifier: shapeView)
    {
        cp::Shape& entityShape = shapeView.get(entityIdentifier);
        if(space.contains(entityShape) == cpTrue)
        {
            space.remove(entityShape);
        }
    }

    auto bodyView = registry.view<cp::Body>();
    for(Entity entityIdentifier: bodyView)
    {
        cp::Body& entityBody = bodyView.get(entityIdentifier);
        if(space.contains(entityBody) == cpTrue)
        {
            space.remove(entityBody);
        }
    }
}

World::World(const World& world)
{

}

World& World::operator=(World &other)
{
    return(*this);
}

World& World::operator=(World&& other)
{
    return(*this);
}



World::World(World&& world)
{

}



void World::update(double d)
{
    space.step(d);
}


Entity World::createCylinder(cpVect position)
{
    Entity aEntity = registry.create();
    cp::Body& aBody = registry.assign<cp::Body>(aEntity, position);
    space.add(aBody);

    Entity bEntity = registry.create();
    cp::Shape& bShape = registry.assign<cp::Shape>(bEntity, aBody, cpv(0, 0));
    Model& bModel = registry.assign<Model>(bEntity, Mesh(cpv(16, 16), cpv(0, 0)), Texture(CYLINDER_LEFT_TEXTURE));
    space.add(bShape);
    Parent& bParent = registry.assign<Parent>(bEntity, aEntity);
    cp::Pivot& bPivot = registry.assign<cp::Pivot>(bEntity, aBody, cpv(0, 0));

    Entity cEntity = registry.create();
    cp::Shape& cShape = registry.assign<cp::Shape>(cEntity, aBody, cpv(16, 0));
    Model& cModel = registry.assign<Model>(cEntity, Mesh(cpv(16, 16), cpv(16, 0)), Texture(CYLINDER_MIDDLE_TEXTURE));
    space.add(cShape);
    Parent& cParent = registry.assign<Parent>(cEntity, aEntity);

    Entity dEntity = registry.create();
    cp::Shape& dShape = registry.assign<cp::Shape>(dEntity, aBody, cpv(32, 0));
    Model& dModel = registry.assign<Model>(dEntity, Mesh(cpv(16, 16), cpv(32, 0)), Texture(CYLINDER_RIGHT_TEXTURE));
    space.add(dShape);
    Parent& dParent = registry.assign<Parent>(dEntity, aEntity);

    return(aEntity);
}

Entity World::createRod(cpVect position)
{
    Entity aEntity = registry.create();
    cp::Body& aBody = registry.assign<cp::Body>(aEntity, position);
    space.add(aBody);
    cp::Shape& aShape = registry.assign<cp::Shape>(aEntity, aBody, cpv(0.0, 0.0));
    space.add(aShape);

    Entity bEntity = registry.create();
    Model& bModel = registry.assign<Model>(bEntity, Mesh(cpv(16.0, 16.0), cpv(-16.0, 0.0)), Texture(ROD_LEFT_TEXTURE));
    Parent& bParent = registry.assign<Parent>(bEntity, aEntity);

    Entity cEntity = registry.create();
    Model& cModel = registry.assign<Model>(cEntity, Mesh(cpv(16.0, 16.0), cpv(0.0, 0.0)), Texture(ROD_MIDDLE_TEXTURE));
    Parent& cParent = registry.assign<Parent>(cEntity, aEntity);

    Entity dEntity = registry.create();
    Model& dModel = registry.assign<Model>(dEntity, Mesh(cpv(16.0, 16.0), cpv(16.0, 0.0)), Texture(ROD_RIGHT_TEXTURE));
    Parent& dParent = registry.assign<Parent>(dEntity, aEntity);
}

Entity World::createPiston(cpVect position)
{
    // Maybe add body/shape to space here? Or maybe even further out (the place which calls this function)?




}


void World::fill(const cpVect& position, const cpVect& size, cpBodyType type, unsigned int texture)
{
    for(cpFloat currentY = position.y; currentY < size.y * 16.0 + position.y; currentY = currentY + 16.0)
    {
        for(cpFloat currentX = position.x; currentX < size.x * 16.0 + position.x; currentX = currentX + 16.0)
        {
            Entity entity = registry.create();

            cp::Body& body = registry.assign<cp::Body>(entity, cpv(currentX, currentY), type);
            space.add(body);

            cp::Shape& shape = registry.assign<cp::Shape>(entity, body, cpv(0.0, 0.0));
            space.add(shape);

            registry.assign<Model>(entity, Mesh(cpv(16.0, 16.0), cpv(0.0, 0.0)), Texture(texture));
        }
    }
}

Entity World::createBase(cpVect position)
{
    Entity aEntity = registry.create();
    cp::Body& aBody = registry.assign<cp::Body>(aEntity, position);
    aBody.setType(CP_BODY_TYPE_STATIC);
    space.add(aBody);

    Entity bEntity = registry.create();
    cp::Shape& bShape = registry.assign<cp::Shape>(bEntity, aBody, cpv(0, 0));
    space.add(bShape);
    Model& bModel = registry.assign<Model>(bEntity, Mesh(cpv(16, 16), cpv(0, 0)), Texture(281));

    Entity cEntity = registry.create();
    cp::Shape& cShape = registry.assign<cp::Shape>(cEntity, aBody, cpv(16, 16));
    space.add(cShape);
    Model& cModel = registry.assign<Model>(cEntity, Mesh(cpv(16, 16), cpv(-16, 16)), Texture(320));

    Entity dEntity = registry.create();
    cp::Shape& dShape = registry.assign<cp::Shape>(dEntity, aBody, cpv(0, 16));
    space.add(dShape);
    Model& dModel = registry.assign<Model>(dEntity, Mesh(cpv(16, 16), cpv(0, 16)), Texture(321));

    Entity eEntity = registry.create();
    cp::Shape& eShape = registry.assign<cp::Shape>(eEntity, aBody, cpv(16.0, 16));
    space.add(eShape);
    Model& eModel = registry.assign<Model>(eEntity, Mesh(cpv(16, 16), cpv(16, 16)), Texture(322));

    return(aEntity);
}

void World::test()
{

    fill(cpv(16 * 2, 16 * 2), cpv(1, 60), CP_BODY_TYPE_STATIC, DIRT_TEXTURE);
    fill(cpv(16 * 3, 16 * 62), cpv(115, 1), CP_BODY_TYPE_STATIC, DIRT_TEXTURE);
    fill(cpv(118 * 16, 16 * 2), cpv(1, 60), CP_BODY_TYPE_STATIC, DIRT_TEXTURE);

    Entity baseEntity = createBase(cpv(500, 16 * 60));
    Entity cylinderEntity = createCylinder(cpv(500, 16 * 60));

    /*Entity aEntity = createBase(cpv(500.0, 500.0));
    cpBody* aBodyHandle = registry.get<cp::Body>(aEntity).handle;
    
    createCylinder(cpv(516.0, 500.0));
    //cpBody* bBodyHandle = registry.get<cp::Body>(1).handle;
    //cp::Pin& bPin = registry.assign<cp::Pin>(2, cp::Body(bBodyHandle), cp::Body(aBodyHandle), cpv(0.0, 0.0), cpv(0.0, 0.0));
    cp::Pivot& bPivot = registry.get<cp::Pivot>(2);
    bPivot.setBodyB(cp::Body(aBodyHandle));
    bPivot.setAnchorB(cpv(0.0, 0.0));
    bPivot.setCollideBodies(cpFalse);
    space.add(bPivot);*/
















































    /*

    This might be better!!!
    Add a Anchor to each subentity, for example the edge of a vehicle and the edge of a cylinder.
    When the two are constrained together, let them have a smart pointer (shared or whatever its called, the ref counting one).
    If both are gone, destroy&free the cpConstraint.


    Maybe let Anchor hold entt::Prototypes (std::vector), so it knows what to create.



    */








    // Maybe have shapes in each children too, but dont add them to space? Only for getNearestShape(mousePosition);
    // Maybe have anchor on a child, storing an identifier to a constraint entity. the constraint entity stores two identifiers, parent and child.
    // Or do the simpler method of the above, store constraints in the child entity, but im not sure if that is good.
    // Remember when a player sits in a vehicle, its not a parent! its a constraint!!!
    // Parenting are permanent (maybe not pipes and such?) and constraints are temporarly?
    // There can only exists one body in a family (parenting)?

/*



    Each child has a shape.
        Easier to find body when rendering.
        Easier to make more complex geometric entities.
        Easier to make a hollow container, for example.
        Easier to find which part of entity is clicked with mouse.
        Can use its origin/offset/transform to position the model for rendering.
        Harder for chipmunk to process physics.
        Do not add to space for no collision.




*/


    /*std::vector<entt::DefaultPrototype> prototypes;

    entt::DefaultPrototype aPrototype;
    aPrototype.set<cp::Body>();
    aPrototype.set<cp::Shape();
    prototypes.push_back(aPrototype);

    entt::DefaultPrototype bPrototype;
    bPrototype.set<Model>(Mesh(cpv(16.0, 16.0), cpv(-16.0, 0.0)), Texture(CYLINDER_LEFT_TEXTURE));
    bPrototype.set<Parent>(0);
    prototypes.push_back(bPrototype);

    entt::DefaultPrototype bPrototype;
    bPrototype.set<Model>(Mesh(cpv(16.0, 16.0), cpv(0.0, 0.0)), Texture(CYLINDER_MIDDLE_TEXTURE));
    bPrototype.set<Parent>(0);
    prototypes.push_back(cPrototype);

    entt::DefaultPrototype bPrototype;
    bPrototype.set<Model>(Mesh(cpv(16.0, 16.0), cpv(16.0, 0.0)), Texture(CYLINDER_RIGHT_TEXTURE));
    bPrototype.set<Parent>(0);
    prototypes.push_back(dPrototype);

    // The factory will walk backwards and check if each has cp::Body until it finds it?
    // It will also create the cpBody and cpShape? add to space too?

    for(entt::DefaultPrototype prototype: prototypes)
    {
        if(prototype.has<cp::Body>() == true)
        {
            std::cout << "yes" << std::endl;
        }
    }*/




/*
    Entity aEntity = registry.create();
    cp::Body& aBody = registry.assign<cp::Body>(aEntity, cpv(500.0, 500.0));
    space.add(aBody);
    cp::Shape& aShape = registry.assign<cp::Shape>(aEntity, aBody);
    space.add(aShape);

    Entity bEntity = registry.create();
    Model& bModel = registry.assign<Model>(bEntity, Mesh(cpv(16.0, 16.0), cpv(-16.0, 0.0)), Texture(CYLINDER_LEFT_TEXTURE));
    Parent& bParent = registry.assign<Parent>(bEntity, aEntity);

    Entity cEntity = registry.create();
    Model& cModel = registry.assign<Model>(cEntity, Mesh(cpv(16.0, 16.0), cpv(0.0, 0.0)), Texture(CYLINDER_MIDDLE_TEXTURE));
    Parent& cParent = registry.assign<Parent>(cEntity, aEntity);

    Entity dEntity = registry.create();
    Model& dModel = registry.assign<Model>(dEntity, Mesh(cpv(16.0, 16.0), cpv(16.0, 0.0)), Texture(CYLINDER_RIGHT_TEXTURE));
    Parent& dParent = registry.assign<Parent>(dEntity, aEntity);
*/





    // Check if chipmunk's boxShape helper does something special to make physics perform faster, since its truely is a box.
    // Why i dont just transform model vertices on creation? and then you just have to cpLocalToWorld(body, vert[]); thats how shape is doing... do the same???? wtf???


/*
    Entity aEntity = registry.create();
    cp::Body& aBody = registry.assign<cp::Body>(aEntity, cpv(500.0, 500.0));
    space.add(aBody);

    Entity bEntity = registry.create();
    cp::Shape& bShape = registry.assign<cp::Shape>(bEntity, aBody);
    space.add(bShape);
    Model& bModel = registry.assign<Model>(bEntity, Mesh(cpv(16.0, 16.0), cpv(-16.0, 0.0)), Texture(CYLINDER_LEFT_TEXTURE));

    Entity cEntity = registry.create();
    cp::Shape& cShape = registry.assign<cp::Shape>(cEntity, aBody);
    space.add(cShape);
    Model& cModel = registry.assign<Model>(cEntity, Mesh(cpv(16.0, 16.0), cpv(0.0, 0.0)), Texture(CYLINDER_MIDDLE_TEXTURE));

    Entity dEntity = registry.create();
    cp::Shape& dShape = registry.assign<cp::Shape>(dEntity, aBody);
    space.add(dShape);
    Model& dModel = registry.assign<Model>(dEntity, Mesh(cpv(16.0, 16.0), cpv(16.0, 0.0)), Texture(CYLINDER_RIGHT_TEXTURE));
*/


    //Anchor& bAnchor = registry.assign<Anchor>(bEntity);
    //Pin& cPin = registry.assign<Pin>(bEntity, aBody, ba.position);
    


    // Do each fking sprite its own entity, try it!!!
    // it will probably fix that problem with anchor points and shit.
    // also might be easier on cache misses.

    // have a main entity (without a model) and child entities with models? one of the child entities will have an anchor and constraints.

    /*Entity cylinderLeftIdentifier = registry.create();
    cp::Body& cylinderLeftBody = registry.assign<cp::Body>(cylinderLeftIdentifier, cpv(500.0, 500.0));
    cp::Shape& cylinderLeftShape = registry.assign<cp::Shape>(cylinderLeftIdentifier, cylinderBody, 16.0 * 6, 16.0 * 1.0, 0.1);
    Model& cylinderModel = registry.assign<Model>(cylinderIdentifier);
    cylinderModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, CYLINDER_LEFT_TEXTURE, glm::vec2(16.0f * -2.5f, 16.0f * 0.0f));
    cylinderModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, CYLINDER_MIDDLE_TEXTURE, glm::vec2(16.0f * -1.5f, 16.0f * 0.0f));
    cylinderModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, CYLINDER_MIDDLE_TEXTURE, glm::vec2(16.0f * -0.5f, 16.0f * 0.0f));
    cylinderModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, CYLINDER_MIDDLE_TEXTURE, glm::vec2(16.0f * 0.5f, 16.0f * 0.0f));
    cylinderModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, CYLINDER_MIDDLE_TEXTURE, glm::vec2(16.0f * 1.5f, 16.0f * 0.0f));
    cylinderModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, CYLINDER_RIGHT_TEXTURE, glm::vec2(16.0f * 2.5f, 16.0f * 0.0f));

    space.add(cylinderBody);
    space.add(cylinderShape);

*/

/*
    Entity cylinderIdentifier = registry.create();
    cp::Body& cylinderBody = registry.assign<cp::Body>(cylinderIdentifier, cpv(500.0, 500.0));
    cylinderBody.setType(CP_BODY_TYPE_STATIC);
    cylinderBody.setUserData(reinterpret_cast<void*>(static_cast<std::uintptr_t>(cylinderIdentifier)));
    cp::Shape& cylinderShape = registry.assign<cp::Shape>(cylinderIdentifier, cylinderBody, 16.0 * 6, 16.0 * 1.0, 0.1);
    cylinderShape.setUserData(reinterpret_cast<void*>(static_cast<std::uintptr_t>(cylinderIdentifier)));
    Model& cylinderModel = registry.assign<Model>(cylinderIdentifier);
    cylinderModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, CYLINDER_LEFT_TEXTURE, glm::vec2(16.0f * -2.5f, 16.0f * 0.0f));
    cylinderModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, CYLINDER_MIDDLE_TEXTURE, glm::vec2(16.0f * -1.5f, 16.0f * 0.0f));
    cylinderModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, CYLINDER_MIDDLE_TEXTURE, glm::vec2(16.0f * -0.5f, 16.0f * 0.0f));
    cylinderModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, CYLINDER_MIDDLE_TEXTURE, glm::vec2(16.0f * 0.5f, 16.0f * 0.0f));
    cylinderModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, CYLINDER_MIDDLE_TEXTURE, glm::vec2(16.0f * 1.5f, 16.0f * 0.0f));
    cylinderModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, CYLINDER_RIGHT_TEXTURE, glm::vec2(16.0f * 2.5f, 16.0f * 0.0f));

    space.add(cylinderBody);
    space.add(cylinderShape);

    Entity pistonIdentifier = registry.create();
    cp::Body& pistonBody = registry.assign<cp::Body>(pistonIdentifier, cpv(500.0, 540.0));
    pistonBody.setType(CP_BODY_TYPE_STATIC);
    pistonBody.setUserData(reinterpret_cast<void*>(static_cast<std::uintptr_t>(pistonIdentifier)));
    cp::Shape& pistonShape = registry.assign<cp::Shape>(pistonIdentifier, pistonBody, 16.0 * 6, 16.0 * 0.75, 0.1);
    pistonShape.setUserData(reinterpret_cast<void*>(static_cast<std::uintptr_t>(pistonIdentifier)));
    Model& pistonModel = registry.assign<Model>(pistonIdentifier);
    pistonModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, PISTON_LEFT_TEXTURE, glm::vec2(16.0f * -2.5f, 16.0f * 0.0f));
    pistonModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, PISTON_MIDDLE_TEXTURE, glm::vec2(16.0f * -1.5f, 16.0f * 0.0f));
    pistonModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, PISTON_MIDDLE_TEXTURE, glm::vec2(16.0f * -0.5f, 16.0f * 0.0f));
    pistonModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, PISTON_MIDDLE_TEXTURE, glm::vec2(16.0f * 0.5f, 16.0f * 0.0f));
    pistonModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, PISTON_MIDDLE_TEXTURE, glm::vec2(16.0f * 1.5f, 16.0f * 0.0f));
    pistonModel.meshes.emplace_back(SQUARE_VERTICES, SQUARE_COORDINATES, PISTON_RIGHT_TEXTURE, glm::vec2(16.0f * 2.5f, 16.0f * 0.0f));

    space.add(pistonBody);
    space.add(pistonShape);
*/


/*
    Entity ground = registry.create();
    cp::Body& groundBody = registry.assign<cp::Body>(ground, cpv(500.0, 500.0));
    groundBody.setType(CP_BODY_TYPE_STATIC);
    groundBody.setUserData(reinterpret_cast<void*>(static_cast<std::uintptr_t>(ground)));
    cp::Shape& groundShape = registry.assign<cp::Shape>(ground, groundBody, 16.0f, 16.0f, 1.0f);
    groundShape.setUserData(reinterpret_cast<void*>(static_cast<std::uintptr_t>(ground)));
    Model& groundModel = registry.assign<Model>(ground, Mesh(SQUARE_MESH_VERTICES), Texture(STONE_TEXTURE_IDENTIFIER, SQUARE_TEXTURE_COORDINATES));


    //std::uint32_t entityIdentifier = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(peer.getUserData()));
    //entityNetwork.peer.setUserData(reinterpret_cast<void*>(static_cast<std::uintptr_t>(entity)));

    space.add(groundBody);
    space.add(groundShape);



    Entity stone = registry.create();
    cp::Body& stoneBody = registry.assign<cp::Body>(stone, cpv(500.0, 470.0));
    stoneBody.setUserData(reinterpret_cast<void*>(static_cast<std::uintptr_t>(stone)));
    cp::Shape& stoneShape = registry.assign<cp::Shape>(stone, stoneBody, 16.0f, 16.0f, 1.0f);
    stoneShape.setUserData(reinterpret_cast<void*>(static_cast<std::uintptr_t>(stone)));
    Model& stoneModel = registry.assign<Model>(stone, Mesh(SQUARE_MESH_VERTICES), Texture(STONE_TEXTURE_IDENTIFIER, SQUARE_TEXTURE_COORDINATES));


    space.add(stoneBody);
    space.add(stoneShape);
*/


}

void World::testt()
{

    cp::Body& body = registry.get<cp::Body>(0);
    body.setType(CP_BODY_TYPE_DYNAMIC);


}


std::experimental::optional<Entity> World::getNearestEntity(float positionX, float positionY, float maxDistance)
{
    std::experimental::optional<cp::Shape> shape = space.pointQueryNearest(cpv(positionX, positionY), maxDistance, CP_SHAPE_FILTER_ALL, NULL);

    if(shape)
    {
        return(std::experimental::optional<Entity>(static_cast<Entity>(reinterpret_cast<std::uintptr_t>(shape->getUserData()))));
    }
    else
    {
        return(std::experimental::optional<Entity>());
    }
}


/*


Adding and removing shapes and bodies to space might be easier with this method (proposed by Slmbecke):
Let Shape store ref~ to body, then when adding to space, use Shape::body
See example at https://chipmunk-physics.net/forum/viewtopic.php?f=1&t=891

If I want to rotate the cylinder, check if it has Parent, if parent has Gear, set "rotate" state to true.
Then in system, loop all Gear components, check state, apply action.

Mapping input actions to correct entity, will work if you have a "Action/Map/Interact"- component to that entity.
Then in system, loop all entities iwht that comp, check if, set state.


// mass and moment should be calculated in prefab!
// Composite object. 1x4 box centered on the center of gravity and a circle sitting on top.
// Just add the moments together.
// cpFloat composite = cpMomentForBox(boxMass, 1, 4) + cpMomentForCircle(circleMass, 0, 1, cpv(0, 3));


// All bodies should be dynamic type, even those that suppose to be static.
// As static: Do not add body to space. Add shape as static to space.
// As dynamic: Add body to space. Add shape as normal to space.
// From static to dynamic: Add body to space, remove static shape, add nromal shape.
// From dynamic to static: snap to grid, removr body, remove normal shape, add static shape.
// Not sure about mass has to be set to infinity during one of these steps, use cpBodySetMass() if so.
// https://chipmunk-physics.net/forum/viewtopic.php?f=1&t=891
// http://chipmunk-physics.net/forum/viewtopic.php?t=1533


*/
