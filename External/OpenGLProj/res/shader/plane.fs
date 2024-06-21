#version 330 core
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D image;
uniform vec3 color;
uniform bool hasTex;
out vec4 FragColor;

void main()
{
    if(hasTex){
        vec4 c=texture(image,TexCoords).rgba;
        if(c.a<0.1)
        discard;
        FragColor =c;
    }else{
        FragColor = vec4(color, 1.0);
    }
}