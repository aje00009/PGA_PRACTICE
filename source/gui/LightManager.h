#ifndef PGA_PRACTICA_LIGHTMANAGER_H
#define PGA_PRACTICA_LIGHTMANAGER_H

#include "GUIElement.h"
#include "../utils/Types.h"

/**
 * @class LightManager Class representing GUI controls for light management (creation, edition, etc.)
 */
namespace PAG {
    class LightManager: public GUIElement {
    private:
        static LightManager* instance; ///< Singleton instance

        int _selectedLight{}; ///< Current selected light
        int _lastSelected{}; ///< Last light selected
        bool _isDelete = false; ///< Delete flag (if the model will be deleted)
        char _nameBuffer[128] = "New light"; ///< Name of the light

        LightPackage _payload; ///< Payload to send to listeners in order to manage light

        void warnListeners();

        LightManager();
    public:
        ~LightManager();
        static LightManager* getInstance();

        void render() override;
    };
}

#endif //PGA_PRACTICA_LIGHTMANAGER_H