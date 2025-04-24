
#include <iostream>
#include "engine.hpp"
#include "exception.hpp"
#include "glpp/exception.hpp"



int main(int argc, char* argv[])
{
    std::cout << " START" << std::endl;

    try
    {
        Engine engine;
        engine.start();
    }
    catch(Exception& exception)
    {
        std::cout << " FATAL Minenture: " << exception.getReason() << std::endl;
    }
    catch(gl::Exception& exception)
    {
        std::cout << " FATAL OpenGL: " << exception.getReason() << std::endl;
    }

    std::cout << " STOP" << std::endl;
    
    return(0);
}
