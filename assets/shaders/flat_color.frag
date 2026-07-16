#version 460 core

out vec4 FragColor;

in VS_OUT
{
    vec3 Normal;
    vec3 Color;
    vec2 TexCoord;
    vec3 FragPosition;
}
fs_in;

void main()
{
    FragColor = vec4(fs_in.Color, 1.0);
}
