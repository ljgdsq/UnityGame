#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

// 纹理
uniform sampler2D texture_0; // diffuse
uniform sampler2D texture_1; // normal
uniform sampler2D texture_2; // specular
uniform sampler2D texture_3; // roughness

// 最大光源数量
#define MAX_LIGHTS 8

// 光源类型
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

// 光源数据结构
struct Light {
    int type;              // 光源类型
    vec3 position;         // 位置 (点光源/聚光灯)
    vec3 direction;        // 方向 (方向光/聚光灯)
    vec3 color;            // 颜色
    float intensity;       // 强度
    float range;           // 范围 (点光源/聚光灯)
    float constant;        // 常数衰减
    float linear;          // 线性衰减
    float quadratic;       // 二次衰减
    float spotInnerAngle;  // 聚光灯内角 (cos值)
    float spotOuterAngle;  // 聚光灯外角 (cos值)
};

// Uniform变量
uniform int u_lightCount;
uniform Light u_lights[MAX_LIGHTS];
uniform vec3 u_ambientLight;
uniform vec3 u_viewPosition;

// 材质属性
uniform vec3 u_diffuseColor;
uniform vec3 u_specularColor;
uniform float u_shininess;

// 计算方向光
vec3 CalculateDirectionalLight(Light light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor) {
    vec3 lightDir = normalize(-light.direction);

    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * light.intensity * diffuseColor;

    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_shininess);
    vec3 specular = spec * light.color * light.intensity * specularColor;

    return diffuse + specular;
}

// 计算点光源
vec3 CalculatePointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor) {
    vec3 lightDir = normalize(light.position - fragPos);

    // 衰减计算
    float distance = length(light.position - fragPos);
    if (distance > light.range) {
        return vec3(0.0);
    }

    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * light.intensity * diffuseColor;

    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_shininess);
    vec3 specular = spec * light.color * light.intensity * specularColor;

    return (diffuse + specular) * attenuation;
}

// 计算聚光灯
vec3 CalculateSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor) {
    vec3 lightDir = normalize(light.position - fragPos);

    // 检查是否在聚光灯锥体内
    float theta = dot(lightDir, normalize(-light.direction));

    if (theta < light.spotOuterAngle) {
        return vec3(0.0); // 超出聚光灯范围
    }

    // 衰减计算
    float distance = length(light.position - fragPos);
    if (distance > light.range) {
        return vec3(0.0);
    }

    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // 聚光灯边缘软化
    float epsilon = light.spotInnerAngle - light.spotOuterAngle;
    float intensity = clamp((theta - light.spotOuterAngle) / epsilon, 0.0, 1.0);

    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * light.intensity * diffuseColor;

    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_shininess);
    vec3 specular = spec * light.color * light.intensity * specularColor;

    return (diffuse + specular) * attenuation * intensity;
}

// 计算单个光源的贡献
vec3 CalculateLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor) {
    if (light.type == DIRECTIONAL_LIGHT) {
        return CalculateDirectionalLight(light, normal, viewDir, diffuseColor, specularColor);
    } else if (light.type == POINT_LIGHT) {
        return CalculatePointLight(light, normal, fragPos, viewDir, diffuseColor, specularColor);
    } else if (light.type == SPOT_LIGHT) {
        return CalculateSpotLight(light, normal, fragPos, viewDir, diffuseColor, specularColor);
    }
    return vec3(0.0);
}

void main() {
    // 获取纹理颜色
    vec3 albedo = texture(texture_0, TexCoord).rgb; // diffuse
    vec3 normalMap = texture(texture_1, TexCoord).rgb; // normal
    vec3 specularMap = texture(texture_2, TexCoord).rgb; // specular
    float roughness = texture(texture_3, TexCoord).r; // roughness

    // 法线贴图处理（TBN略，假定normalMap已是世界空间）
    vec3 normal = normalize(normalMap * 2.0 - 1.0); // [-1,1]
    if (length(normal) < 0.01) normal = normalize(Normal); // 防止无normal贴图时异常

    vec3 viewDir = normalize(u_viewPosition - FragPos);

    // 环境光
    vec3 diffuseColor = albedo * u_diffuseColor;
    vec3 result = u_ambientLight * diffuseColor;

    // 镜面高光参数
    vec3 specularColor = specularMap;
    float shininess = mix(8.0, 128.0, 1.0 - roughness); // 粗糙度越高，高光越散

    // 计算所有光源的贡献
    for (int i = 0; i < u_lightCount && i < MAX_LIGHTS; ++i) {
        // 传递动态参数
        float oldShininess = u_shininess;
        // u_shininess = shininess;
        result += CalculateLight(u_lights[i], normal, FragPos, viewDir, diffuseColor, specularColor);
        // u_shininess = oldShininess;
    }

    FragColor = vec4(result, 1.0);
}
