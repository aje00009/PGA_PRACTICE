#ifndef PGA_PRACTICA_RENDERMODEWINDOW_H
#define PGA_PRACTICA_RENDERMODEWINDOW_H
#include "GUIElement.h"

namespace PAG {
    class RenderModeWindow: public GUIElement {
    private:
        static RenderModeWindow* instance;

        RenderMode _renderMode;

        RenderModeWindow() = default;
        void warnListeners() const;

    public:
        static RenderModeWindow* getInstance();

        void render() override;
    };
}

#endif //PGA_PRACTICA_RENDERMODEWINDOW_H