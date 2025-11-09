#ifndef PGA_PRACTICA_MODELEDITINGWINDOW_H
#define PGA_PRACTICA_MODELEDITINGWINDOW_H

#include "GUIElement.h"

namespace PAG {
    class MaterialEditingWindow: public GUIElement {
    private:
        static MaterialEditingWindow* instance;

        int _selectedMaterial = 0;  ///< 0 = New material, 1 = first material on collection...
        int _lastSelected = -1;
        MaterialEditingPackage _package;

        char _nameBuffer[128] = "New material"; ///< char array for ImGui

        MaterialEditingWindow() = default;
        void warnListeners();

    public:
        static MaterialEditingWindow* getInstance();

        void render() override;

    };
}

#endif //PGA_PRACTICA_MODELEDITINGWINDOW_H