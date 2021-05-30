#include "voxel.h"
#include <ctime>

VoxelArray::VoxelArray(unsigned int arraySize) :
    size(arraySize)
{
    const float halfDimension = dimension / 2.0f;

    //                  |position                                               |normal                 |texture coords
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f,  0.0f, -1.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    {0.0f,  0.0f, -1.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    {0.0f,  0.0f, -1.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    {0.0f,  0.0f, -1.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f,  0.0f, -1.0f}});
    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   {0.0f,  0.0f, -1.0f}});

    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   {0.0f,  0.0f,  1.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    {0.0f,  0.0f,  1.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  0.0f,  1.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  0.0f,  1.0f}});
    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   {0.0f,  0.0f,  1.0f}});
    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   {0.0f,  0.0f,  1.0f}});

    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   {-1.0f,  0.0f,  0.0f}});

    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    {1.0f,  0.0f,  0.0f}});

    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f, -1.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    {0.0f, -1.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    {0.0f, -1.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    {0.0f, -1.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   {0.0f, -1.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f, -1.0f,  0.0f}});

    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   {0.0f,  1.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  1.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    {0.0f,  1.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  1.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   {0.0f,  1.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   {0.0f,  1.0f,  0.0f}});

    glGenBuffers(1, &cubeVBO);
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &instanceVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, cube.size() * sizeof(voxelVertex), &cube[0], GL_STATIC_DRAW);



    glBindVertexArray(cubeVAO);
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(voxelVertex), (void*)offsetof(voxelVertex, position));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(voxelVertex), (void*)offsetof(voxelVertex, normal));


    // voxel position
    // initialize voxel array
    for(unsigned int idx = 0; idx < size; ++idx)
        voxel.push_back({glm::vec3(0.0f),
                         glm::vec3(0.0f)});

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Voxel) * size, &voxel[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, position));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, color));
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
}

void VoxelArray::set(unsigned int index, Voxel value)
{
    voxel[index] = value;
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, (index) * sizeof(Voxel), sizeof(Voxel), &value);
}

void VoxelArray::draw() const
{
    glBindVertexArray(cubeVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, voxel.size());
    glBindVertexArray(0);
}
