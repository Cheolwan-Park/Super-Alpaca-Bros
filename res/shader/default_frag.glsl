#version 410 core

in vec2 texcoord;
out vec4 out_color;

uniform sampler2D tex;

void main() 
{
    vec4 color = texture(tex, texcoord);
    out_color = color;
    // out_color = vec4(1.0, 0.0, 0.0, 1.0);
}
