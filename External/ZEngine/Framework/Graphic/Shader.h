#pragma once
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace framework {
    class Shader {
    public:

        Shader(const char* vPath, const char* fPath);

        // State
        int id;
        // Constructor
        Shader() {}
        // Sets the current shader as active
        void   Use();
        // Compiles the shader from given source code
        void    Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr); // Note: geometry source code is optional 
        // Utility functions
        void    SetFloat(const char* name, float value, bool useShader = false);
        void    SetInteger(const char* name, int value, bool useShader = false);
        void    SetVector2f(const char* name, float x, float y, bool useShader = false);
        void    SetVector2f(const char* name, const glm::vec2& value, bool useShader = false);
        void    SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
        void    SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
        void    SetVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
        void    SetVector4f(const char* name, const glm::vec4& value, bool useShader = false);
        void    SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);
        void    SetMat4(const char* name, const glm::mat4& matrix, bool useShader = false);
        void    SetInt(const char* name, int value, bool useShader = false) { SetInteger(name, value, useShader); }
    private:
        // Checks if compilation or linking failed and if so, print the error logs
        void    checkCompileErrors(int object, std::string type);
    };

}