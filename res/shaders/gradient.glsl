#shader vertex
#version 330

layout(location=0) in vec4 vp;
layout(location=1) in vec4 a_Color;

out vec4 v_Color;

void main() {
    gl_Position = vp;
    v_Color = a_Color;
}


#shader fragment
#version 330

in vec4 v_Color;

out vec4 frag_color;

void main() {
    frag_color = v_Color;
}
