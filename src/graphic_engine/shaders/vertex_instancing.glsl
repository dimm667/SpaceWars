#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aVoxelPosition;
layout (location = 3) in vec3 aColor;

//uniform mat4 model;
layout (std140) uniform TViewProjMatrices
{
    mat4 model;
    mat4 proj_view;
};

out vec3 color;
out vec3 normal;
out vec3 fragmentPosition;
out vec4 fragmentPositionDirectionalLightSpace;

void main()
{
    vec4 position = vec4(aPos + aVoxelPosition, 1.0);

    gl_Position = proj_view * model * position;
    fragmentPosition = vec3(position);
    normal = mat3(transpose(inverse(model))) * aNormal;
    color = aColor;
}
