//
// Created by alber on 25/10/2025.
//

#ifndef PGA_PRACTICA_MODELLOADERWINDOW_H
#define PGA_PRACTICA_MODELLOADERWINDOW_H
#include "GUIElement.h"
#include "imgui.h"
#include "imfilebrowser.h"

namespace PAG {
    /**
     * @class ModelLoaderWindow Class that represents the GUI window that allows loading models on the application
     */
    class ModelLoaderWindow: public GUIElement {
        private:
            static ModelLoaderWindow* instance;
            std::string model;

            ModelLoaderWindow() = default;

            void warnListeners() const;
        public:
            static ModelLoaderWindow* getInstance();

            void render() override;
    };
}

#endif //PGA_PRACTICA_MODELLOADERWINDOW_H