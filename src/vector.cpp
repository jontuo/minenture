
#include "vector.hpp"


glm::vec2 toGlm(const cpVect& cp)
{
    return(glm::vec2(cp.x, cp.y));
}

cpVect toCp(const glm::vec2& glm)
{
    return(cpv(glm.x, glm.y));
}
