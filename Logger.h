//
// Created by alber on 17/09/2025.
//

#ifndef PGA_PRACTICA_LOGGER_H
#define PGA_PRACTICA_LOGGER_H

#include <sstream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class Logger {
    private:
        static Logger* instance;
        std::stringstream messages;
        void clear();
    public:
        template<class T>
        Logger& operator<<(const T& smth);

        void addMessage(const std::string& message);
        void draw();
        static Logger* getInstance();
};


#endif //PGA_PRACTICA_LOGGER_H