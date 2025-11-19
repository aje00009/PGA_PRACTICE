#ifndef PGA_PRACTICA_SHADER_H
#define PGA_PRACTICA_SHADER_H

#include "glad/glad.h"
#include <string>

/**
 * @class Shader Class that generalises both vertex/fragment shader functionality
 */
namespace PAG {
    class Shader {
    protected:
        GLuint _id = 0; ///< Id of the shader
        GLenum _type; ///< Type of shader (vertex/fragment)

        void compile(const std::string& source);

    public:
        Shader(const std::string& filePath, GLenum type);
        virtual ~Shader();

        GLuint getId() const;
    };
}


#endif //PGA_PRACTICA_SHADER_H