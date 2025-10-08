#ifndef PGA_PRACTICA_SHADERLOADWINDOW_H
#define PGA_PRACTICA_SHADERLOADWINDOW_H

#include "GUIElement.h"

/**
 * @class ShaderLoaderWindow Class that represents the window where the user can upload a shader
 */
namespace PAG {
    class ShaderLoaderWindow: public GUIElement {
    private:
        char nameShader[128]{};
        static ShaderLoaderWindow* instance;

    public:
        ShaderLoaderWindow() = default;

        static ShaderLoaderWindow* getInstance();

        void warnListeners() const;

        void render() override;
    };
}

#endif //PGA_PRACTICA_SHADERLOADWINDOW_H