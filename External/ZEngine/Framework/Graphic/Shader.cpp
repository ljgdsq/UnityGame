#include "glad/gl.h"
#include "Framework/Graphic/Shader.h"
#include "Framework/Log/Logger.h"
#include "Framework/Core/EngineFileIO.h"
namespace framework
{
    Shader::Shader(const char *vPath, const char *fPath)
    {
        // Load shader source code from files
        std::string vertexCode = EngineFileIO::LoadText(vPath);
        std::string fragmentCode = EngineFileIO::LoadText(fPath);
        if (vertexCode.empty() || fragmentCode.empty())
        {
            Logger::Error("Failed to load shader source code from {} or {}", vPath, fPath);
            return;
        }

        // Compile the shader
        Compile(vertexCode.c_str(), fragmentCode.c_str());
    }

    void Shader::Use()
    {
        glUseProgram(this->id);
    }

    void Shader::Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource)
    {
        int sVertex, sFragment, gShader;
        // Vertex Shader
        sVertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(sVertex, 1, &vertexSource, NULL);
        glCompileShader(sVertex);
        checkCompileErrors(sVertex, "VERTEX");
        // Fragment Shader
        sFragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(sFragment, 1, &fragmentSource, NULL);
        glCompileShader(sFragment);
        checkCompileErrors(sFragment, "FRAGMENT");
        // If geometry shader source code is given, also compile geometry shader
        if (geometrySource != nullptr)
        {
            gShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(gShader, 1, &geometrySource, NULL);
            glCompileShader(gShader);
            checkCompileErrors(gShader, "GEOMETRY");
        }
        // Shader Program
        this->id = glCreateProgram();
        glAttachShader(this->id, sVertex);
        glAttachShader(this->id, sFragment);
        if (geometrySource != nullptr)
            glAttachShader(this->id, gShader);
        glLinkProgram(this->id);
        checkCompileErrors(this->id, "PROGRAM");

        shaderInfo = std::make_shared<ShaderReflection>(this->id);

        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(sVertex);
        glDeleteShader(sFragment);
        if (geometrySource != nullptr)
            glDeleteShader(gShader);
    }

    void Shader::SetTexture(const char *name, int slot, bool useShader)
    {
        if (useShader)
            this->Use();
        if (auto location = shaderInfo->GetSamplerLocation(name))
        {
            glUniform1i(*location, slot);
            //glActiveTexture(GL_TEXTURE0 + slot);
        }
    }


    void Shader::SetFloat(const char *name, float value, bool useShader)
    {
        if (useShader)
            this->Use();
        if (auto location = shaderInfo->GetUniformLocation(name))
            glUniform1f(*location, value);
    }

    void Shader::SetInteger(const char *name, int value, bool useShader)
    {
        if (useShader)
            this->Use();
        if (auto location = shaderInfo->GetUniformLocation(name))
            glUniform1i(*location, value);
    }

    void Shader::SetVector2f(const char *name, float x, float y, bool useShader)
    {
        if (useShader)
            this->Use();
        if (auto location = shaderInfo->GetUniformLocation(name))
            glUniform2f(*location, x, y);
    }

    void Shader::SetVector2f(const char *name, const glm::vec2 &value, bool useShader)
    {
        if (useShader)
            this->Use();
        if (auto location = shaderInfo->GetUniformLocation(name))
            glUniform2f(*location, value.x, value.y);
    }

    void Shader::SetVector3f(const char *name, float x, float y, float z, bool useShader)
    {
        if (useShader)
            this->Use();
        if (auto location = shaderInfo->GetUniformLocation(name))
            glUniform3f(*location, x, y, z);
    }

    void Shader::SetVector3f(const char *name, const glm::vec3 &value, bool useShader)
    {
        if (useShader)
            this->Use();
        if (auto location = shaderInfo->GetUniformLocation(name))
            glUniform3f(*location, value.x, value.y, value.z);
    }

    void Shader::SetVector4f(const char *name, float x, float y, float z, float w, bool useShader)
    {
        if (useShader)
            this->Use();
        if (auto location = shaderInfo->GetUniformLocation(name))
            glUniform4f(*location, x, y, z, w);
    }

    void Shader::SetVector4f(const char *name, const glm::vec4 &value, bool useShader)
    {
        if (useShader)
            this->Use();
        if (auto location = shaderInfo->GetUniformLocation(name))
            glUniform4f(*location, value.x, value.y, value.z, value.w);
    }

    void Shader::SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader)
    {
        if (useShader)
            this->Use();
        if (auto location = shaderInfo->GetUniformLocation(name))
            glUniformMatrix4fv(*location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
    void Shader::SetMat4(const char *name, const glm::mat4 &matrix, bool useShader)
    {
        SetMatrix4(name, matrix, useShader);
    }

    void Shader::SetProperty(const char *name, const SerializableValue &value, bool useShader)
    {
        if (useShader)
            this->Use();
        if (auto location = shaderInfo->GetUniformLocation(name))
        {
            switch (value.GetType())
            {
                case SerializableValueType::Float:
                    glUniform1f(*location, value.GetValue<float>());
                    break;
                case SerializableValueType::Int:
                    glUniform1i(*location, value.GetValue<int>());
                    break;
                case SerializableValueType::Vec2:
                    glUniform2fv(*location, 1, glm::value_ptr(value.GetValue<glm::vec2>()));
                    break;
                case SerializableValueType::Vec3:
                    glUniform3fv(*location, 1, glm::value_ptr(value.GetValue<glm::vec3>()));
                    break;
                case SerializableValueType::Vec4:
                    glUniform4fv(*location, 1, glm::value_ptr(value.GetValue<glm::vec4>()));
                    break;
                default:
                    Logger::Error("Unsupported SerializableValue type for shader property: {}", (int)value.GetType());
            }
        }
    }


    void Shader::checkCompileErrors(int object, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(object, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(object, 1024, NULL, infoLog);

                Logger::Error("| ERROR::Shader: Compile-time error: Type: {} \n{}\n -- --------------------------------------------------- -- ",
                              type, infoLog);
            }
        }
        else
        {
            glGetProgramiv(object, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(object, 1024, NULL, infoLog);
                Logger::Error("| ERROR::Shader: Link-time error: Type: {} \n{}\n -- --------------------------------------------------- -- ",
                              type, infoLog);
            }
        }
    }

}
