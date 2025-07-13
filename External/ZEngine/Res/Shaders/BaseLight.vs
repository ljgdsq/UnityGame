#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    // 计算世界空间中的位置
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // 计算世界空间中的法线（考虑非均匀缩放）
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // 传递纹理坐标
    TexCoord = aTexCoord;
    
    // 计算最终的屏幕位置
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
