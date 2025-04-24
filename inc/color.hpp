
#ifndef __COLOR_HPP_INCLUDED__
#define __COLOR_HPP_INCLUDED__



struct Color
{
    float r;
    float g;
    float b;
    float a;

    Color(): r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
    Color(float r, float g, float b): r(r), g(g), b(b), a(1.0f) {}
    Color(float r, float g, float b, float a): r(r), g(g), b(b), a(a) {}  
};

#endif
