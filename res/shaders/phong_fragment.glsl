#shader fragment
#version 330 core

#define MAX_LIGHTS 5

struct LightSource_t {
    int type; // 0 - Point, 1 - Directional, 2 - Spotlight
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float cutoff;
    float outer_cutoff;
};

in vec3 v_normal;
in vec3 v_frag_pos;

uniform LightSource_t u_lights[MAX_LIGHTS];
uniform vec3 u_view_position;
uniform int u_light_count;

out vec4 frag_color;

void main() {
    vec3 norm = normalize(v_normal);
    vec3 view_dir = normalize(u_view_position - v_frag_pos);

    // Initial ambient color
    vec3 ambient = vec3(0.0);
    for (int i = 0; i < u_light_count; ++i) {
        ambient += 0.1 * u_lights[i].color * u_lights[i].intensity;
    }

    vec3 result = ambient;

    // Iterate over all lights for diffuse and specular contributions
    for (int i = 0; i < u_light_count; ++i) {
        vec3 light_color = u_lights[i].color * u_lights[i].intensity;

        if (u_lights[i].type == 0) {
            // Point Light
            vec3 light_dir = normalize(u_lights[i].position - v_frag_pos);
            float diff = max(dot(norm, light_dir), 0.0);
            vec3 diffuse = diff * light_color;

            vec3 reflect_dir = reflect(-light_dir, norm);
            float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
            vec3 specular = 0.5 * spec * light_color;

            result += diffuse + specular;

        } else if (u_lights[i].type == 1) {
            // Directional Light
            vec3 light_dir = normalize(-u_lights[i].direction);
            float diff = max(dot(norm, light_dir), 0.0);
            vec3 diffuse = diff * light_color;

            vec3 reflect_dir = reflect(-light_dir, norm);
            float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
            vec3 specular = 0.5 * spec * light_color;

            result += diffuse + specular;

        } else if (u_lights[i].type == 2) {
            // Spotlight
            vec3 light_dir = normalize(u_lights[i].position - v_frag_pos);
            float theta = dot(light_dir, normalize(-u_lights[i].direction));

            if (theta > u_lights[i].outer_cutoff) {
                float diff = max(dot(norm, light_dir), 0.0);
                vec3 diffuse = diff * light_color;

                vec3 reflect_dir = reflect(-light_dir, norm);
                float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
                vec3 specular = 0.5 * spec * light_color;

                // Apply spotlight intensity based on the angle
                float epsilon = u_lights[i].cutoff - u_lights[i].outer_cutoff;
                float intensity = clamp((theta - u_lights[i].outer_cutoff) / epsilon, 0.0, 1.0);

                diffuse *= intensity;
                specular *= intensity;

                result += diffuse + specular;
            }
        }
    }

    frag_color = vec4(result, 1.0);
}
