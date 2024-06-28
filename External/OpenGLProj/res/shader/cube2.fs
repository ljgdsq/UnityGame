#version 330 core
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D image;
uniform bool hasTex;
out vec4 FragColor;

void main()
{

    vec3 color=vec3(0.2,0.6,0.6);
    if(hasTex){
        FragColor = vec4(texture(image,TexCoords).rgb*color,1.0);
    }else{
        FragColor = vec4(color, 1.0);
    }
}