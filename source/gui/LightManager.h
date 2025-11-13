#ifndef PGA_PRACTICA_LIGHTMANAGER_H
#define PGA_PRACTICA_LIGHTMANAGER_H

#include "GUIElement.h"
#include "../utils/Types.h"

namespace PAG {
    class LightManager: public GUIElement {
    private:
        //Background color for main window
        static LightManager* instance;

        int _selectedLight;
        int _lastSelected;
        bool _isDelete = false;
        char _nameBuffer[128] = "New light";

        LightPackage _payload;

        void warnListeners();

        LightManager();
    public:
        ~LightManager();
        static LightManager* getInstance();

        void render() override;
    };
}

#endif //PGA_PRACTICA_LIGHTMANAGER_H