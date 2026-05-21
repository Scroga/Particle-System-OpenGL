#version 430 core

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;
uniform mat3 u_normalMat;

in vec3 in_vert;
in vec3 in_normal;
in vec2 in_texCoord;

out VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} vs_out;

void main()
{
    gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(in_vert, 1);
    vs_out.position = vec3(u_modelMat * vec4(in_vert, 1.0));
    vs_out.normal = normalize(u_normalMat * in_normal);
    vs_out.texCoord = in_texCoord;
}