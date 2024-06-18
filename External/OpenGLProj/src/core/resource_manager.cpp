//
// Created by zhengshulin on 2024/5/21.
//

#include "resource_manager.h"
#include "glad/gl.h"
#include "res_path.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;

std::string normalizePath(const std::filesystem::path& path) {
    std::string normalizedPath = path.string();
#ifdef _WIN32
    std::replace(normalizedPath.begin(), normalizedPath.end(), '/', '\\');
#else
    std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
#endif
    return normalizedPath;
}
Shader ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name)
{
    auto vPath=std::filesystem::path(RES_PATH) / "shader" / std::string(vShaderFile);
    auto fPath=std::filesystem::path(RES_PATH) / "shader" / std::string(fShaderFile);
    std::filesystem::path gPath;
    if (gShaderFile!= nullptr){
        gPath=std::filesystem::path(RES_PATH) / "shader" / std::string(gShaderFile);
    }
    std::string normalizedVPath = normalizePath(vPath);
    std::string normalizedFPath = normalizePath(fPath);
    std::string normalizedGPath = normalizePath(gPath);

    Shaders[name] = loadShaderFromFile(normalizedVPath.c_str(), normalizedFPath.c_str(), normalizedGPath.c_str());
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char *file, std::string name)
{
    auto spritePath=std::filesystem::path(RES_PATH) / "textures" / std::string(name).append(".png");
    std::string normalizedSpritePath = normalizePath(spritePath);
    Textures[name] = loadTextureFromFile(normalizedSpritePath.c_str());
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (Properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.id);
    // (Properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, (unsigned int*)&iter.second.id);
}

Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr && strlen(gShaderFile)>0)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }else{
            gShaderFile= nullptr;
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    // 2. Now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char *file)
{
    // Create Texture object
    Texture2D texture;

    // Load image
    int width, height,ch;
    unsigned char* image = stbi_load(file, &width, &height, &ch,0);

    // Now generate texture
    texture.Generate(width, height, ch,image);
    // And finally free image data
    stbi_image_free(image);
    return texture;
}