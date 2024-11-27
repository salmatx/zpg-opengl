#shader fragment
#version 330 core

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

uniform vec3 u_light_position;
uniform vec3 u_light_color;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // combine results
    vec3 ambient  = light.ambient  * 0.1;
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * spec;
    return (ambient + diffuse + specular);
}

in vec3 v_normal;
in vec3 v_frag_pos;

uniform vec3 u_view_position;

out vec4 frag_color;

void main() {
    vec3 view_dir = normalize(u_view_position - v_frag_pos);
    vec3 result = CalcDirLight(dirLight, normalize(v_normal), view_dir);
    frag_color = vec4(result, 1.0);
}
