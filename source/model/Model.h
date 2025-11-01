//
// Created by alber on 25/10/2025.
//

#ifndef PGA_PRACTICA_MODEL_H
#define PGA_PRACTICA_MODEL_H

#include <glm/glm.hpp>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "../shader/ShaderProgram.h"

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

        std::vector<Vertex> _vertices;
        std::vector<Texture> _textures;
        std::vector<GLuint> _indices;

        std::string modelName;

        glm::mat4 _modelMatrix; ///< Model matrix for each model (rotation, escalation, translation)

        ShaderProgram* _shaderProgram = nullptr;

        void processNode(aiNode* node, const aiScene* scene);

        void processMesh(aiMesh* mesh, const aiScene* scene);

        void setupBuffers();

    public:
        Model(ShaderProgram* shaderProgram, const std::string& modelPath);
        virtual ~Model();


        void draw() const;

        void setModelMatrix(const glm::mat4& modelMatrix);
        glm::mat4 getModelMatrix() const;
        ShaderProgram* getShaderProgram() const;
        const std::string& getModelName() const;

        //Transformations for models
        void resetModelMatrix();
        void translate(const glm::vec3& translation);
        void scale(const glm::vec3& scale);
        void rotate(const glm::vec3& rotation, float angleRadians);
    };
}

#endif //PGA_PRACTICA_MODEL_H