//
// Created by zhengshulin on 2024/5/21.
//

#ifndef OPENGLPROJ_RESOURCE_MANAGER_H
#define OPENGLPROJ_RESOURCE_MANAGER_H


#include <map>
#include "shader.h"
#include "texture.h"

class ResourceManager
{
public:
    // Resource storage
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;
    // Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader   LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
    // Retrieves a stored sader
    static Shader   GetShader(std::string name);
    // Loads (and generates) a texture from file
    static Texture2D LoadTexture(const char *file,  std::string name);
    // Retrieves a stored texture
    static Texture2D GetTexture(std::string name);
    // Properly de-allocates all loaded resources
    static void      Clear();
private:
    // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }
    // Loads and generates a shader from file
    static Shader    loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    // Loads a single texture from file
    static Texture2D loadTextureFromFile(const char *file);
};

#endif //OPENGLPROJ_RESOURCE_MANAGER_H
