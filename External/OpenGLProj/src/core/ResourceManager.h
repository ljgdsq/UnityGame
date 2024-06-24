//
// Created by zhengshulin on 2024/5/21.
//

#ifndef OPENGLPROJ_RESOURCE_MANAGER_H
#define OPENGLPROJ_RESOURCE_MANAGER_H


#include <map>
#include <string>
#include "shader.h"
#include "texture.h"

using std::string;
class ResourceManager
{
public:
    static std::map<std::string, Shader*>    Shaders;
    static std::map<std::string, Texture2D*> Textures;
    static Shader*   LoadShader(string vShaderFile, string fShaderFile, string gShaderFile="", string name="");
    static Shader*   GetShader(std::string name);
    static Texture2D* LoadTexture(std::string name);
    static Texture2D* GetTexture(std::string name);
    static void      Clear();
private:
    ResourceManager() { }
    static Shader    *loadShaderFromFile(const string &vShaderFile, const string &fShaderFile, const string &gShaderFile = "");
    static Texture2D *loadTextureFromFile(const char *file);
};

#endif //OPENGLPROJ_RESOURCE_MANAGER_H
