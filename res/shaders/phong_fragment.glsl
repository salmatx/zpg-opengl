#shader fragment
#version 330 core

in vec3 v_normal;
in vec3 v_frag_pos;

uniform vec3 u_light_position;
uniform vec3 u_light_color;
uniform vec3 u_view_position;

out vec4 frag_color;

void main() {
    vec3 ambient = 0.1 * u_light_color;

    vec3 norm = normalize(v_normal);
    vec3 light_dir = normalize(u_light_position - v_frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * u_light_color;

    vec3 view_dir = normalize(u_view_position - v_frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = 0.5 * spec * u_light_color;

    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(result, 1.0);
}
