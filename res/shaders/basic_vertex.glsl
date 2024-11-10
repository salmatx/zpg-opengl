#shader vertex
#version 330

uniform mat4 u_model_matrix;
uniform mat4 u_project_matrix;
uniform mat4 u_view_matrix;

out vec3 v_color;

layout(location=0) in vec3 vp;
layout(location=1) in vec3 vn;
void main () {
    v_color = vn;
    gl_Position = u_project_matrix * u_view_matrix * u_model_matrix * vec4(vp, 1.0);
}