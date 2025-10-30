#ifndef PGA_PRACTICA_SHADER_H
#define PGA_PRACTICA_SHADER_H

#include <string>
#include "glad/glad.h"


/**
 * @class Shader Class that generalises both vertex/fragment shader functionality
 */
namespace PAG {
    class Shader {
    protected:
        GLuint _id = 0;
        GLenum _type;

        void compile(const std::string& source);

    public:
        Shader(const std::string& filePath, GLenum type);
        virtual ~Shader();

        GLuint getId() const;
    };
}


#endif //PGA_PRACTICA_SHADER_H