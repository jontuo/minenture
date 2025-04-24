
#include "world.hpp"                //
#include <experimental/optional>    // std::experimental::optional<>
#include "chipmunk/chipmunk.h"
#include <iostream>                 // std::cout

// Components
#include "base.hpp"
#include "body.hpp"
#include "shape.hpp"
#include "texture.hpp"
#include "component.hpp"            // TMP!!!


World::World(const std::vector<Definition>& definitions):
    registry(),
    space(cpv(0.0, 235.2)),
    definitions(definitions)
{


}

World::~World()
{

}



void World::update(double deltaTime)
{
    space.update(deltaTime);
}

void World::test()
{
    createTerrain(cpv(10, 40), cpv(40, 2));
    createTerrain(cpv(5, 50), cpv(20, 2));
    createTerrain(cpv(1.5, 1.5), cpv(100, 60));

    createEntity(CYLINDER_ENTITY, cpv(500, 600));
    createEntity(VEHICLE_ENTITY, cpv(500, 500));

    createEntity(BELT_ENTITY, cpv(16*6, 16*51));

    create(definitions[0], cpv(500, 500));
}



Entity World::create(unsigned int definition, const cpVect& position)
{
    return(create(definitions[definition], position));
}

Entity World::create(const Definition& definition, const cpVect& position)
{
    Entity entity = registry.create();

    if(definition.base)
    {
        registry.assign<Base>(entity, definition.base->name);
    }

    if(definition.body)
    {
        Body& body = registry.assign<Body>(entity, position);
        space.add(body);

        if(definition.shape)
        {
            Shape& shape = registry.assign<Shape>(entity, body, definition.shape->vertices, definition.shape->bevel, definition.shape->density, definition.shape->friction, definition.shape->elasticity);
            space.add(shape);
        }

        if(definition.textures.size() == 1)
        {
            registry.assign<Texture>(entity, definition.textures[0].identifier);
        }
        else if(definition.textures.size() > 1)
        {
            Textures& tt = registry.assign<Textures>(entity);

            for(const Definition::Texture& bb: definition.textures)
            {
                tt.textures.emplace_back(bb.identifier);
            }
            
        }
    }


    return(entity);
}









Entity World::createEntity(EntityType type, const cpVect& position)
{
    Entity entity = registry.create();

    switch(type)
    {
        case DIRT_ENTITY:
        {
            Body& body = registry.assign<Body>(entity, position);
            Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-8.0, -8.0), cpv(8.0, -8.0), cpv(8.0, 8.0), cpv(-8.0, 8.0)}), 0.1, 3.125, 1.0, 0.0);
            registry.assign<Texture>(entity, 127);

            space.add(body);
            space.add(shape);

            break;
        }

        case STONE_ENTITY:
        {
            Body& body = registry.assign<Body>(entity, position);
            Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-8.0, -8.0), cpv(8.0, -8.0), cpv(8.0, 8.0), cpv(-8.0, 8.0)}), 0.1, 7.2, 1.0, 0.0);
            registry.assign<Texture>(entity, 131);

            space.add(body);
            space.add(shape);

            break;
        }

        case WALL_ENTITY:
        {
            Body& body = registry.assign<Body>(entity, position, CP_BODY_TYPE_STATIC);
            Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-8.0, -8.0), cpv(8.0, -8.0), cpv(8.0, 8.0), cpv(-8.0, 8.0)}), 0.1, 7.2, 1.0, 0.0);
            registry.assign<Texture>(entity, 49);

            space.add(body);
            space.add(shape);

            break;
        }

        case VEHICLE_ENTITY:
        {
            Body& body = registry.assign<Body>(entity, position);   
            Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-88.0, 24.0), cpv(-88.0, 8.0), cpv(-72.0, 8.0), cpv(-56.0, -8.0), cpv(-40.0, -8.0), cpv(-24.0, 8.0), cpv(8.0, 8.0), cpv(24.0, -8.0), cpv(40.0, -8.0), cpv(56.0, -24.0), cpv(88.0, -24.0), cpv(88.0, 24.0)}), 0.1, 5.7, 1.0, 0.0);
            Textures& textures = registry.assign<Textures>(entity);
            textures.textures.emplace_back(170, glm::vec2(80, -16));
            textures.textures.emplace_back(169, glm::vec2(64, -16));
            textures.textures.emplace_back(168, glm::vec2(48, -16));
            textures.textures.emplace_back(201, glm::vec2(-64, 0));
            textures.textures.emplace_back(202, glm::vec2(-48, 0));
            textures.textures.emplace_back(203, glm::vec2(-32, 0));
            textures.textures.emplace_back(206, glm::vec2(16, 0));
            textures.textures.emplace_back(207, glm::vec2(32, 0));
            textures.textures.emplace_back(208, glm::vec2(48, 0));
            textures.textures.emplace_back(209, glm::vec2(64, 0));
            textures.textures.emplace_back(210, glm::vec2(80, 0));
            textures.textures.emplace_back(240, glm::vec2(-80, 16));
            textures.textures.emplace_back(241, glm::vec2(-64, 16));
            textures.textures.emplace_back(242, glm::vec2(-48, 16));
            textures.textures.emplace_back(243, glm::vec2(-32, 16));
            textures.textures.emplace_back(244, glm::vec2(-16, 16));
            textures.textures.emplace_back(245, glm::vec2(0, 16));
            textures.textures.emplace_back(246, glm::vec2(16, 16));
            textures.textures.emplace_back(247, glm::vec2(32, 16));
            textures.textures.emplace_back(248, glm::vec2(48, 16));
            textures.textures.emplace_back(249, glm::vec2(64, 16));
            textures.textures.emplace_back(250, glm::vec2(80, 16));

            space.add(body);
            space.add(shape);

            break;
        }

        case CYLINDER_ENTITY:
        {
            Body& body = registry.assign<Body>(entity, position);
            Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(24, 8), cpv(-24, 8), cpv(-24, -8), cpv(24, -8)}), 0.1, 1.1, 1.0, 0.0);
            Textures& textures = registry.assign<Textures>(entity);
            textures.textures.emplace_back(172, glm::vec2(-16, 0));
            textures.textures.emplace_back(173, glm::vec2(0, 0));
            textures.textures.emplace_back(174, glm::vec2(16, 0));

            space.add(body);
            space.add(shape);

            break;
        }

        case BELT_ENTITY:
        {    
            Body& body = registry.assign<Body>(entity, position, CP_BODY_TYPE_STATIC);
            Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-8.0, -8.0), cpv(8.0, -8.0), cpv(8.0, 8.0), cpv(-8.0, 8.0)}), 0.0, 7.2, 1.0, 0.0);
            Textures& textures = registry.assign<Textures>(entity);
            textures.textures.emplace_back(43, glm::vec2(0, 0));

            shape.setSurfaceVelocity(cpv(16.0, 0.0));

            space.add(body);
            space.add(shape);

            break;
        }

        default:
        {
            std::cout << "ERROR: Failed to create entity, type isnt declared yet." << std::endl;
        }
    }

    return(entity);
}


Entity World::createBelt(const cpVect& position)
{
    Entity entity = registry.create();

    Body& body = registry.assign<Body>(entity, position, CP_BODY_TYPE_STATIC);
    Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-8.0, -8.0), cpv(8.0, -8.0), cpv(8.0, 8.0), cpv(-8.0, 8.0)}), 0.0, 7.2, 1.0, 0.0);
    Textures& textures = registry.assign<Textures>(entity);
    textures.textures.emplace_back(43, glm::vec2(0, 0));

    shape.setSurfaceVelocity(cpv(16.0, 0.0));

    space.add(body);
    space.add(shape);

    return(entity);
}

Entity World::createVehicle(const cpVect& position)
{
    Entity entity = registry.create();

    Body& body = registry.assign<Body>(entity, position, CP_BODY_TYPE_STATIC);   
    Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-88.0, 24.0), cpv(-88.0, 8.0), cpv(-72.0, 8.0), cpv(-56.0, -8.0), cpv(-40.0, -8.0), cpv(-24.0, 8.0), cpv(8.0, 8.0), cpv(24.0, -8.0), cpv(40.0, -8.0), cpv(56.0, -24.0), cpv(88.0, -24.0), cpv(88.0, 24.0)}), 0.1, 5.7, 1.0, 0.0);
    Textures& textures = registry.assign<Textures>(entity);
    textures.textures.emplace_back(170, glm::vec2(80, -16));
    textures.textures.emplace_back(169, glm::vec2(64, -16));
    textures.textures.emplace_back(168, glm::vec2(48, -16));
    textures.textures.emplace_back(201, glm::vec2(-64, 0));
    textures.textures.emplace_back(202, glm::vec2(-48, 0));
    textures.textures.emplace_back(203, glm::vec2(-32, 0));
    textures.textures.emplace_back(206, glm::vec2(16, 0));
    textures.textures.emplace_back(207, glm::vec2(32, 0));
    textures.textures.emplace_back(208, glm::vec2(48, 0));
    textures.textures.emplace_back(209, glm::vec2(64, 0));
    textures.textures.emplace_back(210, glm::vec2(80, 0));
    textures.textures.emplace_back(240, glm::vec2(-80, 16));
    textures.textures.emplace_back(241, glm::vec2(-64, 16));
    textures.textures.emplace_back(242, glm::vec2(-48, 16));
    textures.textures.emplace_back(243, glm::vec2(-32, 16));
    textures.textures.emplace_back(244, glm::vec2(-16, 16));
    textures.textures.emplace_back(245, glm::vec2(0, 16));
    textures.textures.emplace_back(246, glm::vec2(16, 16));
    textures.textures.emplace_back(247, glm::vec2(32, 16));
    textures.textures.emplace_back(248, glm::vec2(48, 16));
    textures.textures.emplace_back(249, glm::vec2(64, 16));
    textures.textures.emplace_back(250, glm::vec2(80, 16));

    space.add(body);
    space.add(shape);

    return(entity);
}

Entity World::createCylinder(const cpVect& position)
{
    Entity entity = registry.create();

    Body& body = registry.assign<Body>(entity, position, CP_BODY_TYPE_STATIC);
    Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(24, 8), cpv(-24, 8), cpv(-24, -8), cpv(24, -8)}), 0.1, 1.1, 1.0, 0.0);
    Textures& textures = registry.assign<Textures>(entity);
    textures.textures.emplace_back(172, glm::vec2(-16, 0));
    textures.textures.emplace_back(173, glm::vec2(0, 0));
    textures.textures.emplace_back(174, glm::vec2(16, 0));

    space.add(body);
    space.add(shape);

    return(entity);
}

void World::createTerrain(const cpVect& position, const cpVect& size)
{
    cpFloat top = position.y;
    cpFloat left = position.x;

    cpFloat bottom = top + size.y;
    cpFloat right = left + size.x;

    createEntity(WALL_ENTITY, cpv(left * 16, top * 16));
    createEntity(WALL_ENTITY, cpv(right * 16, top * 16));

    for(int y = top * 16; y < bottom * 16; y += 16)
    {
        createEntity(WALL_ENTITY, cpv(left * 16, y));
        createEntity(WALL_ENTITY, cpv(right * 16, y));
    }

    createEntity(WALL_ENTITY, cpv(left * 16, bottom * 16));
    createEntity(WALL_ENTITY, cpv(right * 16, bottom * 16));

    for(int x = (left + 1) * 16; x < right * 16; x += 16)
    {
        createEntity(WALL_ENTITY, cpv(x, bottom * 16));
    }
}

Entity World::createSimple(const cpVect& position, std::uint16_t texture)
{
    Entity entity = registry.create();

    Body& body = registry.assign<Body>(entity, position, CP_BODY_TYPE_STATIC);
    Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-8.0, -8.0), cpv(8.0, -8.0), cpv(8.0, 8.0), cpv(-8.0, 8.0)}), 0.1, 1.0, 1.0, 0.0);
    registry.assign<Texture>(entity, texture);

    space.add(body);
    space.add(shape);

    return(entity);

}

/*
Entity World::create(unsigned int definition, const cpVect& position)
{
    return(create(definitions[definition], position));
}

Entity World::create(const Definition& definition, const cpVect& position)
{
    Entity entity = registry.create();

    if(definition.base)
    {
        registry.assign<Base>(entity, definition.base->name);
    }

    if(definition.body)
    {
        Body& body = registry.assign<Body>(entity, position);
        space.add(body);

        if(definition.shape)
        {
            Shape& shape = registry.assign<Shape>(entity, body, definition.shape->vertices, definition.shape->bevel, definition.shape->density, definition.shape->friction, definition.shape->elasticity);
            space.add(shape);
        }

        if(definition.textures.size() == 1)
        {
            registry.assign<Texture>(entity, definition.textures[0].identifier);
        }
        else if(definition.textures.size() > 1)
        {
            Textures& tt = registry.assign<Textures>(entity);

            for(const Definition::Texture& bb: definition.textures)
            {
                tt.textures.emplace_back(bb.identifier);
            }
            
        }
    }


    return(entity);
}
*/

/*
void World::bajs()
{


    createTerrain(cpv(10, 40), cpv(40, 2));
    createTerrain(cpv(5, 50), cpv(20, 2));
    createTerrain(cpv(1.5, 1.5), cpv(100, 60));

    
    for(int i = 0; i < 9; i++) // Create slope.
    {
        Entity eee = createStone(cpv(40 + i * 16, 500 + i * 16));
        cpBodySetType(registry.get<Body>(eee).handle, CP_BODY_TYPE_STATIC);
    }
    
    for(int y = 0; y < 10; y++) // Create slide.
    {
        for(int x = 0; x < 10; x++)
        {
            createDirt(cpv(64 + x * 16 - y, 16 + y * 16 + x));
        }
    }
*/



/*
    createMagnet(cpv(300, 800));


    //Entity cEntity = createCylinder(cpv(100, 100));
    //Entity rEntity = createRod(cpv(200, 200));
    Entity bEntity = createBoom(cpv(0, 0));
    Entity aEntity = createBoom(cpv(100, 0));
    Entity testEntity = createBoom(cpv(1000, 16*59));
    Entity vEntity = createVehicle(cpv(512+200, 16*59+8));

    cpShapeFilter filter = cpShapeFilterNew(1, CP_ALL_CATEGORIES, CP_ALL_CATEGORIES);




    Body& vBody = registry.get<Body>(vEntity);
    cpShapeSetFilter(registry.get<Shape>(vEntity).handle, filter);
    cpVect vbAnchor = cpv(4*16, -16);


    
    Body& bBody = registry.get<Body>(bEntity);
    cpShapeSetFilter(registry.get<Shape>(bEntity).handle, filter);
    cpVect bvAnchor = cpv(80, 0);
    cpVect brAnchor = cpv(16, 0);
    cpBodySetAngle(bBody.handle, 0.5);
    cpBodySetPosition(bBody.handle, cpvsub(cpBodyLocalToWorld(vBody.handle, vbAnchor), cpBodyLocalToWorld(bBody.handle, bvAnchor)));

    bvGear = cpGearJointNew(vBody.handle, bBody.handle, 0.0, 1.0);
    cpConstraint* bvJoint = cpPivotJointNew2(vBody.handle, bBody.handle, vbAnchor, bvAnchor);
    cpSpaceAddConstraint(space.handle, bvGear);
    cpSpaceAddConstraint(space.handle, bvJoint);
    cpConstraintSetCollideBodies(bvGear, cpFalse);
    cpConstraintSetCollideBodies(bvJoint, cpFalse);

    cpConstraintSetErrorBias(bvGear, cpfpow(1.0f - 0.1, 60.0f));
    cpConstraintSetMaxBias(bvGear, 0.2);

    Body& aBody = registry.get<Body>(aEntity);
    cpShapeSetFilter(registry.get<Shape>(aEntity).handle, filter);
    cpBodySetPosition(aBody.handle, cpBodyLocalToWorld(bBody.handle, cpv(-80, 0)) - cpv(80, 0));

    abGear = cpGearJointNew(aBody.handle, vBody.handle, 0.0, 1.0);
    cpConstraint* abPivot = cpPivotJointNew2(aBody.handle, bBody.handle, cpv(80, 0), cpv(-80, 0));
    cpSpaceAddConstraint(space.handle, abGear);
    cpSpaceAddConstraint(space.handle, abPivot);
    cpConstraintSetCollideBodies(abGear, cpFalse);
    cpConstraintSetCollideBodies(abPivot, cpFalse);

*/



    /*
    Body& cBody = registry.get<Body>(cEntity);
    cpShapeSetFilter(registry.get<Shape>(cEntity).handle, filter);
    cpVect cvAnchor = cpv(-16, 0);
    cpVect crAnchor = cpv(8, 0);
    cpBodySetAngle(cBody.handle, 3.8);
    cpBodySetPosition(cBody.handle, cpBodyLocalToWorld(vBody.handle, vcAnchor));

    cpConstraint* cvJoint = cpPivotJointNew2(vBody.handle, cBody.handle, vcAnchor, cvAnchor);
    cpSpaceAddConstraint(space.handle, cvJoint);
    cpConstraintSetCollideBodies(cvJoint, cpFalse);

    Body& rBody = registry.get<Body>(rEntity);
    cpShapeSetFilter(registry.get<Shape>(rEntity).handle, filter);
    cpBodySetAngle(rBody.handle, 3.8);
    cpBodySetPosition(rBody.handle, cpBodyLocalToWorld(cBody.handle, crAnchor));
    cpVect rcAnchor = cpv(-8, 0);
    cpVect rbAnchor = cpv(16, 0);

    cpConstraint* rcGroove = cpGrooveJointNew(cBody.handle, rBody.handle, cpv(-32, 0), cpv(32, 0), cpvzero);
    cpConstraint* rcGear = cpGearJointNew(cBody.handle, rBody.handle, 0.0, 1.0);
    rcPin = cpPinJointNew(cBody.handle, rBody.handle, crAnchor, rcAnchor);
    cpConstraint* rbPivot = cpPivotJointNew2(rBody.handle, bBody.handle, rbAnchor, brAnchor);
    cpSpaceAddConstraint(space.handle, rcGroove);
    cpSpaceAddConstraint(space.handle, rcGear);
    cpSpaceAddConstraint(space.handle, rcPin);
    cpSpaceAddConstraint(space.handle, rbPivot);
    cpConstraintSetCollideBodies(rcGroove, cpFalse);
    cpConstraintSetCollideBodies(rcGear, cpFalse);
    cpConstraintSetCollideBodies(rcPin, cpFalse);
    cpConstraintSetCollideBodies(rbPivot, cpFalse);

    cpConstraintSetMaxForce(rcPin, 100000);
    cpPinJointSetDist(rcPin, 1);
    */


/*
    Entity vEntity = createVehicle(cpv(512+200, 16*59+8));
    Entity bEntity = createBoom(cpv(100, 100));

    cpShapeFilter filter = cpShapeFilterNew(1, CP_ALL_CATEGORIES, CP_ALL_CATEGORIES);
    cpShapeSetFilter(registry.get<Shape>(vEntity).handle, filter);
    cpShapeSetFilter(registry.get<Shape>(bEntity).handle, filter);

    Body& vBody = registry.get<Body>(vEntity);
    Child& vChild = registry.get<Children>(vEntity).children[0];

    Body& bBody = registry.get<Body>(bEntity);
    Child& bChild = registry.get<Children>(bEntity).children[0];
    cpBodySetAngle(bBody.handle, 3.8);




    cpBodySetPosition(bBody.handle, cpBodyLocalToWorld(vBody.handle, vChild.position) - cpvrotate(cpvforangle(bBody.getAngle()), bChild.position));
    cpSpaceReindexShapesForBody(space.handle, bBody.handle);
    
    

}


Entity World::createDirt(const cpVect& position)
{
    Entity entity = registry.create();

    Body& body = registry.assign<Body>(entity, position);
    Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-8.0, -8.0), cpv(8.0, -8.0), cpv(8.0, 8.0), cpv(-8.0, 8.0)}), 0.1, 3.125, 1.0, 0.0);
    Texture& texture = registry.assign<Texture>(entity, 127);

    space.add(body);
    space.add(shape);

    return(entity);
}

Entity World::createStone(const cpVect& position)
{
    Entity entity = registry.create();

    Body& body = registry.assign<Body>(entity, position);
    Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-8.0, -8.0), cpv(8.0, -8.0), cpv(8.0, 8.0), cpv(-8.0, 8.0)}), 0.0, 7.2, 1.0, 0.0);
    Texture& texture = registry.assign<Texture>(entity, 131);

    space.add(body);
    space.add(shape);

    return(entity);
}

Entity World::createVehicle(const cpVect& position)
{
    Entity entity = registry.create();

    Body& body = registry.assign<Body>(entity, position);
    Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-88.0, 24.0), cpv(-88.0, 8.0), cpv(-72.0, 8.0), cpv(-56.0, -8.0), cpv(-40.0, -8.0), cpv(-24.0, 8.0), cpv(8.0, 8.0), cpv(24.0, -8.0), cpv(40.0, -8.0), cpv(56.0, -24.0), cpv(88.0, -24.0), cpv(88.0, 24.0)}), 0.1, 5.7, 1.0, 0.0);
    Textures& textures = registry.assign<Textures>(entity);
    textures.textures.emplace_back(170, glm::vec2(80, -16));
    textures.textures.emplace_back(169, glm::vec2(64, -16));
    textures.textures.emplace_back(168, glm::vec2(48, -16));
    textures.textures.emplace_back(201, glm::vec2(-64, 0));
    textures.textures.emplace_back(202, glm::vec2(-48, 0));
    textures.textures.emplace_back(203, glm::vec2(-32, 0));
    textures.textures.emplace_back(206, glm::vec2(16, 0));
    textures.textures.emplace_back(207, glm::vec2(32, 0));
    textures.textures.emplace_back(208, glm::vec2(48, 0));
    textures.textures.emplace_back(209, glm::vec2(64, 0));
    textures.textures.emplace_back(210, glm::vec2(80, 0));
    textures.textures.emplace_back(240, glm::vec2(-80, 16));
    textures.textures.emplace_back(241, glm::vec2(-64, 16));
    textures.textures.emplace_back(242, glm::vec2(-48, 16));
    textures.textures.emplace_back(243, glm::vec2(-32, 16));
    textures.textures.emplace_back(244, glm::vec2(-16, 16));
    textures.textures.emplace_back(245, glm::vec2(0, 16));
    textures.textures.emplace_back(246, glm::vec2(16, 16));
    textures.textures.emplace_back(247, glm::vec2(32, 16));
    textures.textures.emplace_back(248, glm::vec2(48, 16));
    textures.textures.emplace_back(249, glm::vec2(64, 16));
    textures.textures.emplace_back(250, glm::vec2(80, 16));

    Children& children = registry.assign<Children>(entity);
    children.children.emplace_back(cpv(-48, 0));
    children.children.emplace_back(cpv(-48, 24));
    children.children.emplace_back(cpv(32, 0));
    children.children.emplace_back(cpv(32, 24));
    children.children.emplace_back(cpv(64, -16));

    space.add(body);
    space.add(shape);

    return(entity);
}

Entity World::createSimple(const cpVect& position, std::uint16_t texture)
{
    Entity entity = registry.create();

    Body& body = registry.assign<Body>(entity, position, CP_BODY_TYPE_STATIC);
    Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-8.0, -8.0), cpv(8.0, -8.0), cpv(8.0, 8.0), cpv(-8.0, 8.0)}), 0.1, 1.0, 1.0, 0.0);
    registry.assign<Texture>(entity, texture);

    space.add(body);
    space.add(shape);

    return(entity);

}

void World::createTerrain(const cpVect& position, const cpVect& size)
{
    cpFloat top = position.y;
    cpFloat left = position.x;

    cpFloat bottom = top + size.y;
    cpFloat right = left + size.x;

    createSimple(cpv(left * 16, top * 16), 11);
    createSimple(cpv(right * 16, top * 16), 11);

    for(int y = top * 16; y < bottom * 16; y += 16)
    {
        createSimple(cpv(left * 16, y), 51);
        createSimple(cpv(right * 16, y), 51);
    }

    createSimple(cpv(left * 16, bottom * 16), 88);
    createSimple(cpv(right * 16, bottom * 16), 90);

    for(int x = (left + 1) * 16; x < right * 16; x += 16)
    {
        createSimple(cpv(x, bottom * 16), 129);
    }
}

Entity World::createCylinder(const cpVect& position)
{
    Entity entity = registry.create();

    Body& body = registry.assign<Body>(entity, position);
    Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(24, 8), cpv(-24, 8), cpv(-24, -8), cpv(24, -8)}), 0.1, 1.1, 1.0, 0.0);
    Textures& textures = registry.assign<Textures>(entity);
    textures.textures.emplace_back(172, glm::vec2(-16, 0));
    textures.textures.emplace_back(173, glm::vec2(0, 0));
    textures.textures.emplace_back(174, glm::vec2(16, 0));

    space.add(body);
    space.add(shape);

    return(entity);
}

Entity World::createRod(const cpVect& position)
{
    Entity entity = registry.create();

    Body& body = registry.assign<Body>(entity, position);
    Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(24, 8), cpv(-24, 8), cpv(-24, -8), cpv(24, -8)}), 0.1, 1.1, 1.0, 0.0);
    Textures& textures = registry.assign<Textures>(entity);
    textures.textures.emplace_back(175, glm::vec2(-16, 0));
    textures.textures.emplace_back(176, glm::vec2(0, 0));
    textures.textures.emplace_back(177, glm::vec2(16, 0));

    space.add(body);
    space.add(shape);

    return(entity);
}

Entity World::createMagnet(const cpVect& position)
{
    Entity entity = registry.create();

    Body& body = registry.assign<Body>(entity, position);
    Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(24, 16), cpv(-24, 16), cpv(-8, 0), cpv(-8, -16), cpv(8, -16), cpv(8, 0)}), 0.1, 1.0, 1.0, 0.0);
    Textures& textures = registry.assign<Textures>(entity);
    textures.textures.emplace_back(373, glm::vec2(0, -8));
    textures.textures.emplace_back(412, glm::vec2(-16, 8));
    textures.textures.emplace_back(413, glm::vec2(0, 8));
    textures.textures.emplace_back(414, glm::vec2(16, 8));

    space.add(body);
    space.add(shape);

    return(entity);
}

Entity World::createBoom(const cpVect& position)
{
    Entity entity = registry.create();

    Body& body = registry.assign<Body>(entity, position, CP_BODY_TYPE_STATIC);
    Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(88, 8), cpv(-88, 8), cpv(-88, -8), cpv(88, -8)}), 0.1, 5.7, 1.0, 0.0);
    Textures& textures = registry.assign<Textures>(entity);
    textures.textures.emplace_back(240, glm::vec2(-80, 0));
    textures.textures.emplace_back(244, glm::vec2(-64, 0));
    textures.textures.emplace_back(244, glm::vec2(-48, 0));
    textures.textures.emplace_back(244, glm::vec2(-32, 0));
    textures.textures.emplace_back(244, glm::vec2(-16, 0));
    textures.textures.emplace_back(244, glm::vec2(0, 0));
    textures.textures.emplace_back(244, glm::vec2(16, 0));
    textures.textures.emplace_back(244, glm::vec2(32, 0));
    textures.textures.emplace_back(244, glm::vec2(48, 0));
    textures.textures.emplace_back(244, glm::vec2(64, 0));
    textures.textures.emplace_back(74, glm::vec2(80, 0));

    Children& children = registry.assign<Children>(entity);
    children.children.emplace_back(cpv(-80, 0));

    space.add(body);
    space.add(shape);

    return(entity);
}

#include <cmath>
void World::createArea(unsigned int amount)
{
    int a = std::sqrt(amount);
    for(int y = 0; y < a; y++)
    {
        for(int x = 0; x < a; x++)
        {
            createDirt(cpv(x * 16 + 100 + x, y * 16 + 100 + y));
        }
    }
}

*/



/*
Entity World::createCylinder(const cpVect& position)
{
    Entity entity = registry.create();

    Body& body = registry.assign<Body>(entity, position);
    Shape& shape = registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(24, 8), cpv(-24, 8), cpv(-24, -8), cpv(24, -8)}), 0.1, 1.0, 1.0, 0.0);
    Textures& textures = registry.assign<Textures>(entity);
    textures.textures.emplace_back(172, glm::vec2(-32, 0));
    textures.textures.emplace_back(173, glm::vec2(-16, 0));
    textures.textures.emplace_back(173, glm::vec2(0, 0));
    textures.textures.emplace_back(174, glm::vec2(16, 0));

    textures.textures.emplace_back(175, glm::vec2(-16, 0));
    textures.textures.emplace_back(176, glm::vec2(0, 0));
    textures.textures.emplace_back(176, glm::vec2(0, 0));
    textures.textures.emplace_back(177, glm::vec2(16, 0));

    space.add(body);
    space.add(shape);

    return(entity);
}



*/
/*



Solid boom
Cylinder boom
Rod boom



Vehicle decides "properties" of attachments. For example force and spring for tires and force for boom. Maybe definied in Anchor~ component?



Tire needs Pivot and Gear.
Tracks needs Pin and Gear(fixed).



Rails on roof will be fixed. NO, RIGHT!!! Rails on roof are actually LOCKED in to the "terrain".
Rails on vehicle will be rotateable or fixed.
Rails will have the platå on itself at all time.

So Rails can be Locked to terrain and Attached to vehicles. Same with Sensor, Radio and Computer for example.
When Locked to Terrain, its body becomes Static.
When Attach to Vehicle, its body becomes Constraint.
But there are different Constraints, Tires has Pivot and Gear, Radio will have Weld.


Vehicle should have some kind of Anchor points, but some Entities are attached on Vehicle surfaces, like Radio.
Maybe you're able to place stuff on the Vehicle, those stuff should be a part of it instead. Not changeable!!!!!!!!
Or NPC can install it for you. OR!!! Have anchor points for those too, but on the surface.
Yes, I think that is good. Then parts can fall of when damaged.

Maybe have a bitset for both anchors that are about to be joined.
Add all constraints to the middle entity that AND both bitsets.
For example a Claw bucket has bitset Pivot, Gear.
For example a Piston rod has bitset Pivot, Gear.
For example a Rope has bitset Pivot.
Claw and Piston will have Pivot and Gear.
Claw and Rope will have Pivot.

Tire has Gear, Groove, Spring. What about Pivot instead of Gear sometimes?
Fork has 
Tracks has two anchor points! 



Lock    - Snaps in to grid and body becomes static.
Place
Merge   - Becomes a part of another entity, sharing the same Body.
Attach  - Snaps in to anchor and is constrained.








What if a Piston is REALLY 1 entity? The joint entity is the entity.



It will have Pin and Gear between Vehicle and Bucket.


registry.assign<Pin>(piston, vehicle, bucket);
registry.assign<Gear>(piston, vehicle, bucket);









*/










Entity World::getEntityOfShapeAtPosition(const cpVect& position)
{
    return(getEntityOfShapeAtPosition(position, CP_SHAPE_FILTER_ALL));
}

Entity World::getEntityOfShapeAtPosition(const cpVect& position, cpShapeFilter filter)
{
    return(getEntityOfShapeNearPosition(position, 0.0, filter));
}

Entity World::getEntityOfShapeNearPosition(const cpVect& position, cpFloat distance, cpShapeFilter filter)
{
    cpShape* shapeHandle = cpSpacePointQueryNearest(space.handle, position, distance, filter, NULL);

    return(shapeHandle != NULL ? static_cast<Entity>(reinterpret_cast<std::uintptr_t>(cpShapeGetUserData(shapeHandle))) : entt::null);
}

/*

void World::attachEntity(Entity entity)
{

}

void World::detachEntity(Entity entity)
{

}








Entity World::getEntityOfShapeNearestPosition(const cpVect& position, cpFloat maximumDistance, cpFilter filter)
{
    cpShape* shapeHandle = cpSpacePointQueryNearest(space.handle, position, maximumDistance, filter, NULL);

    return(shapeHandle != NULL ? static_cast<Entity>(reinterpret_cast<std::uintptr_t>(cpShapeGetUserData(shapeHandle))) : entt::null);
}

Entity World::getEntityOfShapeNearestPosition(const cpVect& position)
{
    cpShape* shapeHandle = cpSpacePointQueryNearest()
}




std::vector<Entity> World::getAnchorEntities


    std::experimental::optional<Shape> Space::pointQueryNearest(cpVect pointPosition, cpFloat maxDistance, cpShapeFilter shapeFilter, cpPointQueryInfo* pointQueryInfo)
    {
        cpShape* shapeHandle = cpSpacePointQueryNearest(handle, pointPosition, maxDistance, shapeFilter, pointQueryInfo);

        if(shapeHandle != NULL)
        {
            return(std::experimental::optional<Shape>(shapeHandle));
        }

        return(std::experimental::optional<Shape>());
    }



    NOTE: Does only work if entity has a 'Shape'.
    TODO: Check if 'Shape' exists in 'space'?

Entity World::getEntityNearestPosition(cpVect position, cpFloat maximumDistance, cpFilter filter)
{
    cpShape* shapeHandle = cpSpacePointQueryNearest(space.handle, position, maximumDistance, filter, NULL);

    return(shapeHandle != NULL ? static_cast<Entity>(reinterpret_cast<std::uintptr_t>(cpShapeGetUserData(shapeHandle))) : entt::null);
}



    NOTE: Does only work if entity has a 'Shape'.

Entity World::getEntityNearestEntity(Entity entity, cpFloat maximumDistance, cpFilter filter)
{

}


*/






/*

    std::vector<cpVect> vertices({cpv(-8.0, -8.0), cpv(8.0, -8.0), cpv(8.0, 8.0), cpv(-8.0, 8.0)});

    for(int y = 32; y < 1000; y += 16)
    {
        Entity wall = registry.create();
        Body& wallBody = registry.assign<Body>(wall, cpv(32, y));
        cpBodySetType(wallBody.handle, CP_BODY_TYPE_STATIC);
        Shape& wallShape = registry.assign<Shape>(wall, wallBody, 1.0, 1.0, 0.0, vertices);
        space.add(wallBody);
        space.add(wallShape);
        Texture& wallTexture = registry.assign<Texture>(wall, 131);
    }

    for(int y = 32; y < 1000; y += 16)
    {
        Entity wall = registry.create();
        Body& wallBody = registry.assign<Body>(wall, cpv(1800, y));
        cpBodySetType(wallBody.handle, CP_BODY_TYPE_STATIC);
        Shape& wallShape = registry.assign<Shape>(wall, wallBody, 1.0, 1.0, 0.0, vertices);
        space.add(wallBody);
        space.add(wallShape);
        Texture& wallTexture = registry.assign<Texture>(wall, 131);
    }

    for(int x = 32; x < 1800; x += 16)
    {
        Entity wall = registry.create();
        Body& wallBody = registry.assign<Body>(wall, cpv(x, 1000));
        cpBodySetType(wallBody.handle, CP_BODY_TYPE_STATIC);
        Shape& wallShape = registry.assign<Shape>(wall, wallBody, 1.0, 1.0, 0.0, vertices);
        space.add(wallBody);
        space.add(wallShape);
        Texture& wallTexture = registry.assign<Texture>(wall, 131);
    }
*/





/*
void World::stones(const cpVect& position)
{
    std::vector<cpVect> vertices({cpv(-8.0, -8.0), cpv(8.0, -8.0), cpv(8.0, 8.0), cpv(-8.0, 8.0)});
    for(int y = 0; y < 10; y++)
    {
        for(int x = 0; x < 10; x++)
        {
            Entity wall = registry.create();
            Body& wallBody = registry.assign<Body>(wall, cpvadd(cpv((x*16)+2, y*16), position));
            Shape& wallShape = registry.assign<Shape>(wall, wallBody, 1.0, 1.0, 0.0, vertices);
            space.add(wallBody);
            space.add(wallShape);
            Texture& wallTexture = registry.assign<Texture>(wall, 131);
        }
    }
}*/


// Have entt signal when child is created, which auto adds filter?
// Remember that there are more than 2 "types" of entities, not only single/orphan (body, shape, text) and family (body)(shape text).
// Think of tires, the parent has Body AND Shape! The children has only textures.
// Sometimes when attaching entities, for example a tire to a vehicle, they should have same group filter.





// createBaseEntity()
// createExtendedEntity(Entity baseEntity = entt::null)  // If null, create new entity as child, otherwise append components to provided entity (parent).












//Entity Game::createSubEntity(Entity parent, glm::vec2 offset, std::uint16_t texture)
//{
    /*Entity entity = world->registry.create();
    Shape& shape = world->registry.assign<Shape>(entity, world->registry.get<Body>(parentIdentifier), materials.at("Wood"), cpv(offset.x, offset.y));
    shape.setUserData(reinterpret_cast<void*>(static_cast<std::uintptr_t>(entity)));
    world->space.add(shape);
    world->registry.assign<Texture>(entity, offset, texture);

    Parent& parent = world->registry.assign<Parent>(entity, parentIdentifier);

    shape.setFilterGroup(parentIdentifier);

    return(entity);*/
//}

//Entity Game::createFoundation(cpVect position)
//{
    /*Entity entity = world->registry.create();
    world->registry.assign<Base>(entity, "Foundation");
    Body& body = world->registry.assign<Body>(entity, position);
    world->space.add(body); // TEMP

    Children& children = world->registry.assign<Children>(entity);
    Entity e1 = createSubEntity(entity, glm::vec2(0.0f, -8.0f), 373);
    world->registry.assign<Anchor>(e1);
    world->registry.get<Shape>(e1).setFilterCategories(ANCHOR);
    world->registry.get<Shape>(e1).setFilterMask(~ANCHOR);
    children.entities.push_back(e1);
    Entity e2 = createSubEntity(entity, glm::vec2(-16.0f, 8.0f), 412);
    children.entities.push_back(e2);
    Entity e3 = createSubEntity(entity, glm::vec2(0.0f, 8.0f), 413);
    children.entities.push_back(e3);
    Entity e4 = createSubEntity(entity, glm::vec2(16.0f, 8.0f), 414);
    children.entities.push_back(e4);

    body.setType(CP_BODY_TYPE_STATIC);

    return(entity);*/
//}

//Entity Game::createPistonCylinder(cpVect position)
//{
    /*Entity entity = world->registry.create();
    world->registry.assign<Base>(entity, "Piston cylinder");
    Body& body = world->registry.assign<Body>(entity, position);
    world->space.add(body); // TEMP

    Children& children = world->registry.assign<Children>(entity);
    Entity e1 = createSubEntity(entity, glm::vec2(-16.0f, 0.0f), 172);
    world->registry.assign<Anchor>(e1);
    world->registry.assign<Pivot>(e1, body, cpv(-16, 0));
    world->registry.assign<Gear>(e1, body);
    world->registry.get<Shape>(e1).setFilterCategories(ANCHOR);
    world->registry.get<Shape>(e1).setFilterMask(~ANCHOR);
    children.entities.push_back(e1);
    Entity e2 = createSubEntity(entity, glm::vec2(0.0f, 0.0f), 173);
    children.entities.push_back(e2);
    Entity e3 = createSubEntity(entity, glm::vec2(16.0f, 0.0f), 174);
    children.entities.push_back(e3);

    //body.setType(CP_BODY_TYPE_STATIC);

    return(entity);*/
//}



























/*



Entity World::getParentEntity(Entity entity)
{
    return(registry.get<Parent>.entity);
}


std::vector<Entity> World::getChildrenEntity(Entity entity)
{

}



std::vector<Entity> World::getFamily(Entity entity)
{

}

std::vector<Entity> World::getAnchorEntities






    std::experimental::optional<Shape> Space::pointQueryNearest(cpVect pointPosition, cpFloat maxDistance, cpShapeFilter shapeFilter, cpPointQueryInfo* pointQueryInfo)
    {
        cpShape* shapeHandle = cpSpacePointQueryNearest(handle, pointPosition, maxDistance, shapeFilter, pointQueryInfo);

        if(shapeHandle != NULL)
        {
            return(std::experimental::optional<Shape>(shapeHandle));
        }

        return(std::experimental::optional<Shape>());
    }



    NOTE: Does only work if entity has a 'Shape'.
    TODO: Check if 'Shape' exists in 'space'?

Entity World::getEntityNearestPosition(cpVect position, cpFloat maximumDistance, cpFilter filter)
{
    cpShape* shapeHandle = cpSpacePointQueryNearest(space.handle, position, maximumDistance, filter, NULL);

    return(shapeHandle != NULL ? static_cast<Entity>(reinterpret_cast<std::uintptr_t>(cpShapeGetUserData(shapeHandle))) : entt::null);
}



    NOTE: Does only work if entity has a 'Shape'.

Entity World::getEntityNearestEntity(Entity entity, cpFloat maximumDistance, cpFilter filter)
{

}

*/







/*
#include "world.hpp"                //
#include <experimental/optional>    // std::experimental::optional<>



World::World(): space(), registry()
{
    
    space.setGravity(cpv(0.0, 9.8));



}

World::~World()
{

}

bool World::hasParent(Entity entity)
{
    return(registry.has<Parent>(entity));
}

Entity World::getParentEntity(Entity entity)
{
    return(registry.get<Parent>.entity);
}

bool World::hasChildren()

std::vector<Entity> World::getChildrenEntity(Entity entity)
{

}



std::vector<Entity> World::getFamily(Entity entity)
{

}

std::vector<Entity> World::getAnchorEntities






    std::experimental::optional<Shape> Space::pointQueryNearest(cpVect pointPosition, cpFloat maxDistance, cpShapeFilter shapeFilter, cpPointQueryInfo* pointQueryInfo)
    {
        cpShape* shapeHandle = cpSpacePointQueryNearest(handle, pointPosition, maxDistance, shapeFilter, pointQueryInfo);

        if(shapeHandle != NULL)
        {
            return(std::experimental::optional<Shape>(shapeHandle));
        }

        return(std::experimental::optional<Shape>());
    }



    NOTE: Does only work if entity has a 'Shape'.
    TODO: Check if 'Shape' exists in 'space'?

Entity World::getEntityNearestPosition(cpVect position, cpFloat maximumDistance, cpFilter filter)
{
    cpShape* shapeHandle = cpSpacePointQueryNearest(space.handle, position, maximumDistance, filter, NULL);

    return(shapeHandle != NULL ? static_cast<Entity>(reinterpret_cast<std::uintptr_t>(cpShapeGetUserData(shapeHandle))) : entt::null);
}



    NOTE: Does only work if entity has a 'Shape'.

Entity World::getEntityNearestEntity(Entity entity, cpFloat maximumDistance, cpFilter filter)
{

}
*/





//std::vector<Entity> World::getEntitiesOverlappingEntity() {}
//Entity World::getEntityOverlappingTheMostEntity() {} // I guess this is done with getEntityNearestEntity tho, with maximumDistance?






/*

bind
unbind
bound
unbound





http://gameprogrammingpatterns.com/spatial-partition.html
https://github.com/slembcke/Chipmunk2D/blob/dbc983e3f3f2f7269aca8d21ba64085c00130172/src/cpSpaceHash.c#L242

https://www.gamedev.net/articles/programming/general-and-gameplay-programming/swept-aabb-collision-detection-and-response-r3084/
https://gamedevelopment.tutsplus.com/tutorials/collision-detection-using-the-separating-axis-theorem--gamedev-169
https://gist.github.com/nyorain/dc5af42c6e83f7ac6d831a2cfd5fbece
https://github.com/SFML/SFML/blob/master/include/SFML/Graphics/Transform.hpp


    CPU cache
        L1
        L2
        L3

        Cache line is 64 bytes, commonly. That is 16 32-bit values or 8 64-bit values.
        When requesting a single byte, still a whole cache line is read from RAM.
        If I need even more than that cache line, a cache miss occurs and it fetches a new cache line.
        Actually no, its so clever that if you traverse forward, it will actually fetch catch line n+1 also.
        I think the fetched cache line also includes some bytes behind too.
        You dont have to read every single byte in order, as long as its a predictable repetitive and a not-to-big stride. 


Maybe let static entities have Movement too,
because then you can merge positionIntegration and spatialPopulating in the same view loop.







mass = density * area;
force = mass * acceleration;
acceleration = force / mass; // Acceleration is the rate of change in velocity over time.
momentum = mass * velocity; // Momentum is a measurement of mass in motion, how much mass is in how much motion.
impulse = force * time; // Impulse is the rate of change in momentum over time???


    Descriptions

        Momentum is a measurement of mass in motion, how much mass is in how much motion.
        Acceleration is the rate of change in velocity over time.
        Velocity is the rate of change in position over time.
        Impulse is the rate of change in momentum over time.





    Integration methods

        Explicit euler integration (a first order integrator).
            Good:   Suited for simulating stiff equations that becomes unstable with other methods.
            Bad:    Requires solving a system of equations each step.
            Psuedo:
                    position += velocity * delta;
                    velocity += force * inverseMass * delta;

        Symplectic euler or Semi-implicit euler integration (a second order integrator).
            Good:   ?
            Bad:    ?
            Psuedo:
                    velocity += force * inverseMass * delta;
                    position += velocity * delta;

        Verlet integration (a second order integrator).
            Good:   Provides greater accuracy than implicit euler and less memory usage when simulating large number of particles.
            Bad:    ?
            Psuedo:
                    ?

        Runge Kutta order 4 or RK4 (a fourth order integrator).
            Good:   Very accurate.
            Bad:    ?
            Psuedo:
                    ?




// Quote from https://gamedev.stackexchange.com/questions/15708/how-can-i-implement-gravity
// "If you have multiple interacting objects, you should update all their positions before recalculating the forces and updating the velocities."



https://channel9.msdn.com/Events/Build/2014/2-661
https://www.youtube.com/watch?v=WDIkqP4JbkE
https://github.com/jrfonseca/gprof2dot
https://stackoverflow.com/questions/375913/how-can-i-profile-c-code-running-on-linux/10945113#10945113
https://stackoverflow.com/questions/375913/how-can-i-profile-c-code-running-on-linux
https://github.com/dbartolini/data-oriented-design
http://www.reedbeta.com/blog/data-oriented-hash-table/
https://stackoverflow.com/questions/27195333/is-something-wrong-with-my-hash-function
https://gist.github.com/kirbysayshi/1760774
https://www.gamedev.net/articles/programming/general-and-gameplay-programming/spatial-hashing-r2697
https://gamedev.stackexchange.com/questions/87939/how-to-avoid-checking-the-same-triangle-twice-in-collision-detection-with-spatia
https://gamedev.stackexchange.com/questions/124186/when-to-use-spatial-hashing-vs-bounding-volume-hierarchy


Two ways of using spatial hash:
    Add an entity to overlapping (multiple) cells.
    Add an entity to closest cell (position/cellSize), when querying, check neighbour cells of this cell.
    Add an entity to closest cell and its neighbours.

To go from entity position to grid position, it could be as easy as converting float to int. For example position 1.345f is grid 1.
Store a spatial hash on static entities, no need to recreate the list for them every tick.
Looks like CP is doing these individually. Maybe its possible to iterate these with different entt::views? maybe rawView?

void
cpBodyUpdatePosition(cpBody *body, cpFloat dt)
{
    cpVect p = body->p = cpvadd(body->p, cpvmult(cpvadd(body->v, body->v_bias), dt));
    cpFloat a = SetAngle(body, body->a + (body->w + body->w_bias)*dt);
    SetTransform(body, p, a);
    
    body->v_bias = cpvzero;
    body->w_bias = 0.0f;
    
    cpAssertSaneBody(body);
}
placement.position += movement.linearVelocity * deltaTime;
placement.rotation += movement.angularVelocity * deltaTime;



void
cpBodyUpdateVelocity(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt)
{
    // Skip kinematic bodies.
    if(cpBodyGetType(body) == CP_BODY_TYPE_KINEMATIC) return;
    
    cpAssertSoft(body->m > 0.0f && body->i > 0.0f, "Body's mass and moment must be positive to simulate. (Mass: %f Moment: %f)", body->m, body->i);
    
    body->v = cpvadd(cpvmult(body->v, damping), cpvmult(cpvadd(gravity, cpvmult(body->f, body->m_inv)), dt));
    body->w = body->w*damping + body->t*body->i_inv*dt;
    
    // Reset forces.
    body->f = cpvzero;
    body->t = 0.0f;
    
    cpAssertSaneBody(body);
}
Vector acceleration = movement.force * movement.inverseMass;
acceleration += gravity;
movement.linearVelocity = acceleration * deltaTime;






Chipmunk stepping process:

Integrate positions.
Find colliding pairs.
Rebuild contact graph.
Pre-step arbiters and constraints.
Integrate velocities.
Apply cached impulses.
Run impulse solver.
Post-solve constraint callbacks.
Post-solve arbiter callbacks.




















    UI needs first priority on input, for example you can open Inventory when driving a vehicle or any time, no matter what.
    Each root entity should have a context component.
    When processing, first check if entity has parent, if so, use parent's context.




    If root entity has the context, translates raw input to actions, those actions wont interfere with a creature driving a vehicle for example.
    For example the action will become ROTATE_BOOM, system will walk down childrens, eventually it will start walking the creature too,
    but it wont catch ROTATE_BOOM.














Another option would be, your input system simply updates all the Input Components.  So, your player entity could have an input component, your camrea entity could have an input component, and you could have the main game/menu system have an input component.
The menu system would look for ESCAPE_KEY (probablymapped to Esc) in the Input Component, the Camera would look for CAMERA_MOVE_FORWARD_KEY (possibly mapped to 'w') from the Input Component, and the player would look for WEAPON_FIRE_KEY, PLAYER_MOVE_FORWARD_KEY, etc. from the input component.


    Check if entity has a parent first, if so, apply on that parent instead and go down the tree (parent to child). But what if it ends up in an endless loop?

    If i set the vehicle as PlayerControll component, how do i then exit the vehicle when Enter key is pressed?








lets say a creature has a PlayerController component, also holding a weapon (child) and driving a vehicle (parent). an input event is obtained, its the key "D". since "D" will reacts on both a creature (walk right) and a vehicle (drive right), i cant translate the key to an Action yet.
so it looks at the creature's InputContext component first, does it want the key "D"? yes! thats bad, since it shouldnt walk when sitting in a vehicle. so it has to check the parent's InputContext first?
still, it doesnt seem right, it feels like i need to translate the key "D" to an Action (to DRIVE in this case) before setting the states.







when processing input events, how do I know which entity to set states to? giving the player entity a tag/flag component? 



about inputs in an ECS, is it wise to give entities a context component? for example a creature has WALK and JUMP, weapon has FIRE.


In updating, going through all entities with Input component, seems crazy.




when processing, poll events, get entity with Control tag/flag component (the player) set states on entities. For example set WALK state to true if key W is pressed.
When updating, check states of all entities and apply actions. For example change position if WALK state is true.

But when processing, how do I know which entity to set a state? For example a creature entity (parent) holding a weapon entity (child).
I was thinking of giving the creature a Control component/tag (only one entity can have this tag at the same time), then walk down the nodes (from parent to child) until it finds an entity which reacts on FIRE.
But I know its the wrong direction to walk, people usually says to go from child to parent.

Its easier in a scenario where the creature is driving a vehicle, then you can walk from child to parent, since creature will be a child of vehicle.



It gets even more complicated when you have different contexts, for example a creature entity can either hold a weapon entity or a tool entity.




*/

