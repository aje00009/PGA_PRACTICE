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

### PRÁCTICA 4

En esta práctica, nos hemos dedicado a la encapsulación de código de la parte de creación, compilación y enlazado del shader en otras
clases para mejorar la cohesión de mi proyecto. En este caso, he realizado una división del trabajo en distintas partes:

* ``ShaderProgram``: se encarga de enlazar los shaders
* ``Shader``: clase padre que heredarán cada uno de los tipos de shader y que se encarga de buscar los archivos, compilar los shaders, comprobar errores y eliminarlos
* ``ShaderLoaderWindow``: clase que representa la ventana para cargar el shader que se quiere renderizar. Se comunica con `Renderer` a través del patrón observador, avisandole cuando el usuario quiera cargar un nuevo shader
* ``Vertex/FragmentShader``: clase que hereda de Shader para procesar un vertex/fragment shader (llamar al constructor de `Shader`) indicando el tipo de shader
* ``Renderer``: ahora esta clase se encargará de guardar, por un lado, todos los shader programs creados en la ejecución de la aplicación en un vector. Así, podremos llevar una cuenta de los shader que el usuario ha creado, cada uno con su respectivo Vertex/Fragment Shader. Por otro lado, en todo momento tendremos el Shader Program activo (si es que hay alguno). También, se mantienen los atributos de idVAO, idVBO, etc. Estos son necesarios para crear el modelo, es decir, para decirle al shader como van a leerse los vertices y de que forman están conectados, etc. Esta función se queda en el shader pues este es el encargado de crear (render) el modelo.

En la imagen de abajo apreciamos el diagrama UML con los cambios comentados. 
![Imagen UML Práctica 4](resources/images/uml_prac4.png)

### PRÁCTICA 5
En esta práctica incluimos la implementación de la camara virtual, de forma que se definan todos sus parámetros correspondientes
y los movimientos posibles de la misma. Para ello, podemos seleccionar desde la ventana de controles de ImGui el movimiento a utilizar. 
Después, podremos utilizar los botones que existe en la propia ventana o bien el botón izquierdo del ratón. En caso del zoom, no tendremos que
seleccionar ningún control, pues este estará siempre disponible si hacemos scroll con el ratón sobre la escena (no funcionará si lo hacemos sobre 
una de las ventanas de ImGui). También he incluido un botón de "reset" de la camara para reestablecer la camara como salía al principio (posición en {0,0,5}, lookAt en {0,0,0} y up {0,1,0})

En la parte de la implementación, se han añadido/modificado las siguientes cosas:
* **Camera**: clase que encapsula todos los datos referentes a la camara virtual (sist. coordenadas, fov, aspecto...) y los movimientos de la misma
* **CameraWindow**: clase que representa la ventana de la interfaz gráfica para los controles de la camara.
* **Renderer**: ahora esta clase tendrá un conjunto de camaras (de momento solo una, pero tendrá varias en el futuro), además de escuchar los eventos de la ventana de controles para implementar el patrón observador también con este aspecto de la aplicación
* **main.cpp**: ahora capturará los eventos de pulsado de ratón y de scroll para mandarlos a renderer para que tenga la información sobre la posición del ratón y que pueda realizar los movimientos correctamente

Los movimientos son los siguientes:
* **Pan**: mueve la posición a la que apunta la cámara de forma horizontal
* **Tilt**: mueve la posición a la que apunta la cámara de forma vertical
* **Dolly**: mueve la posición de la cámara en el eje X y/o Z
* **Orbit**: rota la cámara alrededor del punto al que mira
* **Zoom**: modifica el fov de la cámara para aumentar o disminuir el ángulo de visión de la cámara

Aquí estaría el UML resultante tras los cambios implementados en esta práctica
![Imagen UML Práctica 5](resources/images/uml_prac5.png)

### PRÁCTICA 6
Para esta práctica, he modularizado la gestión de modelos encapsulando un modelo en una nueva clase `Model`. En esta encontramos:
* **_vertices** : vector de vertices que contiene todo el modelo (malla)
* **_indices** : vector de índices que indica la topología del modelo (como se unen los vértices)
* **_textures** : vector de texturas que se usará a futuro para aplicar una textura a un modelo concreto
* **_idVAO** : ID del VAO asociado al modelo
* **_idVBO** : ID del VBO (entrelazado) asociado al modelo
* **_idIBO** : ID del IBO asociado al modelo
* **modelName** : cadena de caracteres del nombre del modelo (nombre.obj)
* **_modelMatrix** : matriz de transformación individual de cada modelo
* **_shaderProgram** : shader program asociado al modelo

Esta clase `Model` se encargará de cargar en memoria un modelo usando la librería `assimp`. También se encargará de 
gestionar los buffer y crear los VAO, VBO, etc. Cada modelo tendrá también la posibilidad de aplicar transformaciones a
la malla que encapsula (`translate`,`rotate`,`scale`).

#### Tipos de datos necesarios:
* `Vertex` : encapsula un vértice y su información relacionada (posición, normal, coordenada de textura, color)
* `Texture` : encapsula la textura y su información relacionada (id, tipo) (Aún no se utiliza)
* `TransformType` : es un enum que define los tipos de transformaciones disponibles para un modelo. Contiene las nombradas anteriormente, además de una opción para resetear el modelo a su matriz original y otro para borrar el modelo.
* `TransformPackage` : es un struct que encapsula un payload mediante el cual la ventana de la interfaz GUI es capaz de mandar el mensaje sobre una transformación a realizar sobre un modelo en concreto a `Renderer`

También he añadido dos nuevas ventanas a la interfaz de usuario:
* `ModelLoaderWindow` : esta ventana nos permite seleccionar un archivo de nuestro directorio y cargar un modelo.
* `ModelTransformationWindow` : esta ventana nos permite seleccionar un modelo previamente cargado en memoria y aplicarle una transformación.

En cuanto a `Renderer`, los cambios son los siguientes:
* Se ha eliminado toda la funcionalidad referente a la gestión de buffers, VAO, VBO, ids, etc.
* Cuando se refresca la escena (`refresh`), se dibujan todos los modelos cargados en memoria en pantalla, pasando cada uno de los uniforms necesarios (matrix de modelado, visión y proyección) al shader program asociado a cada modelo
* Ahora `Renderer` escuchará dos nuevos eventos:
  * De la ventana `ModelLoaderWindow` : para cargar modelos en memoria y añadirlos al vector de modelos.
  * De la ventana `ModelTransformationWindow` : para transformar los modelos cargados y que esta transformación pueda verse reflejada en pantalla.

En cuanto a la funcionalidad de esta nueva parte de la aplicación, es bastante sencilla:
1. Cargamos previamente un shader program de los posibles (para esta práctica: pag06) usando la ventana de carga de shaders.
2. Pinchamos en seleccionar un modelo dentro de la ventana de carga de modelos
3. Seleccionamos un archivo con el formato adecuado (.obj) y le damos a "ok"
4. Si queremos aplicar una transformación (traslación, rotación o escalado):
   * Nos dirigimos a la ventana de transformaciones de modelos.
   * Seleccionamos un modelo.
   * Elegimos el tipo de transformación (traslación, rotación, escalado).
   * Escribimos los parámetros para dicha transformación (posición, factores de escalado, ángulo, etc.).
   * Le damos a aplicar.
5. Si queremos reiniciar la matriz de modelado (resetear el modelo como estaba cuando lo cargamos):
   * Nos dirigimos a la ventana de transformaciones de modelos.
   * Seleccionamos un modelo.
   * Pulsamos el botón de "reset"
6. Si queremos eliminar el modelo:
   * Nos dirigimos a la ventana de transformaciones de modelos.
   * Seleccionamos un modelo.
   * Pulsamos en eliminar el modelo.

Finalmente, el UML actualizado quedaría así:

![Imagen diagrama UML práctica 6](/resources/images/uml_prac6.png)

### PRÁCTICA 7
Para esta práctica, he añadido el uso de subrutinas para controlar el renderizado de modelos mediante el modo de wireframe (color fijo) o el modo de relleno (material), además de la implementación de materiales en la aplicación.
Hay varias cosas añadidas al proyecto:
* `MaterialEditingWindow` en `gui`: esta ventana nueva de la GUI se dedicará a la creación/edición de materiales.
* `ModelManager` en `gui`: esta ventana, antes llamada `ModelTransformationWindow` ha creado más responsabilidades, por ello la he refactorizado. 
Ahora también se encarga de asignar el tipo de material (de los que creamos en la ventana anterior) al modelo seleccionado.
* `Material` en `gui`: nueva clase que encapsula las propiedades de un material (color ambiente, difuso, especular y brillo)
* `Model` en `gui`: para soportar al material, tiene un puntero (asignación) hacia él. 
* `Renderer` en `rendering`: 
  * **Atributos añadidos**:
    * `_renderMode: RenderMode`: nuevo atributo para indicar el modo actual de renderizado (SOLID/WIREFRAME)
    * `_subroutineSolid`: nuevo atributo para guardar el index de la subrutina del modo de renderizado SOLID
    * `subroutineWireframe`: nuevo atributo para guardar el index de la subrutina del modo de renderizado WIREFRAME
    * `_materials: vector<Material>`: nueva colección para guardar todos los materiales que se van creando en la aplicación
  * **Nuevo comportamiento**:
    * `initialize()`: ahora inicializa un material por defecto para que a los modelos se les asignen ese por defecto
    * `wakeUp()`:
      * `ShaderLoad`: al cargar el shader, se piden los index de las subrutinas para tenerlos guardados con antelación a la hora de activarla después
      * `RenderMode`: nuevo evento a escuchar para cambiar el modo de renderizado
      * `ModelEditor`: antes llamado `ModelTransformationWindow`, ahora también tendrá un apartado `MATERIAL_ASSIGN`, donde se asignará el material al modelo tras haber confirmado en la ventana de edición de modelos
      * `MaterialEditor`: evento que escucha `Renderer` para crear/editar un material desde la ventana de `MaterialEditingWindow`.
    * `refresh()`:
      * Ahora hay que comprobar el modo de dibujado cada vez que refresquemos según el `_renderMode`
      * Se pasan los uniforms del material al shader
      * Se activa la subrutina adecuada según `_renderMode`
* **Tipos/structs que se han añadido**:
  * `RenderMode`: para elegir entre el modo de renderizado SOLID (material) o WIREFRAME (color blanco fijo)
  * `MaterialEditingPackage`: payload que se envía a `Renderer` con la información de creación/edición de un material

Para usar estas nuevas funcionalidades, podemos acceder a la ventana de gestión de materiales para crear o modificar un material ya creado. Después (tras cargar el shader y el modelo), asignamos un material que queramos 
al modelo (aunque tiene uno por defecto). Finalmente, podemos elegir ver nuestros modelos en modo SOLID o WIREFRAME.

El UML resultante tras esta práctica es el siguiente:

![Imagen UML Práctica 7](resources/images/uml_prac7.png)

### PRÁCTICA 8
Para esta nueva práctica, se han añadido diferentes funcionalides para el control de luces en la escena. Esto incluye la creación y eliminación de las mismas, así como su renderizado en la escena.
Para lograrlo, se han añadido las siguiente cosas:
* Nuevas EEDD y tipos (`Types.h`):
  * `LightType`: enum para diferenciar el tipo de luz (`AMBIENT_LIGHT`,`DIRECTIONAL_LIGHT`,`SPOT_LIGHT`,`POINT_LIGHT`).
  * `WindowType::LightManager`: nuevo tipo de ventana añadido, el cual hace referencia a la ventana para gestionar las luces.
  * `LightPackage`: estructura de datos para transferir datos de creación de luces desde la GUI al Renderer.
  * `NUM_LIGHTS`: constante representativa del número de luces existentes (4);
* Sistema de iluminación (Aplicando Patrón Estrategia):
  * `Light`: clase "principal" que encapsula la información y comportamiento de una luz en la escena. Tiene varios componentes:
    * `LightProperties`: propiedades asociada a la luz (difusa, ambiente, posicion, dirección, etc.), en concreto a cualquier tipo de luz.
    * `LightApplicator [NUM_LIGHTS]`: vector asociado a una luz (estático para todas las instancias de luces) con el cual se aplica el Patrón Estrategia. De esta forma, cada posición en el vector es un aplicador de luz diferente, asociado al enum `LightType`. Cada uno enviará al shader program unos uniforms u otros dependiendo del tipo de luz:
      * `AmbientLightApplicator`
      * `PointLightApplicator`
      * `DirectionalLightApplicator`
      * `SpotLightApplicator`
      * En el vector se crean instancias que son implementaciones de la clase abstracta (`LightApplicator`) y hacen override del método `applyLight()`, donde cada tipo de luz lo hará de diferente forma.
* Shaders:
  * 

### MANUAL DE USUARIO