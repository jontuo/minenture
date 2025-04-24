#ifndef __WORLD_HPP_INCLUDED__
#define __WORLD_HPP_INCLUDED__

#include "entt/entity/registry.hpp"     // entt::Registry(),
#include "space.hpp"                    // Space()
#include <string>
#include "type.hpp"
#include "definition.hpp"               // Definition
#include <vector>                       // std::vector<Definition>
#include "entity.hpp" // enum EntityType



class World
{
public:

    entt::Registry<Entity> registry; // Should be private?
    Space space;

    World(const std::vector<Definition>& definitions);
    ~World();

    void update(double timeStep);

    void test();





    Entity createEntity(const Definition& definition, const cpVect& position);

    Entity createEntity(EntityType type, const cpVect& position);

    Entity createSimple(const cpVect& position, std::uint16_t texture);
    void createTerrain(const cpVect& position, const cpVect& size);
    Entity createBelt(const cpVect& position);
    Entity createVehicle(const cpVect& position);
    Entity createCylinder(const cpVect& position);

    Entity create(unsigned int definition, const cpVect& position);
    Entity create(const Definition& definition, const cpVect& position);




/*
    Entity createDirt(const cpVect& position);
    Entity createStone(const cpVect& position);
    Entity createMagnet(const cpVect& position);


    Entity createVehicle(const cpVect& position);
    Entity createCylinder(const cpVect& position);
    Entity createRod(const cpVect& position);
    Entity createBoom(const cpVect& position);
    void createArea(unsigned int amount);

    cpConstraint* bvGear;
    cpConstraint* abGear;


    Entity createSimple(const cpVect& position, std::uint16_t texture);
    void createTerrain(const cpVect& position, const cpVect& size);



    void bajs();*/
    //void stones(const cpVect& position);


    Entity getEntityOfShapeAtPosition(const cpVect& position);
    Entity getEntityOfShapeAtPosition(const cpVect& position, cpShapeFilter filter);
    Entity getEntityOfShapeNearPosition(const cpVect& position, cpFloat distance, cpShapeFilter filter);



private:

    const std::vector<Definition>& definitions;

    std::string name;
    





};

#endif








/*
#ifndef __WORLD_HPP_INCLUDED__
#define __WORLD_HPP_INCLUDED__

#include "entt/entity/registry.hpp"
#include <string>
#include "type.hpp"
#include "chipmunkpp/space.hpp"



class World
{
public:

    cp::Space space;
    entt::Registry<Entity> registry; // Should be private?
    


    World();
    ~World();



    Entity getParent(Entity entity);
    std::vector<Entity> getChildren(Entity entity);
    std::vector<Entity> getFamily(Entity entity);

    Entity getEntityNearestPosition(cpVect position, cpFloat maximumDistance, cpFilter filter);
    Entity getEntityNearestEntity(Entity entity, cpFloat maximumDistance, cpFilter filter);


private:

    std::string name;
    





};

#endif






*/


















/*
#include <string>
#include <experimental/optional>

#include "entt/entity/registry.hpp"

#include "chipmunkpp/space.hpp"
#include "chipmunkpp/body.hpp"
#include "chipmunkpp/pivot.hpp"
#include "chipmunkpp/gear.hpp"

#include "type.hpp"






class World
{
public:

    std::string name;

    cp::Body mouseBody;
    cp::Pivot mousePivot;
    cp::Gear mouseGear;



    World();
    World(int a);
    ~World();


    World(const World& world);
    World& operator=(World &other);
    World& operator=(World&& other);
    World(World&& world);

    void update(double d);

    std::experimental::optional<cp::Shape> getNearestShape(float positionX, float positionY, float maxDistance);
    std::experimental::optional<Entity> getNearestEntity(float positionX, float positionY, float maxDistance);


    void fill(const cpVect& position, const cpVect& size, cpBodyType type, unsigned int texture);
    Entity createBase(cpVect position);
    Entity createCylinder(cpVect position);
    Entity createRod(cpVect position);
    Entity createPiston(cpVect position);
    
    void test();
    void testt();


    entt::Registry<Entity> registry;
    
    cp::Space space;

private:




    




};

#endif
*/