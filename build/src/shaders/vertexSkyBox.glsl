#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform TViewProjMatrices
{
    mat4 view;
    mat4 projection;
    mat4 proj_view;
};

out vec3 texCoords;

void main()
{
    vec4 pos = proj_view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
    texCoords = aPos;
}
