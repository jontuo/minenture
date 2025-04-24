
#include "game.hpp"
#include <iostream>
#include "clock.hpp"
//#include "setting.hpp"
#include "glad/glad.h"


#include "glpp/shader_stage.hpp"

#include "entity_shader.hpp"
#include "image.hpp"
#include "model.hpp"
#include <glm/ext.hpp>
#include "timer.hpp"

#include "imgui.h"
#include "chipmunkpp/shape.hpp"
#include <limits> // std::numeric_limits<Entity>::max()

Game::Game():
    contextSettings(CONTEXT_DEPTH_BIT, CONTEXT_STENCIL_BIT, CONTEXT_ANTIALIASING, CONTEXT_MAJOR_VERSION, CONTEXT_MINOR_VERSION),
    window(sf::VideoMode::getDesktopMode(), WINDOW_TITLE, sf::Style::Fullscreen, contextSettings),

    glader(),
    entityVertexBuffer(GL_ARRAY_BUFFER, GL_STREAM_DRAW),
    entityVertexArray(),
    entityTextureArray(),
    entityShaderProgram(),
    gui()
{



    projectionMatrix = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);
    projectionMatrix = projectionMatrix;
    //viewMatrix = glm::translate(viewMatrix, glm::vec3(110.0f, 110.0f, 0.0f));
    //viewMatrix = glm::scale(viewMatrix, glm::vec3(2.0f, 2.0f, 1.0f));

    entityVertexBuffer.bind();
    entityVertexBuffer.setData(nullptr, 0);
    entityVertexBuffer.unbind();

    entityVertexArray.bind();
    entityVertexBuffer.bind();
    entityVertexArray.enableAttribute(0);
    entityVertexArray.setFloatingAttribute(0, 2, GL_FLOAT, false, (4 * sizeof(float)) + sizeof(unsigned int), (void*)(0));
    entityVertexArray.enableAttribute(1);
    entityVertexArray.setFloatingAttribute(1, 2, GL_FLOAT, false, (4 * sizeof(float)) + sizeof(unsigned int), (void*)(2 * sizeof(float)));
    entityVertexArray.enableAttribute(2);
    entityVertexArray.setIntegerAttribute(2, 1, GL_UNSIGNED_INT, (4 * sizeof(float)) + sizeof(unsigned int), (void*)(4 * sizeof(float)));
    entityVertexBuffer.unbind();
    entityVertexArray.unbind();

    Image entityImage("/home/idk/code/minenture/bin/debug/entity_image.png");
    entityTextureArray.bind();
    entityTextureArray.setData(entityImage.getData());
    entityTextureArray.unbind();

    gl::ShaderStage entityVertexShaderStage(GL_VERTEX_SHADER);
    entityVertexShaderStage.setSource(EntityShaderStage::VERTEX);
    entityVertexShaderStage.compile();
    entityShaderProgram.attachStage(entityVertexShaderStage.getHandle());
    gl::ShaderStage entityFragmentShaderStage(GL_FRAGMENT_SHADER);
    entityFragmentShaderStage.setSource(EntityShaderStage::FRAGMENT);
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

    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    glViewport(0, 0, window.getSize().x, window.getSize().y);
    glDisable(GL_DEPTH_TEST);
    glClearColor(BACKGROUND_COLOR_RED, BACKGROUND_COLOR_GREEN, BACKGROUND_COLOR_BLUE, 1.0f);




























    load();

    targetUpdateTime = TARGET_UPDATE_TIME;
    debugWindow = true;
    demoWindow = false;
    pause = false;
    run = true;
}

Game::~Game()
{
}



void Game::start()
{
    MicroSecond currentFrameTime = 0;
    MicroSecond previousFrameTime = Clock::getMicro();
    MicroSecond differentFrameTime = 1; // ???
    MicroSecond previousUpdateTime = 0;

    while(run == true)
    {
        currentFrameTime = Clock::getMicro();
        differentFrameTime = currentFrameTime - previousFrameTime;
        previousFrameTime = currentFrameTime;
        
        process();

        if(pause == false)
        {
            previousUpdateTime = previousUpdateTime + differentFrameTime;
            while(previousUpdateTime > targetUpdateTime)
            {
                update();

                previousUpdateTime = previousUpdateTime - targetUpdateTime;
            }
        }

        render();
        draw();
    }
}

void Game::stop()
{
    run = false;
}

void Game::process()
{
    gui.begin(window);

    sf::Event event;
    while(window.pollEvent(event))
    {
        gui.process(event);

        switch(event.type)
        {
            case sf::Event::MouseMoved:
            {
                mouseScreenPosition.x = event.mouseMove.x;
                mouseScreenPosition.y = event.mouseMove.y;

                cpVect newPosition = cpvlerp(world->mouseBody.getPosition(), cpv(mouseScreenPosition.x, mouseScreenPosition.y), 0.25);
                world->mouseBody.setVelocity(cpvmult(cpvsub(newPosition, world->mouseBody.getPosition()), 60.0));
                world->mouseBody.setPosition(newPosition);

                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                ImGuiIO& io = ImGui::GetIO();
                if(io.WantCaptureMouse == false)
                {
                    entitySelected = world->getNearestEntity(mouseScreenPosition.x, mouseScreenPosition.y, 0.0);

                    std::experimental::optional<cp::Shape> shape = world->space.pointQueryNearest(cpv(mouseScreenPosition.x, mouseScreenPosition.y), 0.0, CP_SHAPE_FILTER_ALL, NULL);
                    if(shape)
                    {
                        cp::Body body = cp::Body(shape->getBodyHandle());
                        world->mousePivot.setBodyB(body);
                        world->mousePivot.setAnchorB(cpBodyWorldToLocal(body.handle, cpv(mouseScreenPosition.x, mouseScreenPosition.y)));
                        world->space.add(world->mousePivot);
                        body = cp::Body(shape->getBodyHandle());
                        world->mouseGear.setBodyB(body);
                        world->space.add(world->mouseGear);
                    }
                }

                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                ImGuiIO& io = ImGui::GetIO();
                if(io.WantCaptureMouse == false)
                {
                    entitySelected = std::experimental::optional<Entity>();

                    if(world->space.contains(world->mousePivot))
                    {
                        world->space.remove(world->mousePivot);
                    }
                    if(world->space.contains(world->mouseGear))
                    {
                        world->space.remove(world->mouseGear);
                    }
                }

                break;
            }
            case sf::Event::MouseWheelScrolled:
            {
                ImGuiIO& io = ImGui::GetIO();
                if(io.WantCaptureMouse == false)
                {
                    if(world->space.contains(world->mouseGear))
                    {
                        world->mouseGear.setPhase(world->mouseGear.getPhase() + event.mouseWheelScroll.delta);
                    }
                }
            }
            case sf::Event::KeyPressed:
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::F1:
                    {
                        world->testt();

                        break;
                    }
                    case sf::Keyboard::F2:
                    {
                        pause = !pause;

                        break;
                    }
                    case sf::Keyboard::F3:
                    {
                        update();

                        break;
                    }
                    case sf::Keyboard::F11:
                    {
                    }
                    case sf::Keyboard::F12:
                    {
                        debugWindow = !debugWindow;

                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                break;
            }
            case sf::Event::Closed:
            {
                stop();

                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void Game::update()
{
    if(world)
    {
        world->update(SPACE_TIME_STEP);
    }
}

void Game::render()
{

    
    if(world)
    {
        vertices.clear();

        auto view = world->registry.view<cp::Shape, Model>();
        for(auto entity: view)
        {
            cp::Shape& shape = view.get<cp::Shape>(entity);
            Model& model = view.get<Model>(entity);

            for(unsigned int index = 0; index < model.mesh.vertices.size(); index = index + 1)
            {
                vertices.emplace_back(cpBodyLocalToWorld(shape.getBodyHandle(), model.mesh.vertices[index]), model.texture.coordinates[index], model.texture.identifier);
            }
        }


        entityVertexBuffer.bind();
        entityVertexBuffer.setData(&vertices[0], vertices.size() * sizeof(Vertex));
        entityVertexBuffer.unbind();
    }

    /*
    if(world)
    {
        vertices.clear();

        auto view = world->registry.view<cp::Body, Model>();
        for(auto entity: view)
        {
            cp::Body& body = view.get<cp::Body>(entity);
            Model& model = view.get<Model>(entity);

            for(Mesh& mesh: model.meshes)
            {
                for(unsigned int index = 0; index < mesh.size; index++)
                {
                    vertices.emplace_back(cpvadd(cpBodyLocalToWorld(body.getHandle(), cpv(mesh.vertices[index].x, mesh.vertices[index].y)), cpv(mesh.offset.x, mesh.offset.y)), mesh.coordinates[index].x, mesh.coordinates[index].y, mesh.texture);
                }
            }
        }

        entityVertexBuffer.bind();
        entityVertexBuffer.setData(&vertices[0], vertices.size() * sizeof(Vertex));
        entityVertexBuffer.unbind();
    }*/

    


        if(debugWindow)
        {
            ImGui::SetNextWindowSize(ImVec2(500, 500));
            //ImGui::SetNextWindowPosCenter();
            ImGui::Begin("Debug", &debugWindow);

            //ImGui::Text("%u milliseconds per frame.", smoothDeltaTime/1000);
            //ImGui::Text("%.0f frames per second.", smoothRateTime);
            //ImGui::Text("Entities\t%u awake\t%u sleep\t%u total", world.awakeEntities, world.sleepEntities, (unsigned int)world.entities.size());
            // Add here so you can set space.setSleep~


            // https://github.com/ocornut/imgui/issues/1729
            // https://github.com/ocornut/imgui/labels/plot%2Fgraph






            ImGui::Text("\n");
            ImGui::Text("\n");

            ImGui::Text("\n");
            ImGui::Text("\n");

            if(ImGui::CollapsingHeader("Time"))
            {

                //ImGui::Text("Frame: difference %u", differentFrameTime);
                //ImGui::Text("Step: previous %u, target %u", previousUpdateTime, targetUpdateTime);
                ImGui::Text("Render ");
                ImGui::Text("Draw ");
                ImGui::Text("Total ");

                if(ImGui::Button("Pause") == true)
                {
                    pause = !pause;
                }
                if(ImGui::Button("Step") == true)
                {
                    update();
                }



            }
            if(ImGui::CollapsingHeader("Info"))
            {
                if(ImGui::TreeNode("General"))
                {
                    ImGui::Text("\n");

                    
                    //ImGui::Text( "Mouse position: screen (%u, %u), world ()", mousePosition.x, mousePosition.y);

                    //glm::ivec2 worldPosition = screenToWorldPosition( mousePosition );
                    //ImGui::Text( "Mouse: (%u, %u) screen, (%i, %i) world.", mousePosition.x, mousePosition.y, worldPosition.x, worldPosition.y );
                        // Add more info of mouse; world, chunk, tile (info of tile too?).
                        // About tile info, maybe put it somewhere else. Select the tile, then see info of selected tile somewhere.
                        // There you can also choose action, like destroy tile.

                    ImGui::Text("\n");
                    //ImGui::Text( "Game state: %s", stateNames[ static_cast< int >( state ) ] );

                    ImGui::Text("\n");
                    //ImGui::Text( "Entities: %u instances, %u vertices.", world.entities.count, world.vertices.size() );
                    

                    ImGui::Text("\n");
                    ImGui::TreePop();
                }

                if(ImGui::TreeNode("Time"))
                {
                    ImGui::Text("\n");
                    //ImGui::Text("a: %u ms", deltaTime);

                    //ImGui::PlotVar("Speed", (float)(deltaTime/1000));
                    //ImGui::PlotVarFlushOldEntries(); // PERF: Call this once in a while, not every frame.


                    ImGui::Text("\n");
                    ImGui::TreePop();
                }

                if(ImGui::TreeNode("Other"))
                {
                    ImGui::Text("\n");

                    



                    ImGui::Text("\n");
                    ImGui::TreePop();
                }
            }

            if(ImGui::CollapsingHeader("Data"))
            {






                ImGui::Text("Mouse position");
                ImGui::Text("\tScreen (%u, %u)", mouseScreenPosition.x, mouseScreenPosition.y);

                
                ImGui::Separator();

                ImGui::Text("World");
                if(world)
                {
                    ImGui::Text("Name:");           ImGui::SameLine(100);       ImGui::Text("%s", world->name.c_str());
                    ImGui::Text("Entities:");       ImGui::SameLine(100);       ImGui::Text("%u total, %u record, %u maximum", world->registry.size(), world->registry.capacity(), std::numeric_limits<Entity>::max());
                }
                else
                {
                    ImGui::Text("Name:");
                    ImGui::Text("Entities:");
                }

                ImGui::Separator();
            
                ImGui::Text("Entity");
                if(entitySelected)
                {
                    Entity identifier = entitySelected.value();
                    cp::Body& body = world->registry.get<cp::Body>(identifier);
                    cp::Shape& shape = world->registry.get<cp::Shape>(identifier);


                    ImGui::Text("General");
                    ImGui::Text("Identifier:");     ImGui::SameLine(100);       ImGui::Text("%u", identifier);
                    ImGui::Text("Mass:");           ImGui::SameLine(100);       ImGui::Text("%.1f", body.getMass());
                    ImGui::Text("Center:");         ImGui::SameLine(100);       ImGui::Text("%.1f", body.getCenterOfGravity());
                    
                    ImGui::Text("Linear");
                    ImGui::Text("Position:");       ImGui::SameLine(100);       ImGui::Text("%.1f, %.1f", body.getPosition().x, body.getPosition().y);
                    ImGui::Text("Velocity:");       ImGui::SameLine(100);       ImGui::Text("%.1f, %.1f", body.getVelocity().x, body.getVelocity().y);
                    ImGui::Text("Force:");          ImGui::SameLine(100);       ImGui::Text("%.1f, %.1f", body.getForce().x, body.getForce().y); // Force applied to center of gravity.

                    ImGui::Text("Angular");
                    ImGui::Text("Angle:");          ImGui::SameLine(100);       ImGui::Text("%.1f", body.getAngle());
                    ImGui::Text("Speed:");          ImGui::SameLine(100);       ImGui::Text("%.1f", body.getAngularVelocity());
                    ImGui::Text("Torque:");         ImGui::SameLine(100);       ImGui::Text("%.1f", body.getTorque());
                    ImGui::Text("Moment:");         ImGui::SameLine(100);       ImGui::Text("%.1f", body.getMoment()); // Moment of inertia, the rotational mass.
                }
                else
                {
                    ImGui::Text("General");
                    ImGui::Text("Identifier:");
                    ImGui::Text("Mass:");
                    ImGui::Text("Center:");
                    
                    ImGui::Text("Linear");
                    ImGui::Text("Position:");
                    ImGui::Text("Velocity:");
                    ImGui::Text("Force:");

                    ImGui::Text("Angular");
                    ImGui::Text("Angle:");
                    ImGui::Text("Speed:");
                    ImGui::Text("Torque:");
                    ImGui::Text("Moment:");
                }


                /*auto view = world->registry.view<cp::Body, Model>();
                for(auto entity: view)
                {
                    cp::Body& body = view.get<cp::Body>(entity);
                    Model& model = view.get<Model>(entity);

                    ImGui::PushID(entity);
                    bool treeNode = ImGui::TreeNode("Object", "%u", entity);
                    if(treeNode)
                    {
                        ImGui::Text("hej");
                        ImGui::TreePop();
                    }


                    ImGui::PopID();

                }*/





                    /*

static void ShowExampleAppPropertyEditor(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(430,450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Example: Property editor", p_open))
    {
        ImGui::End();
        return;
    }

    ShowHelpMarker("This example shows how you may implement a property editor using two columns.\nAll objects/fields data are dummies here.\nRemember that in many simple cases, you can use ImGui::SameLine(xxx) to position\nyour cursor horizontally instead of using the Columns() API.");

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));
    ImGui::Columns(2);
    ImGui::Separator();

    struct funcs
    {
        static void ShowDummyObject(const char* prefix, int uid)
        {
            ImGui::PushID(uid);                      // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
            ImGui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
            bool node_open = ImGui::TreeNode("Object", "%s_%u", prefix, uid);
            ImGui::NextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("my sailor is rich");
            ImGui::NextColumn();
            if (node_open)
            {
                static float dummy_members[8] = { 0.0f,0.0f,1.0f,3.1416f,100.0f,999.0f };
                for (int i = 0; i < 8; i++)
                {
                    ImGui::PushID(i); // Use field index as identifier.
                    if (i < 2)
                    {
                        ShowDummyObject("Child", 424242);
                    }
                    else
                    {
                        ImGui::AlignTextToFramePadding();
                        // Here we use a Selectable (instead of Text) to highlight on hover
                        //ImGui::Text("Field_%d", i);
                        char label[32];
                        sprintf(label, "Field_%d", i);
                        ImGui::Bullet();
                        ImGui::Selectable(label);
                        ImGui::NextColumn();
                        ImGui::PushItemWidth(-1);
                        if (i >= 5)
                            ImGui::InputFloat("##value", &dummy_members[i], 1.0f);
                        else
                            ImGui::DragFloat("##value", &dummy_members[i], 0.01f);
                        ImGui::PopItemWidth();
                        ImGui::NextColumn();
                    }
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    };

    // Iterate dummy objects with dummy members (all the same data)
    for (int obj_i = 0; obj_i < 3; obj_i++)
        funcs::ShowDummyObject("Object", obj_i);

    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::PopStyleVar();
    ImGui::End();
}
                    if(ImGui::TreeNode("Components"))
                    {
                        for(int i = 0; i < 5; i++)
                            if(ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
                            {
                                ImGui::Text("blah blah");
                                ImGui::SameLine(); 
                                if(ImGui::SmallButton("button")) { };
                                ImGui::TreePop();
                            }
                        ImGui::TreePop();
                    }*/









            }

            if(ImGui::CollapsingHeader("Other"))
            {

                if(ImGui::Button("Demo", ImVec2(100, 20)))
                {
                    demoWindow = !demoWindow;
                }
            }


            ImGui::End();
        }

        if(demoWindow)
        {
            ImGui::SetNextWindowSize(ImVec2(0, 0));
            ImGui::ShowTestWindow();
        }
}

void Game::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if(world)
    {
        entityVertexArray.bind();
        entityTextureArray.bind();
        entityShaderProgram.bind();
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        entityShaderProgram.unbind();
        entityTextureArray.unbind();
        entityVertexArray.unbind();
    }

    gui.render();
    gui.draw(); 

    window.display();
}

void Game::load()
{
    world.emplace();

    world->test();


}