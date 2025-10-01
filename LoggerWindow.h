//
// Created by alber on 28/09/2025.
//

#ifndef PGA_PRACTICA_LOGGERWINDOW_H
#define PGA_PRACTICA_LOGGERWINDOW_H

#include "GUIElement.h"

/**
 * @brief Class that represents the logger window in the UI
 */
namespace PAG {
    class LoggerWindow : public GUIElement {
    private:
        static LoggerWindow* instance;
    public:
        LoggerWindow() = default;

        static LoggerWindow* getInstance();

        void render() override;
    };
}


#endif //PGA_PRACTICA_LOGGERWINDOW_H