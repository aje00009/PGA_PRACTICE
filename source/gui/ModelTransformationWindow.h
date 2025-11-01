//
// Created by albertoje04 on 1/11/25.
//

#ifndef PGA_PRACTICA_MODELTRANSFORMATIONWINDOW_H
#define PGA_PRACTICA_MODELTRANSFORMATIONWINDOW_H
#include "GUIElement.h"

namespace PAG
{
    class ModelTransformationWindow: public GUIElement {
    private:
        static ModelTransformationWindow* instance;

        ModelTransformationWindow() = default;
        void warnListeners() const;

    public:
        static ModelTransformationWindow* getInstance();

        void render() override;
    };
}

#endif //PGA_PRACTICA_MODELTRANSFORMATIONWINDOW_H