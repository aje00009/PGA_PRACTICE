#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "Model.h"
#include <string>

#include "../utils/Logger.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

PAG::Model::Model(ShaderProgram *shaderProgram, const std::string &modelPath): _shaderProgram(shaderProgram), _modelMatrix(glm::mat4(1.0f)) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error("Assimp error while loading model: " + std::string(importer.GetErrorString()));
    }

    processNode(scene->mRootNode, scene);

    setupBuffers();
}

void PAG::Model::processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void PAG::Model::processMesh(aiMesh *mesh, const aiScene *scene) {

    //Loop for all vertices of the model
    for (unsigned int i = 0;i < mesh->mNumVertices; i++) {
        Vertex vertex{};

        vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

        // Colors for each vertex
        if (mesh->HasVertexColors(0)) {
            vertex.Color = { mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b };
        } else {
            vertex.Color = { 1.0f, 1.0f, 1.0f };
        }

        // Normals
        if (mesh->HasNormals()) {
            vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        } else {
            vertex.Normal = { 0.0f, 1.0f, 0.0f };
        }

        // Texture coordinates
        if (mesh->HasTextureCoords(0)) {
            vertex.TextCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        } else {
            // Si no hay texturas, ponemos una por defecto
            vertex.TextCoord = { 0.0f, 0.0f };
        }

        _vertices.push_back(vertex);
    }

    //Loop for indices (edges) of the model
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            _indices.push_back(face.mIndices[j]);
        }
    }


}

void PAG::Model::setupBuffers() {
    glGenVertexArrays(1, &_idVAO);
    glBindVertexArray(_idVAO);

    glGenBuffers(1, &_idVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _idVBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &_idIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _idIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    // Colors
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

    // Normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // Texture coordinates
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextCoord));

    glBindVertexArray(0);
}


PAG::Model::~Model() {
    if (_idVAO)
        glDeleteVertexArrays(1, &_idVAO);
    if (_idVBO)
        glDeleteBuffers(1, &_idVBO);
    if (_idIBO)
        glDeleteBuffers(1, &_idIBO);
}

void PAG::Model::draw() const {
    glBindVertexArray(_idVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_vertices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

PAG::ShaderProgram * PAG::Model::getShaderProgram() const {
    return this->_shaderProgram;
}

void PAG::Model::setModelMatrix(const glm::mat4& matrix) {
    _modelMatrix = matrix;
}

glm::mat4 PAG::Model::getModelMatrix() const {
    return _modelMatrix;
}

void PAG::Model::resetModelMatrix() {
    _modelMatrix = glm::mat4(1.0f);
}

// Aplica una traslación A LA MATRIZ ACTUAL
void PAG::Model::translate(const glm::vec3& v) {
    _modelMatrix = glm::translate(_modelMatrix, v);
}

// Aplica una rotación A LA MATRIZ ACTUAL
void PAG::Model::rotate(const glm::vec3& axis, float angleRadians) {
    _modelMatrix = glm::rotate(_modelMatrix, angleRadians, axis);
}

// Aplica un escalado A LA MATRIZ ACTUAL
void PAG::Model::scale(const glm::vec3& s) {
    _modelMatrix = glm::scale(_modelMatrix, s);
}
