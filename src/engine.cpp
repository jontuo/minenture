
#include "engine.hpp"
#include "clock.hpp"
#include "glad/glad.h"
#include <iostream>
#include "imgui.h"


Engine::Engine():
    window(sf::VideoMode::getDesktopMode(), WINDOW_TITLE, sf::Style::Fullscreen, sf::ContextSettings(CONTEXT_DEPTH_BIT, CONTEXT_STENCIL_BIT, CONTEXT_ANTIALIASING, CONTEXT_MAJOR_VERSION, CONTEXT_MINOR_VERSION)),
    glader(),
    game(),
    gui()

{

    if(WINDOW_VERTICAL_SYNCHRONIZATION == true)
        window.setVerticalSyncEnabled(WINDOW_VERTICAL_SYNCHRONIZATION);
    else
        window.setFramerateLimit(WINDOW_FRAMERATE_LIMIT);

    window.setActive(true);




    glViewport(0, 0, window.getSize().x, window.getSize().y);
    glDisable(GL_DEPTH_TEST);
    glClearColor(OPENGL_CLEAR_COLOR_RED, OPENGL_CLEAR_COLOR_GREEN, OPENGL_CLEAR_COLOR_BLUE, OPENGL_CLEAR_COLOR_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);


    run = true;
    pause = false;
    demoWindow = false;
    updateEnabled = true;
    updateCount = 0;
    targetUpdateTime = TARGET_UPDATE_TIME;
}

Engine::~Engine()
{

}


void Engine::start()
{
    std::uint64_t target = 33333;
    std::uint64_t current = Clock::getMicro();
    std::uint64_t previous = 0;
    std::uint64_t delta = 0;
    std::uint64_t elapsed = 0;

    while(run == true)
    {
        previous = current;
        current = Clock::getMicro();
        delta = current - previous;

        process();

        elapsed += delta;
        while(elapsed > target)
        {
            update(target / 1000000.0f);

            elapsed -= target;
        }

        render();
        draw();
    }
}


/*
void Engine::start()
{
    freq = 3;
    cou = 0;
    timeHistoryValues = {{0}};
    timeHistoryOffset = 0;
    updateHistoryValues = {{0}};
    updateHistoryOffset = 0;

    
    std::uint64_t currentFrameTime = 0;
    std::uint64_t previousFrameTime = Clock::getMicro();
    differentFrameTime = 1; // ???
    std::uint64_t previousUpdateTime = 0;

    while(run == true)
    {
        currentFrameTime = Clock::getMicro();
        differentFrameTime = currentFrameTime - previousFrameTime;
        previousFrameTime = currentFrameTime;

        
        

        if(freq < cou)
        {
            frameTime = static_cast<float>(differentFrameTime) / 1000.0f;
            // TODO: Merge these two into a single array!
            timeHistoryValues[timeHistoryOffset] = frameTime; // TODO: Double instead of Float because of std::uint64_t?
            timeHistoryOffset = (timeHistoryOffset + 1) % timeHistoryValues.size();
            updateHistoryValues[updateHistoryOffset] = static_cast<float>(updateCount); // TODO: Double instead of Float because of std::uint64_t?
            updateHistoryOffset = (updateHistoryOffset + 1) % updateHistoryValues.size();
            cou = 0;
        }
        else
            cou++;


        prepare();
        process();

        updateCount = 0;
        if(updateEnabled == true)
        {
            previousUpdateTime = previousUpdateTime + differentFrameTime;
            while(previousUpdateTime > targetUpdateTime)
            {

                update(targetUpdateTime / 1000000.0f);
                //update(previousUpdateTime / 1000000.0f); // FIX: Should this be after updating previousUpdateTime?
                //std::cout << previousUpdateTime << std::endl; // FIX: Is this really accurate? Dont think so...

                previousUpdateTime = previousUpdateTime - targetUpdateTime;
                updateCount++;
            }
        }

        render();
        draw();
    }
}*/

void Engine::stop()
{
    run = false;
}

/*

    Resized, both gui and game wants this.
    Escape, only game wants this.
    

*/


void Engine::prepare()
{
    game.prepare();
}

void Engine::process()
{
    gui.begin(window);

    sf::Event event;
    while(window.pollEvent(event))
    {
        gui.process(event);
        game.process(event);

        switch(event.type)
        {

            case sf::Event::Resized:
            {
                // Both gui and game wants this.

                break;
            }
            case sf::Event::LostFocus:
            {
                // Pause game here.

                break;
            }
            case sf::Event::GainedFocus:
            {
                // Continue game here.

                break;
            }

            case sf::Event::Closed:
            {
                stop();

                break;
            }

            case sf::Event::KeyPressed:
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::F12:
                    {
                        game.debugWindow = !game.debugWindow;

                        break;
                    }

                    default:
                    {
                        break;
                    }
                }
            }

            default:
            {
                break;
            }
        }
    }
}

void Engine::update(double deltaTime)
{
    game.update(deltaTime);
}

void Engine::render()
{
    game.render();






        if(game.debugWindow)
        {
            ImGui::SetNextWindowSize(ImVec2(500, 500));
            //ImGui::SetNextWindowPosCenter();
            ImGui::SetNextWindowSize(ImVec2(1000, 500));
            ImGui::Begin("Debug", &game.debugWindow);
           



            if(ImGui::CollapsingHeader("Time"))
            {
                //ImGui::PlotLines("", timeHistoryValues.data(), timeHistoryValues.size(), timeHistoryOffset, "", 0.0f, FLT_MAX, ImVec2(900,0));
                //ImGui::PlotLines("", updateHistoryValues.data(), updateHistoryValues.size(), updateHistoryOffset, "", 0.0f, FLT_MAX, ImVec2(500,0));



                ImGui::Text("Updating");
                ImGui::SameLine();
                if(ImGui::Button("Pause") == true)
                    updateEnabled = !updateEnabled;
                ImGui::SameLine();
                if(ImGui::Button("Step") == true)
                    update(previousUpdateTime / 1000000.0f);

                //ImGui::Text("Frame time %.3f", frameTime);


                /*
                timing
                frequency
                delay
                interval

                Every:
                Took:
                */




                if (ImGui::TreeNode("Setting"))
                {

                    float a = static_cast<float>(targetUpdateTime);
                    if(ImGui::DragFloat("Update time", &a, 1000.0f, 16666.0f, 66666.0f))
                    {
                        targetUpdateTime = static_cast<std::uint64_t>(a);
                    }


                    ImGui::TreePop();
                }


            }



            if(ImGui::CollapsingHeader("Space"))
            {




                if (ImGui::TreeNode("Statistics"))
                {
                    ImGui::PushItemWidth(50);

                    //ImGui::Text("Step %u", game.world->space.getTimeStamp());

                    //ImGui::Text("Bodies: %u static, %u dynamic, %u sleep, %u total)", game.world->space.getStaticBodyCount(), game.world->space.getDynamicBodyCount(), game.world->space.getSleepingBodyCount(), game.world->space.getBodyCount());
                    //ImGui::Text("Shapes: %u static, %u dynamic, %u total.", game.world->space.getStaticShapeCount(), game.world->space.getDynamicShapeCount(), game.world->space.getShapeCount());

                    ImGui::PopItemWidth();
                    ImGui::TreePop();
                }


                //ImGui::Text("\tBroad phase: 0 entities");
                //ImGui::Text("\tNarrow phase: 0 entities");

                if (ImGui::TreeNode("Visualization"))
                {
                    ImGui::PushItemWidth(50);
                    ImGui::Checkbox("Visualization", &game.collisionVisualization);
                    ImGui::SameLine();                
                    ImGui::DragFloat("Line width", &game.collisionVisualizationHalfWidth, 0.5f, 0.5f, 2.0f);
                    ImGui::PopItemWidth();
                    ImGui::TreePop();
                }


                if (ImGui::TreeNode("Settings"))
                {
                    ImGui::PushItemWidth(50);
/*
                    int spaceIterations = game.world->space.getIterations();
                    if(ImGui::DragInt("Solve iterations", &spaceIterations, 1, 1, 100))
                        game.world->space.setIterations(spaceIterations);
                    ImGui::SameLine();
                    if(ImGui::Button("Default"))
                        game.world->space.setIterations(SPACE_DAMPING);
                    ImGui::SameLine();
                    Gui::Description("Few iterations makes simulation mushy and bouncy.\nMany iterations makes simulation accurate but CPU heavy.");

                    ImGui::Separator();
                    ImGui::Text("Forces");

                    float spaceGravity = static_cast<float>(game.world->space.getGravity().y);
                    if(ImGui::DragFloat("Gravity", &spaceGravity, 0.5f, 0.0f, 100.0f))
                        game.world->space.setGravity(cpv(0.0, static_cast<cpFloat>(spaceGravity)));
                    ImGui::SameLine();
                    if(ImGui::Button("Default"))
                        game.world->space.setGravity(SPACE_GRAVITY);

                    float spaceDamping = static_cast<float>(game.world->space.getDamping());
                    if(ImGui::DragFloat("Damping", &spaceDamping, 0.5f, 0.0f, 100.0f))
                        game.world->space.setDamping(static_cast<cpFloat>(spaceDamping));
                    ImGui::SameLine();
                    if(ImGui::Button("Default"))
                        game.world->space.setDamping(SPACE_DAMPING);
                    ImGui::SameLine();
                    Gui::Description("Amount of damping applied to all bodies.\nExample: A value of 0.9 means that each body will lose 10% of ets velocity per second.");

                    ImGui::Separator();
                    ImGui::Text("Collision");

                    float spaceCollisionSlop = static_cast<float>(game.world->space.getCollisionSlop());
                    if(ImGui::DragFloat("Collision slop", &spaceCollisionSlop, 0.05f, 0.0f, 10.0f))
                        game.world->space.setCollisionSlop(static_cast<cpFloat>(spaceCollisionSlop));
                    ImGui::SameLine();
                    if(ImGui::Button("Default"))
                        game.world->space.setCollisionSlop(SPACE_COLLISION_SLOP);
                    ImGui::SameLine();
                    Gui::Description("Amount of allowed overlap between shapes.\nSet as high as possible without visible overlapping for stability.");

                    float spaceCollisionBias = static_cast<float>(game.world->space.getCollisionBias());
                    if(ImGui::DragFloat("Collision bias", &spaceCollisionBias, 0.05f, 0.0f, 1.0f))
                        game.world->space.setCollisionBias(static_cast<cpFloat>(spaceCollisionBias));
                    ImGui::SameLine();
                    if(ImGui::Button("Default"))
                        game.world->space.setCollisionBias(SPACE_COLLISION_BIAS);
                    ImGui::SameLine();
                    Gui::Description("Remaining correction of overlapping shapes for a second.");
*/
                    /*float spaceCollisionPersistence = 0;
                    if(ImGui::DragFloat("Collision persistence", &spaceCollisionPersistence, 0.05f, 0.0f, 10.0f))
                        cpSpaceSetCollisionPersistence()
                    ImGui::SameLine();
                    if(ImGui::Button("Default"))
                        game.world->space.cpSpaceSetCollisionPersistence(SPACE_DAMPING);
                    ImGui::SameLine();
                    Gui::Description("Number of updates to save collision solutions.\nPrevents jittering contacts from getting worse.");*/
/*
                    ImGui::Separator();
                    ImGui::Text("Sleeping");

                    float spaceIdleSleepThreshold = static_cast<float>(game.world->space.getIdleSpeedThreshold());
                    if(ImGui::DragFloat("Idle speed threshold", &spaceIdleSleepThreshold, 0.01f, 0.0f, 10.0f))
                        game.world->space.setIdleSpeedThreshold(static_cast<cpFloat>(spaceIdleSleepThreshold));
                    ImGui::SameLine();
                    if(ImGui::Button("Default"))
                        game.world->space.setIdleSpeedThreshold(SPACE_IDLE_SPEED_THRESHOLD);
                    if(ImGui::Button("Estimate"))
                        game.world->space.setIdleSpeedThreshold(0.0);
                    ImGui::SameLine();
                    Gui::Description("Maximum speed for a body to be considered idle.\nEstimate is based on gravity.");
                    
                    float spaceSleepTimeThreshold = static_cast<float>(game.world->space.getSleepTimeThreshold());
                    if(ImGui::DragFloat("Sleep time threshold", &spaceSleepTimeThreshold, 0.01f, 0.0f, 10.0f))
                        game.world->space.setSleepTimeThreshold(static_cast<cpFloat>(spaceSleepTimeThreshold));
                    ImGui::SameLine();
                    if(ImGui::Button("Default"))
                        game.world->space.setSleepTimeThreshold(SPACE_SLEEP_TIME_THRESHOLD);
                    ImGui::SameLine();
                    if(ImGui::Button("Infitity"))
                        game.world->space.setSleepTimeThreshold(INFINITY);
                    ImGui::SameLine();
                    Gui::Description("Time a group of bodies must remain idle in order to fall asleep.\nInfitity disables the sleeping feature.");
*/
                    ImGui::PopItemWidth();
                    ImGui::TreePop();
                }
            }



            if(ImGui::CollapsingHeader("Stress"))
            {
                ImGui::Text("Spawn");
                /*ImGui::SameLine();
                if(ImGui::Button("100") == true)
                {
                    game.world->createArea(100);
                }
                ImGui::SameLine();
                if(ImGui::Button("700") == true)
                {
                    game.world->createArea(700);
                }
                ImGui::SameLine();
                if(ImGui::Button("900") == true)
                {
                    game.world->createArea(900);
                }
                ImGui::SameLine();
                if(ImGui::Button("1100") == true)
                {
                    game.world->createArea(1100);
                }*/
            }



            if(ImGui::CollapsingHeader("Test"))
            {

                /*
                    Select, Move, Create, Destroy, Edit
                */
                
               /* ImGui::Text("Current phase %f", cpGearJointGetPhase(game.world->bvGear));

                float force = static_cast<float>(cpConstraintGetMaxForce(game.world->bvGear));
                if(ImGui::DragFloat("Maximum force", &force, 100000.0f, 1000000.0f, 100000000.0f))
                {
                    cpConstraintSetMaxForce(game.world->bvGear, static_cast<cpFloat>(force));
                }
                ImGui::SameLine();
                if(ImGui::Button("INFINITY") == true)
                {
                    cpConstraintSetMaxForce(game.world->bvGear, INFINITY);
                }

                ImGui::Text("Acceleration");
                ImGui::SameLine();
                if(ImGui::Button("10%") == true)
                    cpConstraintSetErrorBias(game.world->bvGear, cpfpow(1.0f - 0.1, 60.0f));
                ImGui::SameLine();
                if(ImGui::Button("30%") == true)
                    cpConstraintSetErrorBias(game.world->bvGear, cpfpow(1.0f - 0.3, 60.0f));
                ImGui::SameLine();
                if(ImGui::Button("50%") == true)
                    cpConstraintSetErrorBias(game.world->bvGear, cpfpow(1.0f - 0.5, 60.0f));
                ImGui::SameLine();
                if(ImGui::Button("70%") == true)
                    cpConstraintSetErrorBias(game.world->bvGear, cpfpow(1.0f - 0.7, 60.0f));
                ImGui::SameLine();
                if(ImGui::Button("90%") == true)
                    cpConstraintSetErrorBias(game.world->bvGear, cpfpow(1.0f - 0.9, 60.0f));

                float correction = static_cast<float>(cpConstraintGetMaxBias(game.world->bvGear));
                if(ImGui::DragFloat("Maximum speed", &correction, 0.1f, 0.0f, 10.0f))
                {
                    cpConstraintSetMaxBias(game.world->bvGear, static_cast<cpFloat>(correction));
                }
                ImGui::SameLine();
                if(ImGui::Button("INFINITY") == true)
                {
                    cpConstraintSetMaxBias(game.world->bvGear, INFINITY);
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
            ImGui::ShowDemoWindow();
        }
}

void Engine::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    game.draw();

    gui.render();
    gui.draw(); 

    window.display();
}