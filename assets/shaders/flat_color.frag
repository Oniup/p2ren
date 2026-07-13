#version 460 core

out vec4 FragColor;
in VS_OUT
{
    vec3 Normal;
    vec2 TexCoord;
    vec3 FragPosition;
}
fs_in;

void main()
{
    FragColor = vec4(0.7, 0.3, 0.2, 1.0);
}
