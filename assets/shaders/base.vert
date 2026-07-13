#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform MatricesBinding
{
    mat4 u_ProjectionMatrix;
    mat4 u_ViewMatrix;
};

uniform mat4 u_ModelMatrix;
uniform mat3 u_InvTranModelMatrix;

out VS_OUT
{
    vec3 Normal;
    vec2 TexCoord;
    vec3 FragPosition;
}
vs_out;

void main()
{
    vec4 position = vec4(a_Position, 1.0);
    gl_Position   = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * position;

    vs_out.Normal       = u_InvTranModelMatrix * a_Normal;
    vs_out.TexCoord     = a_TexCoord;
    vs_out.FragPosition = vec3(u_ModelMatrix * position);
}
