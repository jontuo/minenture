
#ifndef __ENGINE_HPP_INCLUDED__
#define __ENGINE_HPP_INCLUDED__

#include "game.hpp"
#include <SFML/Window.hpp>
#include "glpp/glader.hpp"
#include "gui.hpp"
#include <cstdint>
#include "setting.hpp"

class Engine
{
public:
    Engine();
    ~Engine();

    void start();
    void stop();

private:


    sf::Window window;
    gl::Glader glader;
    Game game;
    Gui gui;

    bool run;
    bool pause;
    bool demoWindow;

    std::uint64_t targetUpdateTime;
    //std::uint64_t differentFrameTime;
    std::uint64_t previousUpdateTime;

    //float frameTime;
    //float updateTime;


    bool updateEnabled;
    unsigned int updateCount;

    //int freq;
    //int cou;
    //std::array<float, HISTORY_WIDTH> timeHistoryValues;
    //unsigned int timeHistoryOffset;
    //std::array<float, HISTORY_WIDTH> updateHistoryValues;
    //unsigned int updateHistoryOffset;




    void prepare();
    void process();
    void update(double deltaTime);
    void render();
    void draw();

};

#endif

