//
// Created by alber on 28/09/2025.
//

#ifndef PGA_PRACTICA_MYGUI_H
#define PGA_PRACTICA_MYGUI_H

#include <vector>
#include "Listener.h"

// Abstract class for every element of the user interface (controls, background, etc.)
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