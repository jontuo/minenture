
#ifndef __VECTOR_HPP_INCLUDED__
#define __VECTOR_HPP_INCLUDED__

#include "chipmunk/cpVect.h"
#include <glm/glm.hpp>



glm::vec2 toGlm(const cpVect& cp);

cpVect toCp(const glm::vec2& glm);


#endif
