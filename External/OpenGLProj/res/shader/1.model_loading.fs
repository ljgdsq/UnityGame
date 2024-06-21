#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;


void main()
{
    // 从法线贴图中获取法线，并将其范围从 [0,1] 转换到 [-1,1]
    vec3 normal = texture(texture_normal1, TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0); // Transforming to [-1, 1]

    // 计算光照
    vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    vec3 lightColor = vec3(1.0);

    // Ambient
    vec3 ambient = 0.9 * color;

    // Diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor * color;

    // Specular
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = lightColor * spec*texture(texture_specular1, TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}