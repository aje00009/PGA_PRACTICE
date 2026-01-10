#define GLM_ENABLE_EXPERIMENTAL

#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/gtx/transform.hpp>

#include "Model.h"

/**
 * @brief Parameterizde constructor of Model
 * @param shaderProgram Shader program assigned to the model
 * @param modelPath Path of the model to be loaded
 */
PAG::Model::Model(ShaderProgram *shaderProgram, const std::string &modelPath): _shaderProgram(shaderProgram), _modelMatrix(glm::mat4(1.0f)) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error("Assimp error while loading model: " + std::string(importer.GetErrorString()));
    }

    modelName = std::filesystem::path(modelPath).filename().string();

    processNode(scene->mRootNode, scene);

    setupBuffers();
}

/**
 * @brief Method that process a node following assimp logic
 * @param node Current node defined by assimp
 * @param scene Scene defined by assimp
 */
void PAG::Model::processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

/**
 * @brief Method that process a mesh (only one per model)
 * @param mesh Mesh defined by assimp
 * @param scene Scene defined by assimp
 */
void PAG::Model::processMesh(aiMesh *mesh, const aiScene *scene) {

    //Loop for all vertices of the model
    for (unsigned int i = 0;i < mesh->mNumVertices; i++) {
        Vertex vertex{};

        vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

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
            vertex.TextCoord = { 0.0f, 0.0f };
        }

        //Tangents
        if (mesh->mTangents) {
            vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
        }else {
            vertex.Tangent = { 0.0f, 0.0f, 0.0f };
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

/**
 * @brief Method that encaspulates the generation, enabling and binding of buffers (VAO,VBO,IBO)
 */
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

    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // Texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextCoord));

    //Tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    glBindVertexArray(0);
}

/**
 * @brief Destructor of class Model
 * @post It will destroy all necessary buffers
 */
PAG::Model::~Model() {
    if (_idVAO)
        glDeleteVertexArrays(1, &_idVAO);
    if (_idVBO)
        glDeleteBuffers(1, &_idVBO);
    if (_idIBO)
        glDeleteBuffers(1, &_idIBO);
}

/**
 * @brief Method that draws the model on the scene
 */
void PAG::Model::draw() const {
    glBindVertexArray(_idVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

/**
 * @return Shader program associated to the model
 */
PAG::ShaderProgram * PAG::Model::getShaderProgram() const {
    return this->_shaderProgram;
}

void PAG::Model::setName(const std::string &name) {
    this->modelName = name;
}

/**
 * @return Method that return the material associated to the model
 */
PAG::Material * PAG::Model::getMaterial() const {
    return _material;
}

/**
 * @brief Method that assigns a material to a model
 * @param mat Material to be assigned
 */
void PAG::Model::setMaterial(Material *mat) {
    _material = mat;
}

PAG::Texture* PAG::Model::getTexture() const
{
    return _texture;
}

void PAG::Model::setTexture(Texture* texture)
{
    _texture = texture;
}

bool PAG::Model::hasTexture() const
{
    return _texture != nullptr;
}

PAG::Texture * PAG::Model::getNormalMap() const {
    return _normalMap;
}

void PAG::Model::setNormalMap(Texture *normalMap) {
    _normalMap = normalMap;
}

bool PAG::Model::hasNormalMap() const {
    return _normalMap != nullptr;
}

/**
 * @return The name of the model
 */
const std::string& PAG::Model::getModelName() const
{
    return modelName;
}

/**
 * @brief Method that sets a model matrix to a model
 * @param matrix The model matrix to be assigned
 */
void PAG::Model::setModelMatrix(const glm::mat4& matrix) {
    _modelMatrix = matrix;
}

/**
 * @return The model matrix for the model
 */
glm::mat4 PAG::Model::getModelMatrix() const {
    return _modelMatrix;
}

/**
 * @brief Method that resets the model -> Resets the model matrix of the model (identity)
 */
void PAG::Model::resetModelMatrix() {
    _modelMatrix = glm::mat4(1.0f);
}

/**
 * @brief Method that translates a model
 * @param v Traslation to be applied to the model
 */
void PAG::Model::translate(const glm::vec3& v) {
    _modelMatrix = glm::translate(_modelMatrix, v);
}

/**
 * @brief Method that rotates a model
 * @param axis Axis (x,y,z) to be rotated by
 * @param angleRadians Angle in radians of the rotation to be applied
 */
void PAG::Model::rotate(const glm::vec3& axis, float angleRadians) {
    _modelMatrix = glm::rotate(_modelMatrix, angleRadians, axis);
}

/**
 * @brief Method that scales a model
 * @param s Scaling factors to be applied (x,y,z)
 */
void PAG::Model::scale(const glm::vec3& s) {
    _modelMatrix = glm::scale(_modelMatrix, s);
}
