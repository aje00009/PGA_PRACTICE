#ifndef PGA_PRACTICA_MODELTRANSFORMATIONWINDOW_H
#define PGA_PRACTICA_MODELTRANSFORMATIONWINDOW_H
#include "GUIElement.h"

namespace PAG
{
    /**
     * @class ModelManager Class that represents the GUI window that allow model transformation into models
     */
    class ModelManager: public GUIElement {
    private:
        static ModelManager* instance;

        //Selection options for transformations(default)
        int _selectedModel = 0;  ///< Selected model id
        int _selectedTransformation = 0; ///< Selected transformation (translation = 0, rotation = 1, scale = 2)

        float _translateVec[3] = {0,0,0}; ///< Translation on x,y,z
        float _rotateAngle = 45; ///< Angle in degrees
        int _rotateAxis = 1; ///< X = 0, Y = 1, Z = 2
        float _scaleVec[3] = {1,1,1}; ///< Scalation on x,y,z

        // Material selection
        int _selectedMaterial = 0; ///< 0 = No material, 1 = first material in collection...

        ModelEditPackage _package{}; ///< Payload to be sended to Renderer

        ModelManager() = default;
        void warnListeners() const;

    public:
        ~ModelManager();
        static ModelManager* getInstance();

        void render() override;
    };
}

#endif //PGA_PRACTICA_MODELTRANSFORMATIONWINDOW_H