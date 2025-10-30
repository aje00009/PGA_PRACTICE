#include <iostream>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "gui/BgWindow.h"
#include "gui/ManagerGUI.h"
#include "rendering/Renderer.h"
#include "utils/Logger.h"
#include "gui/LoggerWindow.h"
#include "gui/ModelLoaderWindow.h"
#include "gui/ShaderLoaderWindow.h"

//Global variables
bool draging = false;
double lastMouseX = 0.0, lastMouseY = 0.0;

//Esta función callback será llamada cuando GLFW produzca algún error
void error_callback(int error, const char* description) {
    PAG::Renderer::error_callback(error,description);
}

//Esta función será llamada cada vez que el área de dibujo de OpenGL
//deba ser redibujada
void window_refresh_callback(GLFWwindow* window) {
    PAG::Renderer::getInstance()->refresh();
}

// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    PAG::Renderer::framebuffer_size_callback(width, height);
}

// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

//Esta función callback será llamada caca vez que sea pulse algun botón
//del ratón sobre el área de dibujo OpenGL
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(button, action == GLFW_PRESS);

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            draging = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        } else if (action == GLFW_RELEASE) {
            draging = false;
        }
    }
}

void set_cursor_pos_callback(GLFWwindow* w, double xpos, double ypos){
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(xpos, ypos);

    if (io.WantCaptureMouse) return;

    if (draging) {
        float deltaX = static_cast<float>(xpos - lastMouseX);
        float deltaY = static_cast<float>(ypos - lastMouseY);
        lastMouseX = xpos;
        lastMouseY = ypos;

        PAG::CameraMovement move = PAG::CameraWindow::getSelectedMovement();

        PAG::Renderer::cursor_pos_callback(move, deltaX, deltaY);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent(xoffset, yoffset);

    if (io.WantCaptureMouse) return;

    PAG::Renderer::scroll_callback(xoffset,yoffset);
}

void initializeGUI() {
    //Add GUI windows/controls
    PAG::ManagerGUI::getInstance()->addWindow(PAG::BgWindow::getInstance());
    PAG::ManagerGUI::getInstance()->addWindow(PAG::LoggerWindow::getInstance());
    PAG::ManagerGUI::getInstance()->addWindow(PAG::ShaderLoaderWindow::getInstance());
    PAG::ManagerGUI::getInstance()->addWindow(PAG::CameraWindow::getInstance());
    PAG::ManagerGUI::getInstance()->addWindow(PAG::ModelLoaderWindow::getInstance());

    //Add listeners GUI
    PAG::BgWindow::getInstance()->addListener(PAG::Renderer::getInstance());
    PAG::ShaderLoaderWindow::getInstance()->addListener(PAG::Renderer::getInstance());
    PAG::CameraWindow::getInstance()->addListener(PAG::Renderer::getInstance());
    PAG::ModelLoaderWindow::getInstance()->addListener(PAG::Renderer::getInstance());
}

int main() {
    srand(time(NULL));

    PAG::Logger::getInstance()->addMessage("Starting Application PAG - Prueba 01");

    //GLFW Errors callback before executing any GLFW functions
    glfwSetErrorCallback(error_callback);
    // - Inicializa GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
    if ( glfwInit () != GLFW_TRUE )
    {
        PAG::Logger::getInstance()->addMessage("Failed to initialize GLFW");
        return -1;
    }

    // - Definimos las características que queremos que tenga el contexto gráfico
    // OpenGL de la ventana que vamos a crear. Por ejemplo, el número de muestras o el
    // modo Core Profile.
    glfwWindowHint ( GLFW_SAMPLES, 4 ); // - Activa antialiasing con 4 muestras.
    glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // - Esta y las 2
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 4 ); // siguientes activan un contexto
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 1 ); // OpenGL Core Profile 4.1.

    // - Definimos el puntero para guardar la dirección de la ventana de la aplicación y
    // la creamos
    GLFWwindow *window;
    float width = 1280;
    float height = 576;

    //Initialize instance
    PAG::Renderer::initialize(width/height);

    //Initialize GUI
    initializeGUI();

    // - Tamaño, título de la ventana, en ventana y no en pantalla completa,
    // sin compartir recursos con otras ventanas.

    window = glfwCreateWindow ( width, height, "PAG Introduction", nullptr, nullptr );

    // - Comprobamos si la creación de la ventana ha tenido éxito.
    if ( window == nullptr )
    {
        PAG::Logger::getInstance()->addMessage("Failed to open GLFW window");
        glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
        return -2;
    }

    // - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent ( window );
    // - Ahora inicializamos GLAD.
    if ( !gladLoadGLLoader ( (GLADloadproc) glfwGetProcAddress ) )
    {
        PAG::Logger::getInstance()->addMessage("GLAD initialization failed");
        glfwDestroyWindow ( window ); // - Liberamos los recursos que ocupaba GLFW.
        window = nullptr;
        glfwTerminate ();
        return -3;
    }

    PAG::Renderer::getInfoGL();

    //Registering all callbacks to respond main events
    glfwSetWindowRefreshCallback(window, window_refresh_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, set_cursor_pos_callback);

    //Initialize imgui
    PAG::ManagerGUI::initialize(window);

    //Initialize OpenGL
    PAG::Renderer::getInstance()->initializeOpenGL();

    while (!glfwWindowShouldClose(window)) {
        //Obtains and organises the remaining events, such as key press,
        //mouse press, etc. Always and the end of each iteration and after
        //the function glfwSwapBuffers(window)
        glfwPollEvents();

        //Initialize imgui controls
        PAG::ManagerGUI::initializeNewFrame();

        //Draw GUI objects
        PAG::ManagerGUI::getInstance()->drawAllWindows();

        //Refresh visualization window
        try {
            PAG::Renderer::getInstance()->refresh();
        }catch (const std::runtime_error& e) {
            PAG::Logger::getInstance()->addMessage(e.what());
        }

        //Render imgui controls
        PAG::ManagerGUI::renderNewFrame();

        //Refresh window
        glfwSwapBuffers(window);
    }
    // - Una vez terminado el ciclo de eventos, liberar recursos, etc.
    PAG::Logger::getInstance()->addMessage("Ending PAG aplication...");

    PAG::ManagerGUI::destroyImGuiObjects();
    glfwDestroyWindow ( window ); // - Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
}