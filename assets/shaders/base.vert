#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in vec2 a_TexCoord;

// layout(std140, binding = 0) uniform UBO_Matrices
// {
//     mat4 u_Projection;
//     mat4 u_View;
// };

uniform mat4 u_Model;
// uniform mat3 u_InverseTransposeModel;

out VS_OUT
{
    vec3 Normal;
    vec3 Color;
    vec2 TexCoord;
    vec3 FragPosition;
}
vs_out;

// uniform vec2 u_Material_TilingFactor;

void main()
{
    vec4 position = vec4(a_Position, 1.0);
    gl_Position   = u_Model * position;
    // gl_Position   = u_Projection * u_View * u_Model * position;

    vs_out.Normal = a_Normal;
    // vs_out.Normal       = u_InverseTransposeModel * a_Normal;
    vs_out.Color        = a_Color;
    vs_out.TexCoord     = a_TexCoord;
    vs_out.FragPosition = vec3(u_Model * position);
}
