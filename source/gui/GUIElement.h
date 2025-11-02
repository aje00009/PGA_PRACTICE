#ifndef PGA_PRACTICA_MYGUI_H
#define PGA_PRACTICA_MYGUI_H

#include <vector>
#include "../utils/Listener.h"

/**
 * @class GUIElement Interface that represents every element of the GUI (controls, log, camera control...)
 */
namespace PAG {
    class GUIElement {
    protected:
        std::vector<Listener*> _listeners;

    public:
        GUIElement() = default;
        virtual ~GUIElement() = default;

        void addListener(Listener* listener) {
            _listeners.push_back(listener);
        }

        virtual void render() = 0;
    };
}

#endif //PGA_PRACTICA_MYGUI_H