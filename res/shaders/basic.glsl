#shader vertex
#version 330

layout(location=0) in vec4 vp;
void main () {
     gl_Position = vp;
}


#shader fragment
#version 330

out vec4 frag_color;

uniform vec4 u_Color;

void main () {
     frag_color = u_Color;
}