
#include "clock.hpp"



Clock::Clock() {}
Clock::~Clock() {}

std::uint64_t Clock::getMicro()
{
    return(std::chrono::duration_cast<std::chrono::duration<std::uint64_t, std::ratio<1, 1000000>>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
}
