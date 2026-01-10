#ifndef PGA_PRACTICA_MODEL_H
#define PGA_PRACTICA_MODEL_H

#include <vector>
#include <assimp/scene.h>

#include "Material.h"
#include "Texture.h"
#include "../shader/ShaderProgram.h"

namespace PAG {
    /**
     * @struct Vertex Encapsulates vertex data
     */
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TextCoord;
        glm::vec3 Tangent;
    };

    /**
     * @class Model Encapsulates model information and behaviour (vertices, indicies, name, model matrix, transformations...)
     */
    class Model {
    private:
        GLuint _idVAO = 0; ///< ID of VAO from this model
        GLuint _idVBO = 0; ///< ID of VBO from this model
        GLuint _idIBO = 0; ///< ID of IBO from this model

        std::vector<Vertex> _vertices; ///< Vertices representing the mesh of this model
        std::vector<GLuint> _indices; ///< Topology associated to the mesh of this model

        std::string modelName; ///< Name of the model (filename)

        glm::mat4 _modelMatrix; ///< Model matrix for each model (rotation, escalation, translation)

        ShaderProgram* _shaderProgram = nullptr; ///< Shader program assigned to Model

        Material* _material = nullptr; ///< Material assigned to Model

        Texture* _texture = nullptr; ///< Texture assigned to Model

        Texture* _normalMap = nullptr; ///< Normal map associated to Model

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

        void setName(const std::string& name);

        Material* getMaterial() const;
        void setMaterial(Material* mat);

        Texture* getTexture() const;
        void setTexture(Texture* texture);
        bool hasTexture() const;

        Texture* getNormalMap() const;
        void setNormalMap(Texture* normalMap);
        bool hasNormalMap() const;

        const std::string& getModelName() const;

        //Transformations for models
        void resetModelMatrix();
        void translate(const glm::vec3& translation);
        void scale(const glm::vec3& scale);
        void rotate(const glm::vec3& rotation, float angleRadians);
    };
}

#endif //PGA_PRACTICA_MODEL_H