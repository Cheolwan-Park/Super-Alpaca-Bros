#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
out vec2 texcoord;

uniform mat4 vp;

void main() 
{
    gl_Position = vp * vec4(pos.x, pos.y, 0.0, 1.0);
    texcoord = uv;
}