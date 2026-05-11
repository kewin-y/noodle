#version 330 core

in vec2 v_tex_coord;
in vec3 v_normal;
in vec3 v_frag_pos;

out vec4 FragColor;

uniform sampler2D tex;
uniform vec3 light_color;
uniform vec3 light_pos;

const float ambient_strength = 0.2f;

void main()
{
  // Calculate diffuse lighting
  vec3 normal = normalize(v_normal);
  vec3 light_direction = normalize(light_pos - v_frag_pos);
  float diff = max(dot(normal, light_direction), 0);

  // Combine diffuse lighting and ambient lighting
  vec3 diff_ambient = (diff + ambient_strength) * light_color;

  // Calculate final colour
  FragColor = vec4(diff_ambient, 1.0f) * texture(tex, v_tex_coord);
}
