#version 460 core

out vec4 FragColor;

struct MaterialColor
{
    vec3      Color;
    sampler2D Texture;
};

struct Material
{
    MaterialColor Diffuse;
    MaterialColor Specular;
    MaterialColor Emission;
    float         Shininess;
};

in VS_OUT
{
    vec3 Normal;
    vec3 Color;
    vec2 TexCoord;
    vec3 FragPosition;
}
fs_in;

uniform Material u_Material;

void main()
{
    vec4 diffuse_sample = texture(u_Material.Diffuse.Texture, fs_in.TexCoord);

    FragColor = vec4(diffuse_sample.rgb, diffuse_sample.a);
}
