
#ifndef __CLOCK_HPP_INCLUDED__
#define __CLOCK_HPP_INCLUDED__

#include <chrono>
#include <cstdint>



class Clock
{
public:
    static std::uint64_t getMicro();

private:
    Clock();
    ~Clock();
};

#endif
