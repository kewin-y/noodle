#version 330 core

in vec4 v_color;
in vec2 v_tex_coord;

out vec4 FragColor;

uniform sampler2D v_texture;

void main()
{
    FragColor = texture(v_texture, v_tex_coord);
}
