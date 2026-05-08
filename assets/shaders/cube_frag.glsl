#version 330 core

in vec2 v_tex_coord;

out vec4 FragColor;

uniform sampler2D tex;
uniform vec3 light_color;

const float ambient_strength = 1.0f;

void main()
{
  vec3 ambient = ambient_strength * light_color;
  FragColor = vec4(ambient, 1.0f) * texture(tex, v_tex_coord);
}
