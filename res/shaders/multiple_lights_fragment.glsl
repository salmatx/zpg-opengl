#shader fragment
#version 330 core

#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 100
#define MAX_FLASH_LIGHTS 10

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct FlashLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform int u_dirLights_count;
uniform DirLight u_dirLights[MAX_DIR_LIGHTS];
uniform int u_pointLights_count;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];
uniform int u_flashLights_count;
uniform FlashLight u_flashLights[MAX_FLASH_LIGHTS];

uniform sampler2D u_texture;
uniform vec3 u_view_position;

in vec3 v_normal;
in vec3 v_frag_pos;
in vec2 v_tex_coords;

out vec4 frag_color;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // combine results
    vec3 ambient = light.ambient  * 0.3;
    vec3 diffuse = light.diffuse * diff * texture(u_texture, v_tex_coords).rgb;
    vec3 specular = light.specular * spec;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient  * 0.1;
    vec3 diffuse = light.diffuse * diff * texture(u_texture, v_tex_coords).rgb;
    vec3 specular = light.specular * spec;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcFlashLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient  * 0.1;
    vec3 diffuse = light.diffuse * diff * texture(u_texture, v_tex_coords).rgb;
    vec3 specular = light.specular * spec;
    ambient *= attenuation;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main() {
    vec3 view_dir = normalize(u_view_position - v_frag_pos);
    vec3 normal = normalize(v_normal);
    vec3 result = vec3(0.0);

    for (int i = 0; i < u_dirLights_count; ++i) {
        result += CalcDirLight(u_dirLights[i], normal, view_dir);
    }
    for (int i = 0; i < u_pointLights_count; ++i) {
        result += CalcPointLight(u_pointLights[i], normal, v_frag_pos, view_dir);
    }
    for (int i = 0; i < u_flashLights_count; ++i) {
        result += CalcFlashLight(u_flashLights[i], normal, v_frag_pos, view_dir);
    }

    vec4 texColor = texture(u_texture, v_tex_coords);

    frag_color = vec4(result, 1.0) * texColor;
}
