#include <iostream>
#include <cstdlib>
#include <ctime>

// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>
#include <vector>

std::vector<std::vector<float>> colors = {{1,0,0,1},{1,1,0,1},{0,0.5,0.3},{0.3,0.5,0.8},{1,0,1}};

//Esta función callback será llamada cuando GLFW produzca algún error
void error_callback(int error, const char* description) {
    std::string aux(description);
    std::cout << "Error de GLFW número " << error << ": " << aux << std::endl;
}

//Esta función será llamada cada vez que el área de dibujo de OpenGL
//deba ser redibujada
void window_refresh_callback(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
    // intercambia el buffer back (que se ha estado dibujando) por el
    // que se mostraba hasta ahora front. Debe ser la última orden de
    // este callback
    glfwSwapBuffers(window);

    std::cout << "Refresh callback called" << std::endl;
}

// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    std::cout << "Resize callback called" << std::endl;
}

// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    std::cout << "Key callback called" << std::endl;
}

//Esta función callback será llamada caca vez que sea pulse algun botón
//del ratón sobre el área de dibujo OpenGL
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS)
        std::cout << "Button pressed: " << button << std::endl;
    else if (action == GLFW_RELEASE)
        std::cout << "Button released: " << button << std::endl;
}

std::vector<float> obtain_random_color() {
    int index = rand() % colors.size();
    return colors[index];
}

//Esta función callback será llamada cada vez que se mueva la rueda
//del ratón sobre el área de dibujo OpenGL
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    std::cout << "Scrolled " << xoffset << " units horizontally and "
    << yoffset << " units vertically" << std::endl;

    std::vector<float> rand_color = obtain_random_color();

    glClearColor(rand_color[0],rand_color[1],rand_color[2],1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);
}

int main(){
    srand(time(NULL));

    std::cout << "Starting Application PAG - Prueba 01" << std::endl;

    //GLFW Errors callback before executing any GLFW functions
    glfwSetErrorCallback((GLFWerrorfun)error_callback);
    // - Inicializa GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
    if ( glfwInit () != GLFW_TRUE )
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
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

    // - Tamaño, título de la ventana, en ventana y no en pantalla completa,
    // sin compartir recursos con otras ventanas.

    window = glfwCreateWindow ( 1024, 576, "PAG Introduction", nullptr, nullptr );

    // - Comprobamos si la creación de la ventana ha tenido éxito.
    if ( window == nullptr )
    {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
        return -2;
    }

    // - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent ( window );
    // - Ahora inicializamos GLAD.
    if ( !gladLoadGLLoader ( (GLADloadproc) glfwGetProcAddress ) )
    {
        std::cout << "GLAD initialization failed" << std::endl;
        glfwDestroyWindow ( window ); // - Liberamos los recursos que ocupaba GLFW.
        window = nullptr;
        glfwTerminate ();
        return -3;
    }

    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
    // 3D construido.
    std::cout << glGetString ( GL_RENDERER ) << std::endl
        << glGetString ( GL_VENDOR ) << std::endl << glGetString ( GL_VERSION ) << std::endl
        << glGetString ( GL_SHADING_LANGUAGE_VERSION ) << std::endl;
        // - Establecemos un gris medio como color con el que se borrará el frame buffer.
        // No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
        glClearColor ( 0.6, 0.6, 0.6, 1.0 );
        // - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
        // No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
        glEnable ( GL_DEPTH_TEST );

        //Registering all callbacks to respond main events
        glfwSetWindowRefreshCallback(window, window_refresh_callback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetScrollCallback(window, scroll_callback);

        while (!glfwWindowShouldClose(window)) {
            //Obtains and organises the remaining events, such as key press,
            //mouse press, etc. Always and the end of each iteration and after
            //the function glfwSwapBuffers(window)
            glfwPollEvents();
        }
        // - Una vez terminado el ciclo de eventos, liberar recursos, etc.
        std::cout << "Finishing application pag prueba" << std::endl;
        glfwDestroyWindow ( window ); // - Cerramos y destruimos la ventana de la aplicación.
        window = nullptr;
        glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
}
