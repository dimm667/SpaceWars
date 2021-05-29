#version 330 core
in vec2 tex_coord;

out vec4 color;

uniform sampler2D my_texture;

void main()
{
   color = texture(my_texture, tex_coord);
}
