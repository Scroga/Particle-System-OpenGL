#version 430 core

layout(location = 0) in vec3 in_pointPos;

layout(location = 1) in vec3 in_instancePos;
layout(location = 2) in vec4 in_instanceColor;
layout(location = 3) in float in_instanceSize;
layout(location = 4) in int in_instanceTexIndex;

out VS_OUT {
    vec3 position;
    vec4 color;
    float size;
    flat int texIndex;
} vs_out;

void main()
{
    vs_out.position = in_pointPos + in_instancePos;
    vs_out.color = in_instanceColor;
    vs_out.size = in_instanceSize;
    vs_out.texIndex = in_instanceTexIndex;

    gl_Position = vec4(vs_out.position, 1.0);
}