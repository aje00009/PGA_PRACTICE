//
// Created by alber on 25/10/2025.
//

#ifndef PGA_PRACTICA_MODEL_H
#define PGA_PRACTICA_MODEL_H
#include <vector>

#include "../shader/ShaderProgram.h"
#include "glad/glad.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace PAG {
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec3 Color;
        glm::vec2 TextCoord;
    };

    struct Texture {
        unsigned int id;
        std::string type;
    };

    class Model {
    private:
        GLuint _idVAO = 0;
        GLuint _idVBO = 0;
        GLuint _idIBO = 0;
        GLsizei _indexCount = 0;

        std::vector<Vertex> _vertices;
        std::vector<Texture> _textures;
        std::vector<GLuint> _indices;

        ShaderProgram* _shaderProgram = nullptr;

        void processNode(aiNode* node, const aiScene* scene);

        void processMesh(aiMesh* mesh, const aiScene* scene);
    };
}

#endif //PGA_PRACTICA_MODEL_H