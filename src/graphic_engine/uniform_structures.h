#ifndef UNIFORM_STRUCTURES
#define UNIFORM_STRUCTURES
#include <glm/glm.hpp>

struct alignas(4*16) TViewProjMatrices
{
    glm::mat4 model;
    glm::mat4 proj_view;
};

#endif // UNIFORM_STRUCTURES
