#shader vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_tex_coords;

uniform mat4 u_project_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;

out vec3 v_normal;
out vec3 v_frag_pos;
out vec3 v_color;
out vec2 v_tex_coords;

void main() {
    v_frag_pos = vec3(u_model_matrix * vec4(a_position, 1.0));
    v_normal = mat3(transpose(inverse(u_model_matrix))) * a_normal;
    v_tex_coords = a_tex_coords;

    gl_Position = u_project_matrix * u_view_matrix * vec4(v_frag_pos, 1.0);
}
