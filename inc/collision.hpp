
#ifndef __COLLISION_HPP_INCLUDED__
#define __COLLISION_HPP_INCLUDED__


/*
#include <cmath>
#include "vector.hpp"




struct Collision
{
    
};






bool axisAlignedBoundingBox()
{

}

bool separateAxisTheorem()
{
    
}


bool isCircleCollidingWithCircle(const Vector& positionA, const Vector& positionB)
{
    return(std::pow(positionB.x - positionA.x, 2) + std::pow(positionA.y - positionB.y, 2) <= 512.0f);
}


    // If you're separating the views (one for Placement and one for Movement, like CP does it).
    // Sort Placement by Parent::depth and sort Movement by Placement.

    if(deltaTime == 0.0f)
    {
        // FIX: Do not update if deltaTime is 0!
    }

    

    // Integrate position.
    auto aView = registry.view<Placement, Movement>();
    for(Entity entity: aView)
    {
        Placement& placement = aView.get<Placement>(entity);
        Movement& movement = aView.get<Movement>(entity);

        placement.position += movement.linearVelocity * deltaTime;
        placement.angle += movement.angularVelocity * deltaTime;
    }

    // Clear collision cells.
    cells.fill({});

    // Populate collision cells.
    auto eView = registry.view<Placement, Blockment>();
    for(Entity entity: eView)
    {
        Placement& placement = eView.get<Placement>(entity);

        cells[static_cast<unsigned int>(std::floor(placement.position.y / SPATIAL_CELL_HEIGHT))][static_cast<unsigned int>(std::floor(placement.position.x / SPATIAL_CELL_WIDTH))].emplace_back(entity);
    }

    // How do know if two entities has already been checked in a neighbour cell, since I add them to all neighbour cells?
    // How does this fixed grid work when moving the camera?
    // Will a hash map fix these?
    // Will a larger cell size fix these? Try size 64 too.
    // PERF: Change 'unsigned int' to 'size_t'?
    // PERF: Check if neighbour cells exists before. For example store neighbour cell count in an array.
    // PERF: Maybe add a temporary local variable of std::vector<Entity> when checking size() and accessing elements.
    // PERF: Replace std::vector<Entity> with std::array<Entity, ?>. 
    // Maybe separate polygons and circles in different spatial hash grid cells?
    
    // Clear collision candidates.
    candidates.clear();

    // Find collision candidates.
    for(unsigned int cellY = 0; cellY < 60; cellY++)
    {
        for(unsigned int cellX = 0; cellX < 60; cellX++)
        {
            unsigned int currentCellSize = cells[cellY][cellX].size();

            if(currentCellSize > 0)
            {
                for(unsigned int currentEntityIndex = 0; currentEntityIndex < currentCellSize; currentEntityIndex++)
                {
                    std::array<std::array<int, 3>, 5> neighbourCells = {{{{1, 0, 0}}, {{0, 1, 0}}, {{0, -1, 1}}, {{0, 0, 1}}, {{0, 1, 1}}}};

                    for(int neighbourCellIndex = 0; neighbourCellIndex < 5; neighbourCellIndex++)
                    {
                        int neighbourEntityFirstIndex = neighbourCells[neighbourCellIndex][0];
                        int neighbourCellX = cellX + neighbourCells[neighbourCellIndex][1];
                        int neighbourCellY = cellY + neighbourCells[neighbourCellIndex][2];
                        
                        for(unsigned int neighbourEntityIndex = neighbourEntityFirstIndex; neighbourEntityIndex < cells[neighbourCellY][neighbourCellX].size(); neighbourEntityIndex++)
                        {
                            candidates.emplace_back(cells[cellY][cellX][currentEntityIndex], cells[neighbourCellY][neighbourCellX][neighbourEntityIndex])
                        }
                    }
                }
            }
        }
    }

    // Clear collision pairs.
    collisions.clear();

    // Find collision pairs.
    for(unsigned int candidateIndex = 0; candidateIndex < candidates.size(); candidateIndex++)
    {
        Entity aEntity = candidates[candidateIndex].first;
        Entity bEntity = candidates[candidateIndex].second;
        Placement& aPlacement = registry.get<Placement>(aEntity);
        Placement& bPlacement = registry.get<Placement>(bEntity);
        Blockment& aBlockment = registry.get<Blockment>(aEntity);
        Blockment& bBlockment = registry.get<Blockment>(bEntity);

        for(int i = 0; i < aBlockment.vertices.size(); i++)
        {

        }

        
    }



// loop over the vertices
for (int i = 0; i < shape.vertices.length; i++) {
  // get the current vertex
  Vector p1 = shape.vertices[i];
  // get the next vertex
  Vector p2 = shape.vertices[i + 1 == shape.vertices.length ? 0 : i + 1];
  // subtract the two to get the edge vector
  Vector edge = p1.subtract(p2);
  // get either perpendicular vector
  Vector normal = edge.perp();
  // the perp method is just (x, y) => (-y, x) or (y, -x)
  axes[i] = normal;
}







    // Integrate velocity.
    auto velocityIntegrationView = registry.view<Movement>();
    for(Entity entity: velocityIntegrationView)
    {
        Movement& movement = velocityIntegrationView.get(entity);

        movement.linearVelocity += (movement.force * movement.inverseMass + gravity) * deltaTime;
        movement.force.reset();
        movement.torque.reset();
    }



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



Collision

    Detection
        Axis aligned bounding box (AABB)
        Separate axis theorem (SAT)
    
    Response
        Projection based methods modifies the positions directly.
        Impulse based methods modifies the first derivative of position (ie velocities).
        Penalty based methods modifies the second derivative of positions (ie accelerations)









Remove Velocity to make an entity fall asleep?
Have a sleeping group, to wake up all entities as a group.
They will fall asleep fast again, since its based on how low their velocity currently is.










https://gamedev.stackexchange.com/questions/72895/separating-axis-theorem-rotation-issue
https://gamedev.stackexchange.com/questions/148360/separating-axis-theorem-where-to-go-from-the-normals
https://gamedev.stackexchange.com/questions/103890/how-to-calculate-faces-involved-in-collision-separating-axis-theorem
https://stackoverflow.com/questions/30891502/separating-axis-theorem-rotation-around-center-of-mass
https://gamedev.stackexchange.com/questions/27596/implementing-separating-axis-theorem-sat-and-minimum-translation-vector-mtv
http://www.dyn4j.org/2010/01/sat/
http://www.metanetsoftware.com/technique/tutorialA.html
*/



#endif
