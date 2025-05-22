#version 330 core

in vec2 v_tex_coord;

out vec4 FragColor;

uniform sampler2D v_texture;

void main()
{
    FragColor = texture(v_texture, v_tex_coord);
}
