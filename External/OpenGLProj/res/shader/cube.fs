#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D image;
uniform vec3 color;
uniform bool hasTex;
out vec4 FragColor;

void main()
{
    FragColor = vec4(color, 1.0);
}