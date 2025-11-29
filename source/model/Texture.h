#ifndef PGA_PRACTICA_TEXTURE_H
#define PGA_PRACTICA_TEXTURE_H
#include <string>

#include "glad/glad.h"

namespace PAG
{
    class Texture
    {
    private:
        GLuint _id = 0;
        std::string _path;
        int _width = 0;
        int _height = 0;

    public:
        Texture(const std::string& path);
        ~Texture();

        void bind() const;
        void unbind() const;
        std::string getPath() const { return _path; }
    };
}

#endif //PGA_PRACTICA_TEXTURE_H