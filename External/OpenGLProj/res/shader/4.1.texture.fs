#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
  vec4 bg=texture(texture2, TexCoord);
	FragColor = texture(texture1, TexCoord)*(1-bg.a) +bg;

// 		FragColor = texture(texture1, TexCoord);
}