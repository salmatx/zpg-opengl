#shader fragment
#version 330 core

uniform samplerCube u_texture;

in vec3 v_tex_coords;

out vec4 frag_color;

void main() {
    frag_color = texture(u_texture, normalize(v_tex_coords));
}
