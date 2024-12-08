#shader vertex
#version 330 core

layout(location = 0) in vec3 a_position;

uniform mat4 u_project_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;

out vec3 v_tex_coords;

void main() {
    // Remove translation from the view matrix by setting the last column to zero
    mat4 view_no_translation = mat4(mat3(u_view_matrix));

    // Compute the final position
    vec4 position = u_model_matrix * vec4(a_position, 1.0);
    gl_Position = u_project_matrix * view_no_translation * position;

    // Pass through the vertex position as texture coordinates
    v_tex_coords = a_position;
}
