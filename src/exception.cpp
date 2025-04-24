
#include "exception.hpp"



Exception::Exception(std::string reason): reason(reason)
{
}

Exception::~Exception()
{

}

const char* Exception::getReason()
{
    return reason.c_str();
}
