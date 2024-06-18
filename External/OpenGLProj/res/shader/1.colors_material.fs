#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;



struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

void main()
{

    float ambientStrength = 0.1;
    vec3 ambient =lightColor *material.ambient;


    vec3 norm=normalize(Normal);
    vec3 lightDir=normalize(lightPos-FragPos);
    float diff=max(dot(lightDir,norm),0);
    vec3 diffuse=diff*lightColor *material.diffuse;

    float specularStrength=1;
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0),material.shininess);
    vec3 specular = specularStrength * spec * lightColor *material.specular;

    vec3 result = (ambient *light.ambient + diffuse*light.diffuse +specular*light.specular) * objectColor;
    FragColor = vec4(result, 1.0);
}