//
// Created by alber on 05/10/2025.
//

#ifndef PGA_PRACTICA_SHADER_H
#define PGA_PRACTICA_SHADER_H

#include "glad/glad.h"
#include <string>

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