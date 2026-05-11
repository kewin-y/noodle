#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

out vec3 v_normal;
out vec2 v_tex_coord;
out vec3 v_frag_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 model_inv_T;
uniform vec3 view_pos;

void main()
{
    gl_Position = projection * view * model * vec4(a_pos, 1.0);
    // TODO: figure out the math behind this
    v_normal = model_inv_T * a_normal;
    v_tex_coord = a_tex_coord;
    v_frag_pos = vec3(model * vec4(a_pos, 1.0));
}
