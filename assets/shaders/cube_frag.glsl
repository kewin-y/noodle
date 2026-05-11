#version 330 core

in vec2 v_tex_coord;
in vec3 v_normal;
in vec3 v_frag_pos;

out vec4 FragColor;

uniform sampler2D tex;
uniform vec3 light_color;
uniform vec3 light_pos;
// view_pos is already in world pos when passed from the CPU
uniform vec3 view_pos;

const float ambient_strength = 0.2f;
const float specular_strength = 0.5f;
const int shininess = 32;

void main()
{
  // Calculate diffuse lighting
  vec3 normal = normalize(v_normal);
  vec3 light_dir = normalize(light_pos - v_frag_pos);
  float diff = max(dot(normal, light_dir), 0);

  // Calculate specular lighting
  vec3 light_reflected_dir = reflect(-light_dir, normal);
  vec3 view_dir = normalize(view_pos - v_frag_pos);
  float spec = specular_strength * pow(max(dot(light_reflected_dir, view_dir), 0), shininess);

  // Combine diffuse lighting and ambient lighting
  vec3 ambient_diff_spec = (ambient_strength + diff + spec) * light_color;

  // Calculate final colour
  FragColor = vec4(ambient_diff_spec, 1.0f) * texture(tex, v_tex_coord);
}
