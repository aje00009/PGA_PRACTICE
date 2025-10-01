# PRÁCTICAS PAG

_Autor: Alberto Jiménez Expósito_

_Email: aje00009@red.ujaen.es_

[Enlace a GitHub](https://github.com/aje00009/PGA_PRACTICE)

### PRÁCTICA 1

* Ejercicio 2: para poder solucionar el problema que se plantea, deberíamos usar una clase que encapsule y actúe de arbitro 
entre el resto de clases de nuestra aplicación gráfica. La llamaremos ``App``. Esta clase deberá contener una instancia de `Renderer`. Dentro de `App` podemos definir métodos
estáticos, de forma que se asemejen a una función normal en C++. Para acceder al ``Renderer`` desde cualquier parte del código, debemos mantener
una instancia ``static App* punteroApp`` que sirva de puntero a la instancia ÚNICA de la aplicación. Nos quedaría lo siguiente:

```
 namespace PAG {
    class App {
        Renderer renderer;
        static App* punteroApp;
        
        ...
        
        static void refrescar_ventana_callback()
            punteroApp->renderer.refrescarVentana();
        
        ...
    }
 }
```

Si queremos visualizarlo de forma visual, tenemos el siguiente UML:

![Imagen UML Práctica 1 Ejercicio 2](resources/images/uml_prac1_ejer2.png)

Sin meternos en detalles de atributos que pudieran llegar a tener cada clase, observamos que se trata
de, simplemente, una relación de composición, en la que ``App`` usa a `Renderer` para refrescar la ventana. Es 
una relación de composición ya que si la aplicación no existe, el renderer tampoco debería de existir por sí solo.

### PRÁCTICA 2

En esta práctica encapsularemos tanto las ordenes relacionadas con OpenGL como las de la librería ImGui. 
Para ello, crearemos las clases ``Renderer`` y `GUI` dentro de un nombre de espacios (`PAG`) para mejorar nuestro código y 
el programa en sí.

Tras ello, el diagrama UML resultante es el siguiente:

![Imagen UML Práctica 2](resources/images/uml_prac2.png)

De esta forma hemos encapsulado la funcionalidad de renderizar la escena y sus callbacks adheridos (y futuros modelos, nubes de puntos...) a
la clase ``PAG::Renderer``; la funcionalidad de diseñar y mostrar la interfaz de usuario (ventanas modales, popups, menús, etc.) a la clase
``PAG::GUI``; y la funcionalidad de almacenar los mensajes de log (errores, mensajes de aviso, eventos, etc.) a la clase `PAG::Logger`.

### PRÁCTICA 3
En esta práctica nos hemos dedicado a crear los shader para poder procesar distintos datos (vértices, colores, etc.) para poder
dibujar diferentes objetos y formas a un nivel bajo.

Aquí estaría el UML resultante. He añadido el patrón observador para el ``Renderer``, de forma que este escuche de los eventos para
cambiar diferentes aspectos de la ventana de visualización durante la ejecución de la aplicación (color de fondo). Además,
he implementado una relación de herencia para diferenciar los distintos objetos (ventanas, controles...) en la cual tenemos una interfaz
``GUIElement`` que implementarán distintas ventanas y/o controles (`BgWindow`, `LoggerWindow`). Finalmente, tendré un gestor de la interfaz (`ManagerGUI`),
que se encargará de gestionar la inicialización y destrucción de objetos de ImGui y de manejar las ventanas que existen en la aplicación.

![Imagen UML Práctica 3](resources/images/uml_prac3.png)