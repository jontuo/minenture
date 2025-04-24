
#include "game.hpp"

#include "shader.hpp"
#include "glpp/shader_stage.hpp"
#include "image.hpp"

#include <glm/ext.hpp>
#include <iostream>

#include "base.hpp"
#include "texture.hpp"
#include "component.hpp"


#include <cmath>
#include "vector.hpp"               // toGlm() and toCp().

Game::Game():
    debug(true),
    debugWindow(true),
    collisionVisualization(true),
    collisionVisualizationHalfWidth(0.5f),
    collisionVisualizationColor(1.0f, 1.0f, 1.0f, 1.0f),

    entityVertexBuffer(GL_ARRAY_BUFFER, GL_STREAM_DRAW),
    entityVertexArray(),
    entityTextureArray(),
    entityShaderProgram(),

    shapeVertexBuffer(GL_ARRAY_BUFFER, GL_STREAM_DRAW),
    shapeVertexArray(),
    shapeShaderProgram(),







    holdingEntity(entt::null)
    //mouseBody(),
    //mousePivot(mouseBody, cpvzero),
    //mouseGear(mouseBody)


{

    projectionMatrix = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);
    //projectionMatrix = projectionMatrix;
    //viewMatrix = glm::translate(viewMatrix, glm::vec3(110.0f, 110.0f, 0.0f));
    //viewMatrix = glm::scale(viewMatrix, glm::vec3(2.0f, 2.0f, 1.0f));



    entityVertices.reserve(10000);

    entityVertexBuffer.bind();
    entityVertexBuffer.setData(nullptr, 0);
    entityVertexBuffer.unbind();

    entityVertexArray.bind();
    entityVertexBuffer.bind();
    entityVertexArray.enableAttribute(0);
    entityVertexArray.setFloatingAttribute(0, 2, GL_FLOAT, false, (8 * sizeof(float)) + sizeof(unsigned int), (void*)(0));
    entityVertexArray.enableAttribute(1);
    entityVertexArray.setFloatingAttribute(1, 2, GL_FLOAT, false, (8 * sizeof(float)) + sizeof(unsigned int), (void*)(2 * sizeof(float)));
    entityVertexArray.enableAttribute(2);
    entityVertexArray.setFloatingAttribute(2, 4, GL_FLOAT, false, (8 * sizeof(float)) + sizeof(unsigned int), (void*)(4 * sizeof(float)));
    entityVertexArray.enableAttribute(3);
    entityVertexArray.setIntegerAttribute(3, 1, GL_UNSIGNED_INT, (8 * sizeof(float)) + sizeof(unsigned int), (void*)(8 * sizeof(float)));
    entityVertexBuffer.unbind();
    entityVertexArray.unbind();

    Image entityImage("/home/idk/code/minenture/bin/debug/entity_image.png");
    entityTextureArray.bind();
    entityTextureArray.setData(entityImage.getData());
    entityTextureArray.unbind();

    gl::ShaderStage entityVertexShaderStage(GL_VERTEX_SHADER);
    entityVertexShaderStage.setSource(EntityShader::VERTEX);
    entityVertexShaderStage.compile();
    entityShaderProgram.attachStage(entityVertexShaderStage.getHandle());
    gl::ShaderStage entityFragmentShaderStage(GL_FRAGMENT_SHADER);
    entityFragmentShaderStage.setSource(EntityShader::FRAGMENT);
    entityFragmentShaderStage.compile();
    entityShaderProgram.attachStage(entityFragmentShaderStage.getHandle());
    entityShaderProgram.link();
    //entityShaderProgram.detachSource(vertexSource);
    //entityShaderProgram.detachSource(fragmentSource);

    viewUniform.setLocation(entityShaderProgram.getHandle(), "viewMatrix");
    projectionUniform.setLocation(entityShaderProgram.getHandle(), "projectionMatrix");
    entityShaderProgram.bind();
    viewUniform.setData(glm::value_ptr(viewMatrix));
    projectionUniform.setData(glm::value_ptr(projectionMatrix));
    entityShaderProgram.unbind();

    




    shapeVertices.reserve(10000);

    shapeVertexBuffer.bind();
    shapeVertexBuffer.setData(nullptr, 0);
    shapeVertexBuffer.unbind();

    shapeVertexArray.bind();
    shapeVertexBuffer.bind();
    shapeVertexArray.enableAttribute(0);
    shapeVertexArray.setFloatingAttribute(0, 2, GL_FLOAT, false, (6 * sizeof(float)), (void*)(0));
    shapeVertexArray.enableAttribute(1);
    shapeVertexArray.setFloatingAttribute(1, 4, GL_FLOAT, false, (6 * sizeof(float)), (void*)(2 * sizeof(float)));
    shapeVertexBuffer.unbind();
    shapeVertexArray.unbind();

    gl::ShaderStage shapeVertexShaderStage(GL_VERTEX_SHADER);
    shapeVertexShaderStage.setSource(ShapeShader::VERTEX);
    shapeVertexShaderStage.compile();
    shapeShaderProgram.attachStage(shapeVertexShaderStage.getHandle());
    gl::ShaderStage shapeFragmentShaderStage(GL_FRAGMENT_SHADER);
    shapeFragmentShaderStage.setSource(ShapeShader::FRAGMENT);
    shapeFragmentShaderStage.compile();
    shapeShaderProgram.attachStage(shapeFragmentShaderStage.getHandle());
    shapeShaderProgram.link();
    //entityShaderProgram.detachSource(vertexSource);
    //entityShaderProgram.detachSource(fragmentSource);

    viewUniform.setLocation(shapeShaderProgram.getHandle(), "viewMatrix");
    projectionUniform.setLocation(shapeShaderProgram.getHandle(), "projectionMatrix");
    shapeShaderProgram.bind();
    viewUniform.setData(glm::value_ptr(viewMatrix));
    projectionUniform.setData(glm::value_ptr(projectionMatrix));
    shapeShaderProgram.unbind();









    //mouseBody.setType(CP_BODY_TYPE_KINEMATIC);

    tmp = entt::null;

    loadMaterials();
    loadDefinitions();

    loadWorld();
    testWorld();








}

Game::~Game()
{
    /*if(world)
    {
        if(world->space.contains(mousePivot))
        {
            world->space.remove(mousePivot);
        }
        if(world->space.contains(mouseGear))
        {
            world->space.remove(mouseGear);
        }

        world->space.remove(mouseBody);
    }*/
}
/*
void Game::fillInputs(Entity entity)
{
    if(world->registry.has<Input>(entity))
    {
        inputs.push_back(entity);
    }

    if(world->registry.has<Anchors>(entity))
    {
        for(const Anchor& anchor: world->registry.get<Anchors>(entity))
        {
            fillInputs(anchor.entity);
        }
    }
}*/

/*std::vector<Entity> Game::getFamily(Entity entity)
{
    std::vector<Entity> entities;

    entities.push_back(entity);

    if(world->registry.has<Anchors>(entity))
    {
        for(const Anchor& anchor: world->registry.get<Anchors>(entity).anchors)
        {
            
            if(anchor.entity != entt::null)
            {

                std::vector<Entity> relatives = getFamily(anchor.entity);
                entities.insert(entities.end(), relatives.begin(), relatives.end());
            }
        }
    }

    return(entities);
}*/

void Game::prepare()
{
    
    if(world)
    {
        /*if(world->registry.has<Control>())
        {
            std::vector<Entity> entities = getFamily(world->registry.attachee<Control>());

            for(Entity entity: entities)
            {
                std::cout << "name: " << world->registry.get<Base>(entity).name << std::endl;
            }
        }*/
    }


}

void Game::process(sf::Event& event)
{
    if(world)
    {
        /*if(world->registry.has<Control>())
        {
            Entity entity = world->registry.attachee<Control>();

            while(entity != entt::null)
            {
                if(world->registry.has<Input>(entity) == true)
                {
                    Input& input = world->registry.get<Input>(entity);

                    for(unsigned int index = 0; index < input.types.size(); index++)
                    {
                        sf::Event::EventType type = input.types[index];

                        if(type == event.type)
                        {
                            if(type == sf::Event::KeyPressed || type == sf::Event::KeyReleased)
                            {
                                if(input.keys[index] == event.key.code)
                                {
                                    Apply& apply = world->registry.accommodate<Apply>(entity);
                                    apply.actions.push_back(input.actions[index]);
                                }
                            }
                            else if(type == sf::Event::MouseButtonPressed || type == sf::Event::MouseButtonReleased)
                            {
                                if(input.buttons[index] == event.mouseButton.button)
                                {
                                    Apply& apply = world->registry.accommodate<Apply>(entity);
                                    apply.actions.push_back(input.actions[index]);
                                }
                            }
                            else
                            {
                                Apply& apply = world->registry.accommodate<Apply>(entity);
                                apply.actions.push_back(input.actions[index]);
                            }
                        }
                    }
                }
                
                if(world->registry.has<Anchor>(entity) == true)
                {
                    Anchor& anchor = world->registry.get<Anchor>(entity);
                    entity = anchor.entity;
                }
                else
                {
                    break;
                }
            }
        }*/

        switch(event.type)
        {


            case sf::Event::MouseMoved:
            {
                mousePosition.x = event.mouseMove.x;
                mousePosition.y = event.mouseMove.y;

                break;
            }
            case sf::Event::MouseButtonPressed:
            {

                switch(event.mouseButton.button)
                {
                    case sf::Mouse::Left:
                    {

                        world->createEntity(STONE_ENTITY, toCp(mousePosition));

                        /*
                        selectedEntity = world->getEntityOfShapeAtPosition(toCp(mousePosition));

                        if(selectedEntity != entt::null)
                        {
                            Body& selectedBody = world->registry.get<Body>(selectedEntity);

                            mousePivot.setBodyB(selectedBody);
                            mousePivot.setAnchorB(cpBodyWorldToLocal(selectedBody.handle, toCp(mousePosition)));
                            mouseGear.setBodyB(selectedBody);

                            world->space.add(mousePivot);
                            world->space.add(mouseGear);
                        }*/



                        break;
                    }
                    case sf::Mouse::Right:
                    {
                        //world->createStone(cpv(mousePosition.x, mousePosition.y));

                        /*if(holdingEntity != entt::null)
                        {
                            //Entity matchedEntity = matchEntity(holdingEntity);

                            if(matchedEntity != entt::null)
                            {
                                //attachEntity(holdingEntity, matchedEntity);
                            }
                        }*/

                        break;
                    }
                    default:
                    {
                        break;
                    }
                }



                
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                switch(event.mouseButton.button)
                {
                    case sf::Mouse::Left:
                    {


                        //mouseEntity = std::experimental::optional<Entity>();

                        /*if(holdingEntity != entt::null)
                        {
                            holdingEntity = entt::null;
                        }
                        if(world->space.contains(mousePivot))
                        {
                            world->space.remove(mousePivot);
                        }
                        if(world->space.contains(mouseGear))
                        {
                            world->space.remove(mouseGear);
                        }*/

                        break;
                    }
                    case sf::Mouse::Right:
                    {
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                

                break;
            }
            case sf::Event::MouseWheelScrolled:
            {
                //cpGearJointSetPhase(world->bvGear, cpGearJointGetPhase(world->bvGear) + event.mouseWheelScroll.delta * 0.1);
                //cpPinJointSetDist(world->rcPin, cpPinJointGetDist(world->rcPin) + event.mouseWheelScroll.delta * 1.0);
                //world->stones(cpv(mousePosition.x, mousePosition.y));
                /*if(world->space.contains(mouseGear))
                {
                    mouseGear.setPhase(mouseGear.getPhase() + event.mouseWheelScroll.delta * mouseRotationFactor);
                }*/

                break;
            }



            case sf::Event::Resized:
            {
                break;
            }
            case sf::Event::LostFocus:
            {
                break;
            }
            case sf::Event::GainedFocus:
            {
                break;
            }

            default:
            {
                break;
            }
        }
    }


}



void Game::update(double deltaTime)
{
    if(world)
    {


            //mouseBody.setVelocity(cpvmult(cpvsub(cpvlerp(mouseBody.getPosition(), cpv(mousePosition.x, mousePosition.y), mouseSomeFactor), mouseBody.getPosition()), mouseOtherFactor));



        /*// TODO: Move this to Game::process() ?
        entt::View<Entity, Apply> entities = world->registry.view<Apply>();
        for(Entity entity: entities)
        {
            Apply& apply = entities.get(entity);
            for(Action action: apply.actions)
            {
                switch(action)
                {
                    case ROTATE_CLOCKWISE:
                    {
                        Gear& gear = world->registry.get<Gear>(entity);
                        gear.addPhase(-0.1);

                        break;
                    }
                    case ROTATE_ANTICLOCKWISE:
                    {
                        Gear& gear = world->registry.get<Gear>(entity);
                        gear.addPhase(0.1);

                        break;
                    }

                    case ATTRACT_PIN:
                    {
                        Pin& pin = world->registry.get<Pin>(entity);
                        pin.addDistance(-1.0);

                        break;
                    }
                    case RETRACT_PIN:
                    {
                        Pin& pin = world->registry.get<Pin>(entity);
                        pin.addDistance(1.0);

                        break;
                    }


                    case ROTATE_TO_MOUSE:
                    {
                        //cpVect mouseDelta = cpvsub(ChipmunkDemoMouse, cpBodyGetPosition(tankBody));
                        //cpFloat turn = cpvtoangle(cpvunrotate(cpBodyGetRotation(tankBody), mouseDelta));
                        //cpBodySetAngle(tankControlBody, cpBodyGetAngle(tankBody) - turn);
    
                        Body& body = world->registry.get<Body>(entity);
                        cpVect mousePosition = cpv(currentMousePosition.x, currentMousePosition.y);
                        cpVect mp = cpvsub(mousePosition, body.getPosition());
                        double ma = cpvtoangle(cpvunrotate(body.getRotation(), mp));

                        //cpVect delta = cpv(currentMousePosition.x, currentMousePosition.y) - body.getPosition();
                        //double angle = cpvtoangle(delta);
                        //double angle = glm::angle(glm ::normalize(glm::fvec2(currentMousePosition) - body.getPosition()), glm::normalize(glm::fvec2(previousMousePosition) - body.getPosition()));
                        //double angle = cpvtoangle(cpvunrotate(body.getRotation(), cpvsub(cpv(currentMousePosition.x, currentMousePosition.y), body.getPosition())));
                        //double previousMouseAngle = std::atan2(previousMousePosition.x, previousMousePosition.y);
                        //double currentMouseAngle = std::atan2(currentMousePosition.x, currentMousePosition.y);
                        //double angle = std::atan2(currentMousePosition.x, currentMousePosition.y) - std::atan2(previousMousePosition.x, previousMousePosition.y);

                        Gear& gear = world->registry.get<Gear>(entity);
                        gear.addPhase(ma);

                        break;
                    }

                    case KISS:
                    {
                        Gear& gear = world->registry.get<Gear>(entity);
                        gear.addPhase(0.5);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }
        world->registry.reset<Apply>();*/

        world->update(deltaTime);
    }
}



void Game::render()
{    
    if(world)
    {


        entityVertices.clear();

        
        std::array<glm::vec2, 6> coordinates({glm::vec2(0.0f, 0.0f) ,glm::vec2(1.0f, 0.0f) ,glm::vec2(1.0f, 1.0f) ,glm::vec2(1.0f, 1.0f) ,glm::vec2(0.0f, 1.0f) ,glm::vec2(0.0f, 0.0f)});
        entt::View<Entity, Body, Texture> renderView = world->registry.view<Body, Texture>();
        for(Entity entity: renderView)
        {
            Body& body = renderView.get<Body>(entity);
            Texture& texture = renderView.get<Texture>(entity);

            for(unsigned int index = 0; index < 6; index++)
            {
                cpVect a = cpBodyLocalToWorld(body.handle, cpv(texture.vertices[index].x, texture.vertices[index].y));
                entityVertices.emplace_back(glm::vec2(a.x, a.y), coordinates[index], texture.identifier);
            }
        }

        entt::View<Entity, Body, Textures> renderVieww = world->registry.view<Body, Textures>();
        for(Entity entity: renderVieww)
        {
            Body& body = renderVieww.get<Body>(entity);
            Textures& textures = renderVieww.get<Textures>(entity);

            for(const Texture& texture: textures.textures)
            {
                for(unsigned int index = 0; index < 6; index++)
                {
                    cpVect a = cpBodyLocalToWorld(body.handle, cpv(texture.vertices[index].x, texture.vertices[index].y));
                    entityVertices.emplace_back(glm::vec2(a.x, a.y), coordinates[index], texture.identifier);
                }
            }
        }
        

/*
        entityVertices.clear();
        entt::View<Entity, Texture> renderView = world->registry.view<Texture>();

        for(Entity entity: renderView)
        {
            cpBody* bodyHandle = NULL; // REAL: Remove "= NULL".

            if(world->registry.has<Body>(entity) == true)
            {
                bodyHandle = world->registry.get<Body>(entity).handle;
            }
            else
            {
                if(world->registry.has<Parent>(entity)) // REAL: Remove this check.
                {
                    bodyHandle = world->registry.get<Body>(world->registry.get<Parent>(entity).entity).handle;
                }
            }

            Texture& texture = renderView.get(entity);

            for(unsigned int index = 0; index < 6; index += 1)
            {
                cpVect a = cpBodyLocalToWorld(bodyHandle, cpv(texture.vertices[index].x, texture.vertices[index].y));
                entityVertices.emplace_back(glm::vec2(a.x, a.y), texture.coordinates[index], texture.color, texture.identifier);
            }
        }*/

        entityVertexBuffer.bind();
        entityVertexBuffer.setData(&entityVertices[0], entityVertices.size() * sizeof(EntityVertex));
        entityVertexBuffer.unbind();

        if(collisionVisualization)
        {
            // TODO: Iterate only through root entities (those with a Body) and somehow only draw the outer vertices for bounding boxes.
            // TODO: Be able to draw boundaries of either shape or texture.

            shapeVertices.clear();
            //entt::View<Entity, Shape> debugView = world->registry.view<Shape>();
            //for(Entity entity: debugView)
            //{
                /*
                // Bounding box
                Shape& shape = debugView.get(entity);
                cpBB box = shape.getBox();
                std::array<glm::vec2, 4> boxVertices({glm::vec2(box.l, box.t), glm::vec2(box.l, box.b), glm::vec2(box.r, box.b), glm::vec2(box.r, box.t)});
                for(int shapeVertexSourceIndex = 0, shapeVertexTargetIndex = 3; shapeVertexSourceIndex < 4; shapeVertexTargetIndex = shapeVertexSourceIndex, shapeVertexSourceIndex++)
                {
                    std::array<ShapeVertex, 6> lineVertices = vertizeLine(boxVertices[shapeVertexSourceIndex], boxVertices[shapeVertexTargetIndex], collisionVisualizationHalfWidth, collisionVisualizationColor);

                    shapeVertices.insert(shapeVertices.end(), lineVertices.begin(), lineVertices.end());
                }
                */

                /*
                Shape& shape = debugView.get(entity);
                cpBody* bodyHandle = shape.getBodyHandle(); // TEMP: Because of TEMP above. Chipmunk's internal vertices are probably already transformed.
                for(int shapeVertexSourceIndex = 0, shapeVertexTargetIndex = 3; shapeVertexSourceIndex < 4; shapeVertexTargetIndex = shapeVertexSourceIndex, shapeVertexSourceIndex++)
                {
                    cpVect a = cpBodyLocalToWorld(bodyHandle, shape.vertices[shapeVertexSourceIndex]);
                    cpVect b = cpBodyLocalToWorld(bodyHandle, shape.vertices[shapeVertexTargetIndex]);
                    std::array<ShapeVertex, 6> lineVertices = vertizeLine(glm::vec2(a.x, a.y), glm::vec2(b.x, b.y), collisionVisualizationHalfWidth, collisionVisualizationColor);

                    shapeVertices.insert(shapeVertices.end(), lineVertices.begin(), lineVertices.end());
                }*/
            //}

            shapeVertexBuffer.bind();
            shapeVertexBuffer.setData(&shapeVertices[0], shapeVertices.size() * sizeof(ShapeVertex));
            shapeVertexBuffer.unbind();
        }
    }
}




void Game::draw()
{
    entityVertexArray.bind();
    entityTextureArray.bind();
    entityShaderProgram.bind();
    glDrawArrays(GL_TRIANGLES, 0, entityVertices.size());
    entityShaderProgram.unbind();
    entityTextureArray.unbind();
    entityVertexArray.unbind();

    if(collisionVisualization)
    {
        shapeVertexArray.bind();
        shapeShaderProgram.bind();
        glDrawArrays(GL_TRIANGLES, 0, shapeVertices.size());
        shapeShaderProgram.unbind();
        shapeVertexArray.unbind();
    }
}










void Game::loadWorld()
{
    world.emplace(definitions);

    world->test();
}



void Game::loadMaterials()
{
    materials.emplace_back("Default", 0.5, 0.5, 0.5);

    materials.emplace_back("Earth", 3.125, 1.0, 0.5);
    materials.emplace_back("Stone", 7.2, 1.0, 0.5);

    materials.emplace_back("Iron ore", 12.0, 1.0, 0.5);
    materials.emplace_back("Cast iron", 19.0, 1.0, 0.1);

    materials.emplace_back("Steel", 20.35, 0.5, 0.1);
    materials.emplace_back("Steel beam", 5.7, 1.0, 0.5);

    materials.emplace_back("Rubber", 1.0, 1.0, 0.5);
    materials.emplace_back("Wood", 1.0, 1.0, 0.1);
    materials.emplace_back("Ice", 2.35, 1.0, 0.5);
    materials.emplace_back("Gold", 50.25, 1.0, 0.5);
}



void Game::loadDefinitions()
{


    
    std::vector<cpVect> vertices({cpv(-8.0, -8.0), cpv(8.0, -8.0), cpv(8.0, 8.0), cpv(-8.0, 8.0)});

    definitions.emplace_back("Dirt", vertices, 0.1, materials[1], 127);
    definitions.emplace_back("Stone", vertices, 0.1, materials[2], 127);

    {
        definitions.emplace_back("Safe", vertices, 0.1, materials[2], {43, glm::vec2(0, 0)});
    }
    
    {
        definitions.emplace_back("Conveyor belt", vertices, 0.1, materials[6], 127);
        Definition& d = definitions.back();

    }
}

/*

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

*/


void Game::testWorld()
{
    world->create(definitions[0], cpv(500, 500));
}

/*


i have a class named Game, which has a function called loadWorld() and stores an instance of World. the resources (materials and item definitions) are stored in Game, because the resources are the same no matter which World is loaded.

logics needs access to resources, for example the player pulls a lever which spawns an item. the function createEntity(entityDefinition definition, Vector position) has to be a member of Game, since it needs access to resources.

is it more common to have all the logics and such in Game and let World only store the data?



    materials.emplace(std::piecewise_construct, std::forward_as_tuple("Wood"), std::forward_as_tuple(1.0, 1.0, 0.1));
    materials.emplace(std::piecewise_construct, std::forward_as_tuple("Iron"), std::forward_as_tuple(20.0, 1.0, 0.1));
    materials.emplace(std::piecewise_construct, std::forward_as_tuple("Steel"), std::forward_as_tuple(10.0, 0.5, 0.1));
    materials.emplace(std::piecewise_construct, std::forward_as_tuple("Rubber"), std::forward_as_tuple(5.0, 1.0, 0.5));


void Game::addAnchor()
{

}

void Game::createEntity() {}
void Game::destroyEntity() {}

void Game::adoptEntity() {}
void Game::leaveEntity() {}
becomes stepparent and stepchild

void Game::attachEntity() {}
void Game::detachEntity() {}

joinEntity()
partEntity()

marryEntity()
divorceEntity()

*/


// Use Parent and Child for both subentities and constraint entities? I think so... Depending on system's prupose, check if entity has constraint or body.
// I think I should skip the piston as arm for player for a while. Just use the mouse and check distance to entity you're about to interact with.
// Maybe fix the rendering and then mouse interaction! Then create single entities and assemble/adopt them.

// Maybe create sub entities separate too and then merge them. maybe remove body from sub and use the master's body instead.
// This will support breakable features later in game, for example when digging a larger chunk of multiple stone entities acting as a single entity,
// which then can be smashed in to smaller chunks and even to single stone entities. Also vehicles can break in middle. But not sure if thats good. How to repair?

// Parent and child is for sub entities. Anchor and constraints are for attached entities.













//Entity Game::createSubEntity(Entity parentIdentifier, glm::vec2 offset, std::uint16_t texture)
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


//void Game::test()
//{

    //world->createOrphanEntity("Stone", cpv(500.0, 500.0), materials.at("Wood"), 131);

//    world->bajs();
/*

    Entity piston = createPistonCylinder(cpv(564, 784));
    Entity found = createFoundation(cpv(500, 776));
    


    for(unsigned int x = 0; x < 50; x++)
    {
        Entity entity = world->registry.create();
        world->registry.assign<Base>(entity, "Stone");
        Body& body = world->registry.assign<Body>(entity, cpv(100 + x * 16, 800));
        world->space.add(body);
        body.setType(CP_BODY_TYPE_STATIC);
        Shape& shape = world->registry.assign<Shape>(entity, body, materials.at("Wood"));
        shape.setUserData(reinterpret_cast<void*>(static_cast<std::uintptr_t>(entity)));
        world->space.add(shape);
        world->registry.assign<Texture>(entity, 131);
    }
    */

//}


//void Game::testing()
//{
    //world->registry.get<Texture>(6).color = glm::vec4(1.0f, 0.0f, 0.0, 1.0f);

    //cpVect offset = cpv(-16.0, 0.0);
    //cpVect position = cpBodyLocalToWorld(world->registry.get<Body>(source).handle, offset);

    //std::experimental::optional<cp::Shape> shape = world->space.pointQueryNearest(position, 0.0, CP_SHAPE_FILTER_ALL, NULL);
//}





//void Game::printEntity(Entity entity)
//{
    /*std::cout << "\nEntity " << entity << "\n";
    std::cout << "Name " << (world->registry.has<Base>(entity) == true ? world->registry.get<Base>(entity).name : "") << "\n";
    std::cout << "Parent " << (world->registry.has<Parent>(entity) == true ? reinterpret_cast<char*>(world->registry.get<Parent>(entity).entity) : "") << "\n";
    std::cout << "Children" << "\n";
*///}


//void Game::constrainEntity() {}
//void Game::releaseEntity() {}




/*
    TODO: Change from "source" and "target" to "slave" and "master"?
    TODO: Define and declare maximumDistance for query somewhere else?
*/
//Entity Game::matchEntity(Entity entity)
//{

/*
    double minimumDistance;
    Entity anchoringEntity = entt::null;

    Entity sourceParentEntity = world->registry.get<Parent>(holdingEntity).entity;
    for(Entity sourceChildEntity: world->registry.get<Children>(sourceParentEntity).entities)
    {
        if(world->registry.has<Anchor>(sourceChildEntity) == true) // Child entity has Anchor component.
        {
            if(world->registry.get<Anchor>(sourceChildEntity).entity == entt::null) // Child entity does not have any other entity anchored yet.
            {
                Shape& sourceChildShape = world->registry.get<Shape>(sourceChildEntity);
                Body& sourceParentBody = world->registry.get<Body>(sourceParentEntity);
                cpVect sourceChildPosition = cpBodyLocalToWorld(sourceParentBody.handle, sourceChildShape.offset);

                cpShapeFilter filter = {sourceParentEntity, JAJA, ANCHOR};
                std::experimental::optional<cp::Shape> targetShape = world->space.pointQueryNearest(sourceChildPosition, 0.0, filter, NULL);


                if(targetShape)
                {
                    Entity targetChildEntity = static_cast<Entity>(reinterpret_cast<std::uintptr_t>(targetShape->getUserData()));
                    if(world->registry.has<Anchor>(targetChildEntity) == true)
                    {
                        if(world->registry.get<Anchor>(targetChildEntity).entity == entt::null)
                        {
                            // A match was found!

                            // Calculate distance between the two entities.

                            Entity targetParentEntity = world->registry.get<Parent>(targetChildEntity).entity;
                            Body& targetParentBody = world->registry.get<Body>(targetParentEntity);
                            cpVect targetChildPosition = cpBodyLocalToWorld(targetParentBody.handle, world->registry.get<Shape>(targetChildEntity).offset);

                            double currentDistance = cpvlength(cpvnormalize(cpvsub(sourceChildPosition, targetChildPosition)));
                            if(currentDistance < minimumDistance || anchoringEntity == entt::null)
                            {
                                minimumDistance = currentDistance;
                                anchoringEntity = targetChildEntity;
                            }
                        }
                    }
                }
            }
        }
    }

    return(anchoringEntity);*/
//}



/*
    TODO: Change from "source" and "target" to "slave" and "master"?
*/
//void Game::attachEntity(Entity source, Entity target)
//{
/*

    world->registry.get<Texture>(source).color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    world->registry.get<Texture>(target).color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
*/

    /*

    if(world->registry.has<Anchors>(target)) // If target entity has an anchor component.
    {
        Anchors& anchors = world->registry.get<Anchors>(target);

        // Get the anchor closest to ?.


        if(anchor.entity == entt::null) // If anchor is not already occupied.
        {
            if(world->registry.has<Pin>(source))
            {
                Pin& pin = world->registry.get<Pin>(source);
                pin.setBodyB(world->registry.get<Body>(target));
                pin.setAnchorB(anchor.position);
                pin.setDistance(10); // TEMP

                world->space.add(pin);

                anchor.entity = source;

            }

            if(world->registry.has<Pivot>(source))
            {
                Pivot& pivot = world->registry.get<Pivot>(source);
                pivot.setBodyB(world->registry.get<Body>(target));
                pivot.setAnchorB(anchor.position);

                world->space.add(pivot);

                anchor.entity = source;

            }

            if(world->registry.has<Gear>(source))
            {
                Gear& gear = world->registry.get<Gear>(source);
                gear.setBodyB(world->registry.get<Body>(target));
                //gear.setPhase(world->registry.get<Body>(target).getAngle()); // Is this needed?
                world->space.add(gear);

                anchor.entity = source;

            }

            if(world->registry.has<Groove>(source))
            {
                Groove& groove = world->registry.get<Groove>(source);
                groove.setBodyB(world->registry.get<Body>(target));
                groove.setAnchorB(anchor.position);
                world->space.add(groove);

                anchor.entity = source;

            }
        }
        else
        {
            std::cout << "Anchor is already occupied!" << std::endl;
        }
    }
    else
    {
        std::cout << "Target has no anchor!" << std::endl;
    }*/
//}


/*
void Game::detachEntity(Entity source, Entity target)
{

}

Entity Game::createEntity(EntityType type, cpVect position, cpGroup group)
{
    
    switch(type)
    {

        case ROD_ENTITY:
        {
            Entity entity = world->registry.create();
            world->registry.assign<Base>(entity, "Rod");
            Body& body = world->registry.assign<Body>(entity, position);
            Shape& shape = world->registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-24.0, -6.0), cpv(22.0, -6.0), cpv(22.0, 6.0), cpv(-24.0, 6.0)}), 0.1, materials.at("Wood"));
            world->registry.assign<Model>(entity, std::vector<Sprite>({Sprite(glm::vec2(-16, -0), 175), Sprite(glm::vec2(0, 0), 176), Sprite(glm::vec2(16, 0), 177)}));
            world->registry.assign<Pin>(entity, body, cpv(0, 0));
            world->registry.assign<Gear>(entity, body); // Do this really need a gear? It has a groove already...
            world->registry.assign<Groove>(entity, body, cpvzero, cpv(-32, 0));




            Input& input = world->registry.assign<Input>(entity);
            input.bind(sf::Event::KeyPressed, sf::Keyboard::R, RETRACT_PIN);
            input.bind(sf::Event::KeyPressed, sf::Keyboard::F, ATTRACT_PIN);




            shape.setFilterGroup(group); // TODO: Merge this into shape constructor.

            world->space.add(body);
            world->space.add(shape);

            return(entity);
        }

        case CYLINDER_ENTITY:
        {    
            Entity entity = world->registry.create();

            world->registry.assign<Base>(entity, "Cylinder");
            Body& body = world->registry.assign<Body>(entity, position);
            Shape& shape = world->registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-22.0, -6.0), cpv(24.0, -6.0), cpv(24.0, 6.0), cpv(-22.0, 6.0)}), 0.1, materials.at("Wood"));
            world->registry.assign<Model>(entity, std::vector<Sprite>({Sprite(glm::vec2(-16, -0), 172), Sprite(glm::vec2(0, 0), 173), Sprite(glm::vec2(16, 0), 174)}));

            Input& input = world->registry.assign<Input>(entity);
            input.bind(sf::Event::KeyPressed, sf::Keyboard::Q, ROTATE_ANTICLOCKWISE);
            input.bind(sf::Event::KeyPressed, sf::Keyboard::E, ROTATE_CLOCKWISE);

            world->registry.assign<Anchor>(entity, cpv(16, 0));
            world->registry.assign<Anchors>(entity, std::vector<Anchor>({Anchor(cpv(16, 0))}));
            
            world->registry.assign<Pivot>(entity, body, cpv(-16, 0));

            body.setMass(1);
            Gear& gear = world->registry.assign<Gear>(entity, body);
            gear.setMaxForce(100000000); // TEMP!!!
            //gear.setErrorBias(); // TEMP!!!
            //gear.setMaxBias(10); // TEMP!!!

            shape.setFilterGroup(group); // TODO: Merge this into shape constructor.

            world->space.add(body);
            world->space.add(shape);

            return(entity);
        }

        case PISTON_ENTITY:
        {

            break;
        }

        case ROBOT_ENTITY:
        {
            Entity entity = world->registry.create();
            world->registry.assign<Base>(entity, "Body");
            Body& body = world->registry.assign<Body>(entity, position);
            Shape& shape = world->registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(16.0, 24.0), cpv(-16.0, 24.0), cpv(-16.0, -24.0), cpv(16.0, -24.0)}), 0.1, materials.at("Wood"));
            world->registry.assign<Model>(entity, std::vector<Sprite>({Sprite(glm::vec2(-8, -16), 178), Sprite(glm::vec2(8, -16), 179), Sprite(glm::vec2(-8, 0), 218), Sprite(glm::vec2(8, 0), 219), Sprite(glm::vec2(-8, 16), 258), Sprite(glm::vec2(8, 16), 259)}));
            world->registry.assign<Input>(entity); // Remove this later!
            world->registry.assign<Anchors>(entity, std::vector<Anchor>({Anchor(cpv(0, -16)), Anchor(cpv(0, 16))}));


            world->space.add(body);
            world->space.add(shape);

            return(entity);
        }

        case PLAYER_ENTITY:
        {
            Entity robotEntity = createEntity(ROBOT_ENTITY, position);
            world->registry.get<Body>(robotEntity).setMoment(INFINITY);
            world->registry.get<Shape>(robotEntity).setFilterGroup(robotEntity);

            world->registry.assign<Control>(entt::tag_t{}, robotEntity);
            // Possible to override input bindings of children here.

            Entity cylinderEntity = createEntity(CYLINDER_ENTITY, cpvadd(position, cpv(16, -16)), robotEntity);
            attachEntity(cylinderEntity, robotEntity);

            Entity rodEntity = createEntity(ROD_ENTITY, cpvadd(cpvadd(position, cpv(16, -16)), cpv(16, 0)), robotEntity);
            attachEntity(rodEntity, cylinderEntity);



            return(robotEntity);
        }

        case WHEEL_ENTITY:
        {
            Entity entity = world->registry.create();

            world->registry.assign<Base>(entity, "Wheel");

            Body& body = world->registry.assign<Body>(entity, position);
            world->space.add(body);

            Shape& shape = world->registry.assign<Shape>(entity, body, 16.0, materials.at("Rubber"));
            world->space.add(shape);

            world->registry.assign<Model>(entity, std::vector<Sprite>({Sprite(glm::vec2(-8, -8), 280), Sprite(glm::vec2(8, -8), 281), Sprite(glm::vec2(-8, 8), 320), Sprite(glm::vec2(8, 8), 321)}));

            //world->registry.assign<Motor>

            return(entity);
        }


        case VEHICLE_ENTITY:
        {
            Entity entity = world->registry.create();

            world->registry.assign<Base>(entity, "Vehicle");

            Body& body = world->registry.assign<Body>(entity, position);
            world->space.add(body);

            Shape& shape = world->registry.assign<Shape>(entity, body, std::vector<cpVect>({cpv(-88.0, 24.0), cpv(-88.0, 8.0), cpv(-72.0, 8.0), cpv(-56.0, -8.0), cpv(-40.0, -8.0), cpv(-24.0, 8.0), cpv(8.0, 8.0), cpv(24.0, -8.0), cpv(40.0, -8.0), cpv(56.0, -24.0), cpv(88.0, -24.0), cpv(88.0, 24.0)}), 0.1, materials.at("Wood"));
            world->space.add(shape);

            world->registry.assign<Model>(entity, std::vector<Sprite>({Sprite(glm::ivec2(80, -16), 170), Sprite(glm::ivec2(64, -16), 169), Sprite(glm::ivec2(48, -16), 168), Sprite(glm::ivec2(-64, 0), 201), Sprite(glm::ivec2(-48, 0), 202), Sprite(glm::ivec2(-32, 0), 203), Sprite(glm::ivec2(16, 0), 206), Sprite(glm::ivec2(32, 0), 207), Sprite(glm::ivec2(48, 0), 208), Sprite(glm::ivec2(64, 0), 209), Sprite(glm::ivec2(80, 0), 210), Sprite(glm::ivec2(-80, 16), 240), Sprite(glm::ivec2(-64, 16), 241), Sprite(glm::ivec2(-48, 16), 242), Sprite(glm::ivec2(-32, 16), 243), Sprite(glm::ivec2(-16, 16), 244), Sprite(glm::ivec2(0, 16), 245), Sprite(glm::ivec2(16, 16), 246), Sprite(glm::ivec2(32, 16), 247), Sprite(glm::ivec2(48, 16), 248), Sprite(glm::ivec2(64, 16), 249), Sprite(glm::ivec2(80, 16), 250)}));

            world->registry.assign<Anchors>(entity, std::vector<Anchor>({Anchor(cpv(16, 0))}));
            


            return(entity);
        }

        case EXCAVATOR_ENTITY:
        {
            Entity rootEntity = createEntity(VEHICLE_ENTITY, position);

            //Entity leftWheelEntity = createEntity(WHEEL_ENTITY, cpvadd(position, ))



            return(rootEntity);
        }


        default:
        {
            return(entt::null);
        }
    }
}
*/




/*
void Game::test()
{

    //testRoom();

    //createEntity(PLAYER_ENTITY, cpv(500, 740));
    //createEntity(EXCAVATOR_ENTITY, cpv(700, 740));
    //createPlayer(cp::Vector(500.0, 740.0));
    //testVehicle(cp::Vector(800.0, 740.0));
    //testWheel(cp::Vector(705.0, 740.0));
    //testStones(cp::Vector(800.0, 500.0));
}



Entity Game::testStone(cp::Vector position)
{
    Entity entity = world->registry.create();

    Base& base = world->registry.assign<Base>(entity, "Stone");

    Body& body = world->registry.assign<Body>(entity, position);
    world->space.add(body);

    std::vector<cpVect> vertices = {cpv(-8.0, -8.0), cpv(8.0, -8.0), cpv(8.0, 8.0), cpv(-8.0, 8.0)};
    Shape& shape = world->registry.assign<Shape>(entity, body, vertices, 0.1, materials.at("Wood"));
    shape.setFriction(1.0);
    world->space.add(shape);

    std::array<Sprite, 1> sprites = {Sprite(glm::ivec2(0, 0), 131)};
    Model& model = world->registry.assign<Model>(entity, sprites);

    return(entity);
}

void Game::testStones(cp::Vector position)
{
    for(unsigned int y = position.y; y < 17 * 5 + position.y; y += 17)
    {
        for(unsigned int x = position.x; x < 17 * 5 + position.x; x += 17)
        {
            testStone(cp::Vector(x, y));
        }
    }
}


void Game::testRoom()
{
    for(unsigned int x = 16; x < 1700; x += 16)
    {
        Entity entity = testStone(cp::Vector(x, 800.0));
        Body& body = world->registry.get<Body>(entity);
        body.setType(CP_BODY_TYPE_STATIC);
    }
}*/

/*
std::array<ShapeVertex, 6> Game::vertizeLine(const glm::vec2& source, const glm::vec2& target, float halfWidth, const glm::vec4& color)
{
    glm::vec2 normal = glm::normalize(glm::vec2(source.y - target.y, (source.x - target.x) * -1)) * halfWidth;
    std::array<ShapeVertex, 6> vertices({ShapeVertex(source + normal, color), ShapeVertex(target + normal, color), ShapeVertex(target - normal, color), ShapeVertex(target - normal, color), ShapeVertex(source - normal, color), ShapeVertex(source + normal, color)});

    return(vertices);
}*/











