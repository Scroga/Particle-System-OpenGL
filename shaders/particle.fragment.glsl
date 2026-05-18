#version 430 core

in GS_OUT {
    vec3 Normal;
    vec2 TexCoord;
    vec4 Color;
} fs_in;

out vec4 FragColor;

void main()
{
    FragColor = fs_in.Color;
}