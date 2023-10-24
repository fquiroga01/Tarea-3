*Guía ejecución programa Tarea 3*

*Importante*
Al copiar el repositorio estarán todos los libros en una carpeta, el problema es que cargarlos todos se demora demasiado, es altamente recomendable borrar varios de estos libros y trabajar con un tamaño reducido para hacer pruebas. En caso de que se usen todos los libros, algunas funciones van a tardar entre bastante y mucho en cargar, por ejemplo, la funcion 1) se va a tomar muchísimo tiempo (sobre 1 minuto), mientras que por ejemplo la 5) va a tardar casi 1 minuto.
Ambas dan la impresión de que el programa no funciona, pero si funciona, solo se toma mucho tiempo.

1) Para comenzar a utilizar el programa hay que ingresar el número 1, para luego ingresar como entrada "Libros", con esto vamos a cargar todos los libros y a su vez crea el mapa de palabras con su frecuencia, el cual es recorrido en varias otras funciones. Cuando se ingrese la palabra, hay que esperar un tiempo considerable de tiempo para que termine de cargar todos los libros, debido a esto, hay dos carpetas de libros, una con los 100 libros (la cual se tardará un tiempo monumental para cargar todos los libros), y una carpeta con menos libros. Al usar la segunda carpeta debería tardar unos segundos en cargarlo todo.

2) En esta función basta con ingresar el 2 y el programa mostrará los libros ordenados alfabéticamente con sus respectivos datos.

   *NOTA* : SI SE VA A PROBAR EL RESTO DE FUNCIONES, ESTA FUNCIÓN HACE QUE SE CAMBIE EL ORDEN DE LOS ID, ASÍ QUE EN ESE CASO HAY QUE OMITIR ESTA FUNCIÓN (error nombrado más abajo).

3) Esta función muestra automáticamente el título del libro dándole una palabra que está contenida en él. Es importante respetar las mayúsculas, por ejemplo, ingresar "Alice" y no "alice".

4) Esta opción muestra las 10 palabras con mayor frecuencia no ordenadas que se encuentran al recorrer el mapa de principio a fin ignorando las que están en la lista negra, esta función también se toma su tiempo en funcionar, aunque no tanto como la 1). Recordar que la función mostrará las palabras de un libro equivocado si se usa la función 2) antes.

5) Esta también se va a demorar un poco. Ingresando el id del libro, la función muestra las 10 palabras de mayor relevancia ordenadas de mayor a menor, con la mayor en la cima de la lista.

6) Para usar esta función hay que ingresar una palabra completamente en minúsculas, por ejemplo "alice" y no "Alice". (Otro caso de prueba puede ser ingresar "the"). El resultado son todos los libros en los que se encuentra la palabra que se busca.

7) Para usar esta función nuevamente hay que ingresar el id del libro y luego la palabra en minúculas, por ejemplo, ingresar "israel". La salida son 10 palabras hacia atrás y 10 hacia adelante.

*ERRORES ENCONTRADOS:*

- Para comenzar, el principal "error" del programa es la cantidad de tiempo que se tarda en cargar los libros, es totalmente soportable al trabajar con pocos libros, pero no al trabajar con 100 de tamaño considerable de palabras y caracteres.

- Otro error es el descrito previamente en la guía de uso. Al usar la segunda función se ordenan los libros alfabeticamente, sin embargo, el orden en el que se quedan los libros luego de llamar a la función hace que se cambien los id respetando ese orden. Esto provoca que al llamar a las otras funciones ingresándo un id, vaya a mostrar los datos del libro de otro id, es decir, muestra datos correspondientes a otro libro.

- Tanto en la función 5 como en la 7, la tarea especifica que el ingreso debe ser del título del libro para hacer lo que pide cada punto, sin embargo, no pudimos hacer que el programa reconozca el título del libro en cuestión, debido a esto, hicimos que el programa pida en su reemplazo el id del libro que se quiere ocupar. De todos modos el resultado de la función es el esperado.

- No estamos muy seguros de si esto es un "error", pero el nombre del primer libro en la carpeta de libros es "10.txt", ¿puede que el "10" sea el verdadero ID del libro?.

Contribuciones:

Fracisco Quiroga (3/3):
- Diseño de la solución inicial.
- Diseño de las funciones 1, 3, 5, 7, main.
- Investigación y modificación de uso de dirent.h para cargar archivos dentro del entorno de trabajo.

Felipe Nuñez (3/3):
- Corrección de errores de contadores para la función 2), la cual influye en varias de las otras funciones.
- Diseño de las funciones 2, 4, 6.

El archivo README fue hecho por ambos.
