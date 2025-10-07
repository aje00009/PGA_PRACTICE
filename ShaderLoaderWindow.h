//
// Created by alber on 02/10/2025.
//

#ifndef PGA_PRACTICA_SHADERLOADWINDOW_H
#define PGA_PRACTICA_SHADERLOADWINDOW_H

#include "GUIelement.h"

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