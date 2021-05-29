#ifndef VOXEL_H
#define VOXEL_H

#include <array>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct voxelVertex {
    glm::vec3 position;
    glm::vec3 normal;
};

struct Voxel
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 dimension;
};

class VoxelArray
{
public:
    VoxelArray(unsigned int size);
    VoxelArray(const VoxelArray& other) = delete;
    VoxelArray(const VoxelArray&& other) = delete;

    void set(unsigned int index, Voxel value);

    void draw() const;

private:
    static constexpr float dimension = 1.0f;

    GLuint cubeVBO, cubeVAO, instanceVBO;

    std::vector<voxelVertex> cube;

    std::vector<Voxel> voxel;
    unsigned int size;
};

#endif // VOXEL_H
