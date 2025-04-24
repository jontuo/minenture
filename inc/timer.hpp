
#ifndef __TIMER_HPP_INCLUDED__
#define __TIMER_HPP_INCLUDED__

#include <cstdint>

class Timer
{
public:

    std::uint64_t realTime;
    std::uint64_t virtualTime;

    Timer();
    ~Timer();




private:

};

/*
http://lspiroengine.com/?p=378
https://www.gamedev.net/forums/topic/630693-pause-game-how-should-it-be-treated/
https://gamedev.stackexchange.com/questions/150922/game-time-and-pausing






*/

#endif
