//
// Created by alber on 17/09/2025.
//

#include "Renderer.h"

#include "imgui.h"
#include "Logger.h"

PAG::Renderer* PAG::Renderer::instance = nullptr;

PAG::Renderer::Renderer() = default;


PAG::Renderer *PAG::Renderer::getInstance() {
    if (!instance)
        instance = new Renderer();
    return instance;
}

//Esta función callback será llamada cuando GLFW produzca algún error
void PAG::Renderer::error_callback(int error, const char* description) {
    std::string aux(description);
    Logger::getInstance()->addMessage("Error de GLFW número " + std::to_string(error) + ": " + aux);
}

//Esta función será llamada cada vez que el área de dibujo de OpenGL
//deba ser redibujada
void PAG::Renderer::window_refresh_callback(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
    // intercambia el buffer back (que se ha estado dibujando) por el
    // que se mostraba hasta ahora front. Debe ser la última orden de
    // este callback
    glfwSwapBuffers(window);

    Logger::getInstance()->addMessage("Refresh callback called");
}

// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void PAG::Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Logger::getInstance()->addMessage("Resize callback called");
}

// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void PAG::Renderer::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        ImGuiIO& io = ImGui::GetIO();
        io.AddKeyEvent(static_cast<ImGuiKey>(key),true);
    }

    Logger::getInstance()->addMessage("Key callback called");
}

//Esta función callback será llamada caca vez que sea pulse algun botón
//del ratón sobre el área de dibujo OpenGL
void PAG::Renderer::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        Logger::getInstance()->addMessage("Button pressed: " + std::to_string(button));
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseButtonEvent(button,true);
    }
    else if (action == GLFW_RELEASE) {
        Logger::getInstance()->addMessage("Button released: " + std::to_string(button));
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseButtonEvent(button,false);
    }
}

//Esta función callback será llamada cada vez que se mueva la rueda
//del ratón sobre el área de dibujo OpenGL
void PAG::Renderer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Logger::getInstance()->addMessage("Scrolled " + std::to_string(xoffset) + " units horizontally and "
    + std::to_string(yoffset) + " units vertically");
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent(xoffset,yoffset);
}
