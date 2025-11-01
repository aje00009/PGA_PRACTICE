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

        //Selection options (default)
        int _selectedModel = 0;
        int _selectedTransformation = 0;

        float _translateVec[3] = {0,0,0}; //Translation on x,y,z
        float _rotateAngle = 45; //Angle in degrees
        int _rotateAxis = 1; // X = 0, Y = 1, Z = 2
        float _scaleVec[3] = {1,1,1}; //Scalation on x,y,z

        TransformPackage _package;

        ModelTransformationWindow() = default;
        void warnListeners() const;

    public:
        static ModelTransformationWindow* getInstance();

        void render() override;
    };
}

#endif //PGA_PRACTICA_MODELTRANSFORMATIONWINDOW_H