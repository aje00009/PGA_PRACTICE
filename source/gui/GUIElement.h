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
        std::string title; ///< Title of the GUI window
        bool visible; ///< Visibility of window

        std::vector<Listener*> _listeners;

    public:
        GUIElement(const std::string& t): title(t), visible(false) {};
        virtual ~GUIElement() = default;

        void addListener(Listener* listener) {
            _listeners.push_back(listener);
        }

        virtual void render() = 0;

        [[nodiscard]] std::string getTitle() const {
            return title;
        }

        [[nodiscard]] bool& getVisible() {
            return visible;
        }

        void setVisible(const bool v) {
            visible = v;
        }
    };
}

#endif //PGA_PRACTICA_MYGUI_H