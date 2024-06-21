//
// Created by zhengshulin on 2024/6/19.
//

#ifndef OPENGLPROJ_ASSIMP_INCLUDE_H
#define OPENGLPROJ_ASSIMP_INCLUDE_H

#include <string>
#include <vector>
#include <iostream>
#include "glm/glm.hpp"
#include "core/shader.h"
#include "core/buffer.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "core/resource_manager.h"
#include "stb_image.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
namespace ex::assimp {

#define MAX_BONE_INFLUENCE 4

    struct Vertex {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
        //bone indexes which will influence this vertex
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        //weights from each bone
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    struct Texture {
        unsigned int id;
        string type;
        string path;
    };


    class Mesh{
    public:
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        void Draw(Shader* shader);
    private:
        Buffer*VBO;
        Buffer*EBO;
        VertexArray*VAO;
        void setupMesh();
    };

    Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        VAO=new VertexArray();
        VBO=new Buffer(BufferType::VBO);
        EBO=new Buffer(BufferType::EBO);
        setupMesh();
    }

    void Mesh::Draw(Shader *shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr   = 1;
        unsigned int heightNr   = 1;
        for (int i = 0; i < textures.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            string number;
            string name = textures[i].type;

            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);
            else if(name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if(name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            glUniform1i(glGetUniformLocation(shader->id, (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(VAO->getId());
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Mesh::setupMesh() {
        VAO->bind();

        VBO->bind();
        VBO->uploadData(vertices.size()* sizeof(Vertex),&vertices[0],DataUsage::STATIC_DRAW);

        EBO->bind();
        EBO->uploadData(indices.size()* sizeof(unsigned int),&indices[0],DataUsage::STATIC_DRAW);

        VAO->addBufferLayout({0,3,GL_FLOAT, false, sizeof(Vertex),0});
        VAO->enableAttribute(0);

        VAO->addBufferLayout({1,3,GL_FLOAT, false, sizeof(Vertex), (int)offsetof(Vertex,Normal)});
        VAO->enableAttribute(1);

        VAO->addBufferLayout({2,2,GL_FLOAT, false, sizeof(Vertex), (int)offsetof(Vertex,TexCoords)});
        VAO->enableAttribute(2);


        VAO->addBufferLayout({3,3,GL_FLOAT, false, sizeof(Vertex), (int)offsetof(Vertex,Tangent)});
        VAO->enableAttribute(3);

        VAO->addBufferLayout({4,3,GL_FLOAT, false, sizeof(Vertex), (int)offsetof(Vertex,Bitangent)});
        VAO->enableAttribute(4);

        VAO->addBufferLayout({5,4,GL_INT, false, sizeof(Vertex), (int)offsetof(Vertex,m_BoneIDs)});
        VAO->enableAttribute(5);

        VAO->addBufferLayout({6,4,GL_FLOAT, false, sizeof(Vertex), (int)offsetof(Vertex,m_Weights)});
        VAO->enableAttribute(6);

        VAO->unbind();
    }
    unsigned int TextureFromFile(const char *path, const string &directory, bool gamma= false);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
    class Model{
    public:
        vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        vector<Mesh>    meshes;
        string directory;
        bool gammaCorrection;

        // constructor, expects a filepath to a 3D model.
        Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
        {
            loadModel(path);
        }

        // draws the model, and thus all its meshes
        void Draw(Shader *shader);
    private:
        void loadModel(string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                             string typeName);

    };

    void Model::Draw(Shader *shader) {
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

    void Model::loadModel(string path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }

        int index=path.find_last_of('/');
        if (index<0){
            index=path.find_last_of('\\');
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, index);

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode *node, const aiScene *scene) {
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        for (int i = 0; i < mesh->mNumVertices; ++i) {
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position=vector;

            if (mesh->HasNormals()){
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            if (mesh->mTextureCoords[0]){
                glm::vec2 vec;
                vec.x=mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            } else{
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
            vertices.push_back(vertex);

        }


        for (int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face=mesh->mFaces[i];
            for (int j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }

       aiMaterial*material= scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        return Mesh(vertices,indices,textures);
    }

    vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
        vector<Texture> textures;
        for (int i = 0; i < mat->GetTextureCount(type); ++i) {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for(unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }

            if (!skip){
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);

            }

        }

        return textures;
    }
    unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
    {
        string filename = string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

}

#endif //OPENGLPROJ_ASSIMP_INCLUDE_H
