#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;

in VS_OUT {
    vec3 position;
    vec4 color;
    float size;
    flat int texIndex;
} gs_in[];

out GS_OUT {
    vec3 worldPos;
    vec3 normal;
    vec2 texCoord;
    vec4 color;
    flat int texIndex;
} gs_out;

void emitParticleVertex(vec3 pos, vec2 texCoord, vec3 normal)
{
    gl_Position = u_projMat * u_viewMat * vec4(pos, 1.0);

    gs_out.worldPos = vec3(u_modelMat * vec4(pos, 1.0));
    gs_out.texCoord = texCoord;
    gs_out.normal = normal;
    gs_out.color = gs_in[0].color;
    gs_out.texIndex = gs_in[0].texIndex;

    EmitVertex();
}

void main()
{
    vec3 center = gs_in[0].position;
    float halfSize = gs_in[0].size * 0.5;

    vec3 right = vec3(u_viewMat[0][0], u_viewMat[1][0], u_viewMat[2][0]);
    vec3 up    = vec3(u_viewMat[0][1], u_viewMat[1][1], u_viewMat[2][1]);

    vec3 normal = normalize(cross(right, up));

    emitParticleVertex(
        center - right * halfSize - up * halfSize,
        vec2(0.0, 0.0),
        normal
    );

    emitParticleVertex(
        center + right * halfSize - up * halfSize,
        vec2(1.0, 0.0),
        normal
    );

    emitParticleVertex(
        center - right * halfSize + up * halfSize,
        vec2(0.0, 1.0),
        normal
    );

    emitParticleVertex(
        center + right * halfSize + up * halfSize,
        vec2(1.0, 1.0),
        normal
    );

    EndPrimitive();
}