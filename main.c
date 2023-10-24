#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <math.h>
#include <ctype.h>

#define MAX_LIBROS 100
#define MAX_LONGITUD_NOMBRE 256
#define MAX_LONGITUD_TITULO 512
#define MAX_PALABRAS 50000

struct Libro 
{
    char titulo[MAX_LONGITUD_TITULO];
    int id;
    int num_palabras;
    int num_caracteres;
    char **palabras;
    int *frecuencia; //Dos arreglos dinámicos para almacenar las palabras y la frecuencia.
};

struct PalabraRelevante 
{
    char palabra[MAX_LONGITUD_TITULO];
    double relevancia;
};

//Función que lee el archivo con el uso de dirent.h obtenida de stackoverflow.com para que trabaje con la carpeta de archivos incluida en el programa y modificada para que trabaje con los textos y almacene los datos necesitados dentro un mapa de palabras.
void cargarDocumentos(struct Libro libros[], int *cantidadLibros) 
{
    char rutaCarpeta[MAX_LONGITUD_NOMBRE];
    printf("Ingrese la ubicación de la carpeta de libros: ");
    scanf("%s", rutaCarpeta);

    DIR *dir;
    struct dirent *entry;

    // Abre el directorio especificado por el usuario.
    dir = opendir(rutaCarpeta);

    if (!dir) 
    {
        perror("Error al abrir el directorio");
        return;
    }

    char rutaArchivo[MAX_LONGITUD_NOMBRE];
    char buffer[MAX_LONGITUD_TITULO];
    FILE *archivo;
    int id = 1;

    //Itera a través de los archivos dentro de la carpeta.
    while ((entry = readdir(dir)) != NULL && *cantidadLibros < MAX_LIBROS) 
    {
        if (entry->d_type == DT_REG) 
        {
            //Construye al ruta completa del archivo.
            snprintf(rutaArchivo, sizeof(rutaArchivo), "%s/%s", rutaCarpeta, entry->d_name);
            //Ahora abre el archivo.
            archivo = fopen(rutaArchivo, "r");

            if (archivo == NULL) 
            {
                perror("Error al abrir el archivo");
                continue;
            }

            //Lee el título del libro.
            if (fgets(buffer, sizeof(buffer), archivo) != NULL) 
            {
                //Copia el título del libro.
                strcpy(libros[*cantidadLibros].titulo, buffer);
                libros[*cantidadLibros].id = id;
                libros[*cantidadLibros].num_palabras = 0;
                libros[*cantidadLibros].num_caracteres = 0;

                //Asignamos memoria dinámica para almacenar las palabras y sus frecuencias.
                libros[*cantidadLibros].palabras = (char **)malloc(MAX_PALABRAS * sizeof(char *));
                libros[*cantidadLibros].frecuencia = (int *)malloc(MAX_PALABRAS * sizeof(int));

                char *palabra;
                //Vamos leyendo las palabras dentro del archivo.
                while (fscanf(archivo, "%s", buffer) == 1) 
                {
                    //Vamos normalizando las palabras.
                    for (int i = 0; i < strlen(buffer); i++) 
                    {
                        if (!isalpha(buffer[i])) {
                            for (int j = i; j < strlen(buffer); j++) 
                            {
                                buffer[j] = buffer[j + 1];
                            }
                            i--;
                        } 
                        else 
                        {
                            buffer[i] = tolower(buffer[i]);
                        }
                    }

                    int existe = 0;
                    //Verificamos si la palabra ya existe.
                    for (int i = 0; i < libros[*cantidadLibros].num_palabras; i++) 
                    {
                        if (strcmp(buffer, libros[*cantidadLibros].palabras[i]) == 0) 
                        {
                            libros[*cantidadLibros].frecuencia[i]++;
                            existe = 1;
                            break;
                        }
                    }

                    if (!existe) 
                    {
                        //Si no existe la palabra, asignamos memoria dinámica y la agregamos.
                        libros[*cantidadLibros].palabras[libros[*cantidadLibros].num_palabras] = (char *)malloc(strlen(buffer) + 1);
                        strcpy(libros[*cantidadLibros].palabras[libros[*cantidadLibros].num_palabras], buffer);
                        libros[*cantidadLibros].frecuencia[libros[*cantidadLibros].num_palabras] = 1;
                        libros[*cantidadLibros].num_palabras++;
                    }

                    libros[*cantidadLibros].num_caracteres += strlen(buffer);
                }

                (*cantidadLibros)++;
                id++;
            }

            fclose(archivo);
        }
    }

    closedir(dir);
}

void mostrarDocumentosOrdenados(struct Libro libros[], int cantidadLibros) 
  {
    if (cantidadLibros == 0) 
    {
        printf("No se han cargado libros.\n");
        return;
    }

    //Ordenamos los títulos de los libros mediante selection sort, intercambiando cuando sea necesario.
    for (int i = 0; i < cantidadLibros - 1; i++) 
    {
        for (int j = i + 1; j < cantidadLibros; j++) 
        {
            if (strcmp(libros[i].titulo, libros[j].titulo) > 0) 
            {
                struct Libro temp = libros[i];
                libros[i] = libros[j];
                libros[j] = temp;
            }
        }
    }

    printf("Libros ordenados por título:\n");
    for (int i = 0; i < cantidadLibros; i++) 
    {
        printf("ID: %d\n", libros[i].id);
        printf("Título: %s", libros[i].titulo);
        printf("Número de palabras distintas: %d\n", libros[i].num_palabras);
        printf("Número de caracteres: %d\n", libros[i].num_caracteres);
        printf("\n");
    }
}

//Usamos strstr para buscar si la cadena "busqueda" está contenida en el título del libro.
//La función devuelve un puntero al primer carácter de la primera aparición de búsqueda en el título.
void buscarLibroPorTitulo(struct Libro libros[], int cantidadLibros, const char *busqueda) 
{
    int encontrado = 0;
    for (int i = 0; i < cantidadLibros; i++) 
    {
        if (strstr(libros[i].titulo, busqueda)) 
        {
            printf("ID: %d\n", libros[i].id);
            printf("Título: %s", libros[i].titulo);
            printf("\n");
            encontrado = 1;
        }
    }

    if (!encontrado) 
    {
        printf("No se encontraron libros que coincidan con la búsqueda.\n");
    }
}

void palabrasConMayorFrecuencia(struct Libro libros[], int cantidadLibros, int libroID) 
{
    if (libroID < 1 || libroID > cantidadLibros) 
    {
        printf("ID de libro no válido.\n");
        return;
    }

    int indiceLibro = libroID - 1;

    printf("Las 10 palabras de mayor frecuencia en el libro \"%s\" (ID: %d) son:\n", libros[indiceLibro].titulo, libros[indiceLibro].id);

    int numPalabras = libros[indiceLibro].num_palabras;
    char **palabras = libros[indiceLibro].palabras;
    int *frecuencia = libros[indiceLibro].frecuencia;

    //Creamos la lista negra para omitir ciertas palabras.
    const char *listaNegra[] = 
    {
        "the", "and", "in", "of", "a", "to", "for", "with", "on", "as", "an", "by", "at", "or", "from", "is", "this", "that", "which", "but", "have", "not", "all", "are", "were", "be", "are", "being", "been", "become", "ebook", "united", "states", "no"
    };

    int palabrasMostradas = 0;

    //Recorremos el mapa de palabras y vamos almacenando las palabras con mayor frecuencia, sustituyéndolas cuando sea necesario.
    //Para cada casilla del mapa verificamos si la palabra está en la lista negra, si está se omite, si no está se sigue ejecutando.
    for (int i = 0; i < numPalabras; i++) 
    {
        int esPalabraComun = 0;
        for (int j = 0; j < sizeof(listaNegra) / sizeof(listaNegra[0]); j++) 
        {
            if (strcmp(palabras[i], listaNegra[j]) == 0) 
            {
                esPalabraComun = 1;
                break;
            }
        }

        if (esPalabraComun) 
        {
            continue;
        }

        if (palabrasMostradas < 10) 
        {
            //Imprimimos las palabras que quedan.
            printf("%s - Frecuencia: %d\n", palabras[i], frecuencia[i]);
            palabrasMostradas++;
        }
    }
}

void palabrasMasRelevantes(struct Libro libros[], int cantidadLibros, int libroID) 
{
    if (libroID < 1 || libroID > cantidadLibros) 
    {
        printf("ID de libro no válido.\n");
        return;
    }

    int indiceLibro = libroID - 1;
    printf("Las 10 palabras más relevantes en el libro \"%s\" (ID: %d) son:\n", libros[indiceLibro].titulo, libros[indiceLibro].id);

    int numPalabras = libros[indiceLibro].num_palabras;
    char **palabras = libros[indiceLibro].palabras;
    int *frecuencia = libros[indiceLibro].frecuencia;

    //Inicializamos arreglos para TF-IDF y conteo de documentos con cada palabra.
    double tfidf[MAX_PALABRAS] = {0.0};
    int numDocumentosConPalabra[MAX_PALABRAS] = {0};

    //Calculamos cuántos documentos contienen cada palabra en el conjunto de libros.
    for (int i = 0; i < cantidadLibros; i++) 
    {
        for (int j = 0; j < libros[i].num_palabras; j++) 
        {
            for (int k = 0; k < numPalabras; k++) 
            {
                if (strcmp(palabras[k], libros[i].palabras[j]) == 0) 
                {
                    numDocumentosConPalabra[k]++;
                }
            }
        }
    }

    //Calculamos el valor TF-IDF para cada palabra en el libro específico.
    for (int i = 0; i < numPalabras; i++) 
    {
        tfidf[i] = (double)frecuencia[i] / libros[indiceLibro].num_palabras * log((double)cantidadLibros / numDocumentosConPalabra[i]);
    }

    //Mostramos las 10 palabras con el valor más alto.
    for (int i = 0; i < 10; i++) 
    {
        int maxIndex = -1;
        double maxTFIDF = -1.0;

        for (int j = 0; j < numPalabras; j++) 
        {
            if (tfidf[j] > maxTFIDF) 
            {
                maxTFIDF = tfidf[j];
                maxIndex = j;
            }
        }

        if (maxIndex != -1) 
        {
            printf("%s - TF-IDF: %.4f\n", palabras[maxIndex], tfidf[maxIndex]);
            tfidf[maxIndex] = -1.0;
        }
    }
}

void buscarLibrosPorPalabra(struct Libro libros[], int cantidadLibros, const char *palabraBuscada) 
{
    //Definimos una estructura para almacenar los resultados de la búsqueda.
    struct ResultadoBusqueda {
        int id;
        char titulo[MAX_LONGITUD_TITULO];
    };

    //Arreglo para almacenar resultados.
    struct ResultadoBusqueda resultados[MAX_LIBROS];
    int numResultados = 0;

    //Recorremos todos los libros y sus palabras.
    for (int i = 0; i < cantidadLibros; i++) 
    {
        for (int j = 0; j < libros[i].num_palabras; j++) 
        {
            //Comparamos la palabra buscada con la del libro, si se encuentra, almacenamos ID y título y acabamos ciclo.
            if (strcmp(libros[i].palabras[j], palabraBuscada) == 0) 
            {
                resultados[numResultados].id = libros[i].id;
                strcpy(resultados[numResultados].titulo, libros[i].titulo);
                numResultados++;
                break;
            }
        }
    }

    if (numResultados > 0) 
    {
        printf("Libros que contienen la palabra '%s' en su contenido:\n", palabraBuscada);
        for (int i = 0; i < numResultados; i++) 
        {
            printf("ID: %d, Título: %s\n", resultados[i].id, resultados[i].titulo);
        }
    } 
    else 
    {
        printf("No se encontraron libros que contengan la palabra '%s' en su contenido.\n", palabraBuscada);
    }
}

void mostrarPalabraEnContexto(struct Libro libros[], int cantidadLibros, int libroID, const char *palabraBuscada) 
{
    if (libroID < 1 || libroID > cantidadLibros) 
    {
        printf("ID de libro no válido.\n");
        return;
    }

    int indiceLibro = libroID - 1;

    //Encabezado con información del libro y la palabra que buscamos obtener el contexto.
    printf("Apariciones de la palabra \"%s\" en el libro \"%s\" (ID: %d) junto con su contexto:\n", palabraBuscada, libros[indiceLibro].titulo, libros[indiceLibro].id);

    char **palabras = libros[indiceLibro].palabras;

    for (int i = 0; i < libros[indiceLibro].num_palabras; i++) 
    {
        if (strcmp(palabras[i], palabraBuscada) == 0) 
        {
            int contexto = 5;

            int inicio = (i - contexto) > 0 ? (i - contexto) : 0;
            int fin = (i + contexto + 1) < libros[indiceLibro].num_palabras ? (i + contexto + 1) : libros[indiceLibro].num_palabras;

            printf("Contexto en la línea: ");
            for (int j = inicio; j < fin; j++) 
            {
                if (j == i) 
                {
                    printf("*%s* ", palabras[j]);
                } 
                else 
                {
                    printf("%s ", palabras[j]);
                }
            }
            printf("\n");
        }
    }
}



int main() 
{
    struct Libro libros[MAX_LIBROS];
    int cantidadLibros = 0;
    char busqueda[MAX_LONGITUD_TITULO];
    int idLibro;
    char tituloLibro[MAX_LONGITUD_TITULO];
    char palabraBuscada[MAX_LONGITUD_TITULO];

    int opcion;
    int librosCargados = 0;

    do 
    {
        printf("\nBienvenido, seleccione una opción:\n");
        printf("1. Cargar documentos desde una carpeta\n");
        printf("2. Mostrar documentos ordenados por título\n");
        printf("3. Buscar un libro por título\n");
        printf("4. Palabras con mayor frecuencia\n");
        printf("5. Palabras más relevantes\n");
        printf("6. Buscar por palabra\n");
        printf("7. Mostrar palabra en su contexto dentro del libro\n");
        printf("0. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) 
        {
            case 1:
                if (librosCargados == 0) 
                {
                    cargarDocumentos(libros, &cantidadLibros);
                    librosCargados = 1;
                    printf("Documentos cargados exitosamente.\n");
                } 
                else 
                {
                    printf("Los libros ya han sido cargados. Utilice otra opción del menú.\n");
                }
                break;
            case 2:
                if (librosCargados == 1) 
                {
                    mostrarDocumentosOrdenados(libros, cantidadLibros);
                } 
                else 
                {
                    printf("Debe cargar los libros primero (opción 1 del menú).\n");
                }
                break;
            case 3:
                if (librosCargados == 1) 
                {
                    printf("Ingrese el título a buscar: ");
                    scanf(" %[^\n]", busqueda);
                    buscarLibroPorTitulo(libros, cantidadLibros, busqueda);
                }
                else 
                {
                    printf("Debe cargar los libros primero (opción 1 del menú).\n");
                }
                break;
            case 4:
                if (librosCargados == 1) 
                {
                    printf("Ingrese el ID del libro: ");
                    scanf("%d", &idLibro);
                    palabrasConMayorFrecuencia(libros, cantidadLibros, idLibro);
                } 
                else 
                {
                    printf("Debe cargar los libros primero (opción 1 del menú).\n");
                }
                break;
            case 5:
                if (librosCargados == 1) 
                {
                    printf("Ingrese el ID del libro: ");
                    scanf("%d", &idLibro);
                    palabrasMasRelevantes(libros, cantidadLibros, idLibro);
                } 
                else 
                {
                    printf("Debe cargar los libros primero (opción 1 del menú).\n");
                }
                break;
            case 6:
                if (librosCargados == 1) 
                {
                    printf("Ingrese la palabra a buscar: ");
                    scanf(" %[^\n]", palabraBuscada);
                    buscarLibrosPorPalabra(libros, cantidadLibros, palabraBuscada);
                } 
                else 
                {
                    printf("Debe cargar los libros primero (opción 1 del menú).\n");
                }
                break;
            case 7:
                if (librosCargados == 1) 
                {
                    printf("Ingrese el ID del libro: ");
                    scanf("%d", &idLibro);
                    printf("Ingrese la palabra a buscar: ");
                    scanf(" %[^\n]", palabraBuscada);
                    mostrarPalabraEnContexto(libros, cantidadLibros, idLibro, palabraBuscada);
                } 
                else 
                {
                    printf("Debe cargar los libros primero (opción 1 del menú).\n");
                }
                break;
        }
    } while (opcion != 0);

    for (int i = 0; i < cantidadLibros; i++) 
    {
        for (int j = 0; j < libros[i].num_palabras; j++) 
        {
            free(libros[i].palabras[j]);
        }
        free(libros[i].palabras);
        free(libros[i].frecuencia);
    }

    return 0;
}
