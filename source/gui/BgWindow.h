#ifndef PGA_PRACTICA_BGWINDOW_H
#define PGA_PRACTICA_BGWINDOW_H

#include "GUIElement.h"

/**
 * @class BgWindow Class that implements the interface of a GUI element which specifies itself as the background color control
 */
namespace PAG {
    class BgWindow: public GUIElement {
        private:
        //Background color for main window
        float bgColor[4] = {0.6, 0.6, 0.6, 1.0};
        static BgWindow* instance;

        void warnListeners();

        public:
        BgWindow() = default;

        static BgWindow* getInstance();

        void render() override;
    };
}


#endif //PGA_PRACTICA_BGWINDOW_H