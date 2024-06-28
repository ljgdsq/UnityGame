//
// Created by zhengshulin on 2024/5/21.
//

#include "ResourceManager.h"
#include "res_path.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "utils/FileUtil.h"

// Instantiate static variables
std::map<std::string, Texture2D *>    ResourceManager::Textures;
std::map<std::string, Shader *>       ResourceManager::Shaders;

std::string normalizePath(const std::filesystem::path &path) {
    return FileUtil::NormalizePath(path);
}

Shader *ResourceManager::LoadShader(string vShaderFile, string fShaderFile, string gShaderFile, string name) {

    if (name == "") {
        name = vShaderFile + fShaderFile + gShaderFile;
    }

    if (Shaders.find(name) != Shaders.end()) {
        return Shaders[name];
    }

    auto vPath = std::filesystem::path(RES_PATH) / "shader" / std::string(vShaderFile);
    auto fPath = std::filesystem::path(RES_PATH) / "shader" / std::string(fShaderFile);
    std::filesystem::path gPath;
    if (gShaderFile != "") {
        gPath = std::filesystem::path(RES_PATH) / "shader" / std::string(gShaderFile);
    }
    std::string normalizedVPath = normalizePath(vPath);
    std::string normalizedFPath = normalizePath(fPath);
    std::string normalizedGPath = normalizePath(gPath);

    Shaders[name] = loadShaderFromFile(normalizedVPath, normalizedFPath, normalizedGPath);
    return Shaders[name];
}

Shader *
ResourceManager::loadShaderFromFile(const string &vShaderFile, const string &fShaderFile, const string &gShaderFile) {
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try {
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
        if (gShaderFile != "" && gShaderFile.size() > 0) {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e) {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    // 2. Now create shader object from source code
    Shader *shader = new Shader();
    shader->Compile(vShaderCode, fShaderCode, gShaderFile != "" ? gShaderCode : nullptr);
    return shader;
}


Shader *ResourceManager::GetShader(std::string name) {
    if (Shaders.find(name)==Shaders.end()){
        return LoadShader(name+".vs",name+".fs","",name);
    }
    return Shaders[name];
}

Texture2D *ResourceManager::LoadTexture(std::string name) {
    if (Textures.find(name) != Textures.end()) {
        return Textures[name];
    }
    auto spritePath = std::filesystem::path(name);
    if (name.find("res/textures") == string::npos && name.find("res\\textures") == string::npos) {
        spritePath = std::filesystem::path(RES_PATH) / "textures";
        if (name.find(".png") != std::string::npos) {
            spritePath = spritePath / std::string(name);
        } else {
            spritePath = spritePath / std::string(name).append(".png");
        }
    }
    std::string normalizedSpritePath = normalizePath(spritePath);
    Textures[name] = loadTextureFromFile(normalizedSpritePath.c_str());
    return Textures[name];
}

Texture2D *ResourceManager::GetTexture(std::string name) {
    return LoadTexture(name);
}

void ResourceManager::Clear() {

}


Texture2D *ResourceManager::loadTextureFromFile(const char *file) {
    // Create Texture object
    Texture2D *texture = new Texture2D();

    // Load image
    int width, height, ch;
    unsigned char *image = stbi_load(file, &width, &height, &ch, 0);
    // Now generate texture
    texture->Generate(width, height, ch, image);
    texture->path=file;
    // And finally free image data
    stbi_image_free(image);
    return texture;
}



