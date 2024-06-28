#version 330 core
layout (location = 0) in vec3 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec2 texcoord; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    TexCoords = texcoord;
    gl_Position = projection *  vec4(vertex.xy, 0.0, 1.0);
}