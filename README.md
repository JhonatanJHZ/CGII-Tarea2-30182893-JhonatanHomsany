# CGII-Tarea2-30182893-Jhonatan Homsany

## Índice

1. [Introducción](#introducción)
2. [Guía de compilación](#guía-de-compilación)
3. [Librerías utilizadas](#librerías-utilizadas)
4. [Requerimientos implementados](#requerimientos-implementados)
5. [Guía de uso](#guía-de-uso)

   5.1 [Controles](#controles)

   5.2 [Interfaz Gráfica de Usuario (GUI)](#interfaz-gráfica-de-usuario-gui)

6. [Buenas prácticas llevadas a cabo en el desarrollo](#buenas-prácticas-llevadas-a-cabo-en-el-desarrollo)
7. [Dificultades encontradas](#dificultades-encontradas)
8. [Funcionamiento del programa](#funcionamiento-del-programa)

---

## Introducción.

El presente documento constituye la guía técnica y operativa del sistema desarrollado para la cátedra de Tópicos Avanzados en Computación Gráfica en la Escuela de Computación de la Facultad de Ciencias de la Universidad Central de Venezuela.

El proyecto consiste en un renderizador de volúmenes generado a partir de los conocimientos impartidos a lo largo del curso. El programa permite modificar la opacidad de los materiales en el volúmen con el fin de poder ver a través de ellos y no únicamente su superficie. Además, cuenta con funcionalidades que permiten la inserción de objetos, manipulación de sus características, el modo Stoneburner, entre otros requerimientos solicitados en el documento de la asignación.

## Guía de compilación.

Este proyecto fue realizado y probado utilizando Visual Studio 2026. Para poder ejecutarlo, es necesario tener instalado MSVC - v143 - VS 2022 C++ x64/x86 build tools (v14.44 - 17.14) como herramienta de construcción de proyectos para así poder compilarlo y ejecutarlo correctamente.

Adicionalmente, la configuración de ejecución se ha establecido en modo $Release$ para plataformas $x64$.

Una vez establecidas las configuraciones, basta con utilizar el depurador local de Windows en Visual Studio 2022 para ejecutar el proyecto.

Este proyecto también se encuentra disponible en la plataforma GitHub a través del siguiente enlace: https://github.com/JhonatanJHZ/CGII-Tarea2-30182893-JhonatanHomsany

## Librerías utilizadas.

Ante la ausencia de un manejador de paquetes para programas hechos en C++, el proyecto fue realizado utilizando la configuración base de CMake definida por Visual Studio. Luego, a partir de CMake, se instalaron las siguientes librerías necesarias para la realización del proyecto:

- Glad.
- GLFW.
- GLM.
- ImGUI.
- TinyGLTF.

Adicionalmente, fue incluída la implementación de TinyFileDialog para acudir al explorador de archivos del sistema operativo del computador a la hora de ubicar las rutas de los archivos GLB/GLTF que se usarán en la escena. De esta manera, se evitó que el usuario deba escribir manualmente la ruta donde está o donde quiere guardar su archivo.

TinyFileDialogs es una librería de código abierto desarrollada por el usuario Guillaume Vareille y fue publicada en el 2017. Se puede encontrar información adicional de la librería en la dirección https://github.com/native-toolkit/libtinyfiledialogs/blob/master/tinyfiledialogs.c

## Requerimientos implementados.

El presente proyecto contiene la implementación completa de los requerimientos obligatorios solicitados en el documento enviado por el grupo docente. Adicionalmente, se implementaron los siguientes requisitos opcionales:

- Escalar un objeto dentro del volumen mundo.

- Generar escena base proceduralmente

A pesar de no formar parte de los requerimientos, también se agregó lo siguiente:

- Limitar de tasa de frames (sincronizado vertical): A medida que se llevó a cabo el desarrollo y al incluir el contador de FPS, se notó un comportamiento donde la tasa de frames contenía un valor considerablemente grande. En aras de reducir el esfuerzo de la GPU y mantener un rendimiento más estable y coherente en todos los computadores, se implementó la posibilidad de limitar la tasa de frames por decisión del usuario.

- Modo Stoneburner: Es posible activar una funcionalidad a través de UI para que la perforación del volúmen sea en la dirección del rayo y no en la dirección ortogonal al plano XZ (Ver imágenes en funcionamiento del programa). Mantener esta función desactivada mientras se activa el modo Stoneburner genera los cortes en la dirección requerida por el PDF.

## Guía de uso.

El programa cuenta con los controles básicos en la interfaz de ImGUI para ayudar al usuario a controlar el programa en tiempo de ejecución sin tener que cambiar entre la pestaña de esta documentación y la aplicación. Sin embargo, a continuación se describen en mayor detalle las funcionalidades de la aplicación.

### Controles.

Los movimientos de cámara se implementaron tomando gran inspiración en el manejo de cámara en videojuegos de estilo first-person shooter (FPS) o Minecraft (en modo creativo). Se tomó inspiración para utilizar aspectos como el uso de WASD para el movimiento en el plano horizontal, SPACE para subir, LSHIFT para bajar.

Para controlar la dirección a la que apunta la cámara, el usuario puede mantener presionado el click derecho o la tecla Z mientras mueve su mouse para así ver a su alrededor.

Para aumentar o disminuir el field of view del programa (FOV), es posible utilizar la rueda del mouse.

Esto describe los controles utilizados con entradas de teclado y ratón. A continuación, veamos los controles asociados a la interfaz gráfica de usuario (GUI).

### Interfaz Gráfica de Usuario (GUI).

En la GUI, el usuario cuenta con lo siguiente:

- Checkbox para la límitar la tasa de frames (sincronizado vertical).

- Cuadro de texto a modo informativo con los controles de movimiento del usuario.

- Manejo de archivos: - Dimensiones del volumen: En este apartado el usuario debe ingresar las dimensiones del volumen que desea cargar. Este caso aplica únicamente cuando el usuario quiere cargar un volumen en formato .raw ya que en formato .nrrd, dicha información se encuentra presente y es extraída del archivo.

      - Botones activadores del explorador de archivos del sistema para que el usuario pueda indicar de manera cómoda la ubicación del archivo que quiere guardar/cargar.

- Función de transferencia:

      - Controles por material: El usuario posee diferentes deslizadores o campos para la inserción de valores que le permiten modificar la opacidad de lo que se muestra en pantalla por cada material. Por cada material, tiene un slider general que permite modificar todos los objetos que se encuentran dentro del rango alpha especificado en el documento de la asignación, además, se cuenta con dos campos de inserción numérica para que así, el usuario pueda delimitar objetos de un mismo material. En otras palabras, el usuario puede seleccionar si solamente quiere ver los gases cuya opacidad se encuentra en el intervalo [1.0, 20.0].

      - Rango de densidad: Al igual que la opacidad por material, la GUI contiene controles para delimitar el rango de densidad que se debe mostrar.

- Controles de los vóxeles:

      - Tamaño de los vóxeles: En este apartado, el usuario puede modificar el tamaño de los vóxeles del volumen. Con esto, el usuario puede manipular el porcentaje de dibujado del volumen que quiere observar. Valores más altos permiten tener menos detalles en los vóxeles. (Ver imágenes en funcionamiento del programa).

- Gizmo: Esta sección permite activar o desactivar el bounding box y los ejes en la escena (Ver imágenes en funcionamiento del programa).

- Inserción de objetos:

      - Profundidad: El usuario cuenta con una lista de profundidades en bits que puede contener el objeto que desea ingresar.

      - Dimensiones: El usuario debe ingresar las dimensiones del objeto tal y como hizo al cargar la escena inicial. Del mismo modo, esta funcionalidad solo es necesaria para archivos .raw.

      - Posición: El usuario puede modificar la posición que tendrá el objeto en el volumen una vez sea ingresado.

      - Escala: El usuario puede ingresar la escala del objeto en sus componentes XYZ.

      - Color: Un color picker para que el usuario indique de manera visual las componentes RGB del objeto.

- Modificación de objetos insertados: Este apartado se habilita únicamente cuando el usuario ha ingresado un objeto a la escena.

      - Objeto #X: El usuario posee una lista de los objetos que han sido ingresados en la escena y tiene la posibilidad de seleccionarlos para luego modificar atributos tales como, posición, escala y color. Tras hacer una modificación del objeto, es necesario presionar el botón "Modificar objeto" para poder visualizar los cambios (veáse el apartado de dificultades encontradas). El usuario también cuenta con la posibilidad de eliminar objetos.

- Stoneburner:

      - Checkbox para activar/desactivar el modo Stoneburner.

      - Checkbox para que el modo Stoneburner siga la dirección del rayo o mantenga la destrucción en la dirección ortogonal al plano XZ.

      - Deslizador para el umbral de selección. Este slider permite indicar cual debe ser el alpha que debe poseer un material para poder ser el centro del cilíndro del modo Stoneburner.

      - El usuario cuenta con la posibilidad de hacer click en un lugar del volumen y realizar la destrucción con el modo stoneburner activado. Sin embargo, cuenta con controles via UI que le permiten modificar el centro del cilíndro destructor.

      - Campo para ingresar el radio del cilíndro.

      - Inserción del intervalo para los valores de alfa que deben ser destruidos por el modo stoneburner.

      - Un botón de ejecución manual en caso de ingresar el centro via GUI.

- Generar escena:

      - Botón que permite generar una escena aleatoria de manera procedural.

## Buenas prácticas llevadas a cabo en el desarrollo.

El presente proyecto basa su implementación en el uso de buenas prácticas de la Ingeniería de Software. A pesar de tener un lenguaje que puede ser restrictivo para ciertas cosas, el diseño del algoritmo está basado en el principio SOLID. Particularmente, el desarrollo hizo énfasis en aprovechar el principio de responsabilidad única, el principio open/closed y la inyección de dependencias para lograr un código escalable y con una mantenibilidad sencilla. El uso de estos principios es posible gracias a la programación orientada a objetos, donde se aprovecha el concepto de polimorfismo y encapsulación.

## Dificultades encontradas.

1. Inserción de objetos: La inserción de objetos en el volumen se realiza reemplazando los vóxeles correspondientes del mundo existente por los del nuevo objeto. Esto evita inconsistencias en las dimensiones del archivo modificado, garantizando su correcta lectura y guardado.

2. Manipulación de objetos: Para la manipulación, el sistema almacena el volumen cargado originalmente y mantiene un vector con los objetos insertados. Cuando el usuario realiza una modificación, se actualiza el objeto correspondiente y se regenera la escena a partir del mundo original, reinsertando todos los objetos con sus parámetros actualizados.

El motivo principal de esta implementación es resolver los escenarios que surgen al modificar la escala de un objeto insertado:

- 2.1: El usuario aumenta la escala del objeto: No representa una complicación mayor, ya que únicamente implica sobrescribir los vóxeles del mundo original según las nuevas dimensiones.

- 2.2: El usuario disminuye la escala del objeto: Se presenta el problema de que los vóxeles del mundo original ya fueron sobrescritos en la inserción inicial. Por lo tanto, al reducir la escala, se pierde la información previa de los vóxeles que ocupaban ese espacio.

Por esta razón, al cargar un volumen se conserva siempre una copia inmutable del volumen original. Así, modificar un objeto consiste en tomar dicho volumen base e reinsertar los objetos en su orden cronológico con sus respectivos cambios. De esta manera, y de forma transparente para el usuario, se simula que los nuevos atributos fueron aplicados desde el inicio.

3. Modo Stoneburner, dirección del rayo: Al procesar la destrucción de vóxeles en la dirección del rayo, la operación genera una breve latencia de cálculo durante el procesamiento. Una vez finalizada la destrucción, el programa continúa su ejecución con normalidad.

## Funcionamiento del programa.

<img width="1918" height="1137" alt="image" src="https://github.com/user-attachments/assets/3cd984c5-daf7-461f-9fd7-9157abf37146" />

<p align="center">
  <i><b>Fig. 1.</b> Escena generada manualmente y renderizada con rasterización.</i>
</p>

<img width="1918" height="1133" alt="image" src="https://github.com/user-attachments/assets/bfafc5d8-622d-4770-b5c1-8ca50d3bd668" />

<p align="center">
  <i><b>Fig. 2.</b> Escena generada manualmente y renderizada con Ray Tracing.</i>
</p>
