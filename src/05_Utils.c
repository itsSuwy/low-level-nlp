//
// Created by suwy on 6/2/26.
//
// INICIO: Bibliotecas propias de C
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
// FIN: Bibliotecas propias de C

// INICIO: Bibliotecas creadas por suwy
#include "../include/02_Node-STR.h"
#include "../include/01_Reading.h"
#include "../include/05_Utils.h"
#include "../include/03_Dijkstra.h"
// FIN: Bibliotecas creadas por suwy

// INICIO: Constantes implementadas para mantener un poco mas legible el codigo
#define FILE_READING '1'
#define GENERAL_GRAPH_PRINT '2'
#define NODE_PRINT '3'
#define EDGE_PRINT '4'
#define PHRASE_PREDICTION '5'
#define COMMON_PHRASES '6'
#define CLEANING_GRAPH '7'
#define EXIT '8'
#define PATH "../texts/"
// FIN: Constantes implementadas para mantener un poco mas legible el codigo

/* Codigo muerto
#define max_size 46
Electroencefalografista <- 25 letters
Anticonstitucionalísimamente <- 29 letters
Pneumonoultramicroscopicsilicovolcanoconiosis <- 45 letters
*/
// INICIO: Prototipos locales
static void opening_file(struct graph *graph, FILE *fp);
static char *input_from_user(void);
static void indicaciones(void);
static char user_input(char opcion);
static void mensaje_principal(void);
static void terminal_cleaning(void);
static void titulo_principal(void);
static void limpiando_buffer(void);
static void general_print_graph(struct graph *graph);
static void node_print(struct graph *graph);
static void edge_print(struct graph *graph);
static void cleaning_graph(struct graph *graph);
static void default_case(void);
static struct graph *file_reading(struct graph *graph);
static char *word_process(char phrase[100]);
static void phrase_prediction(struct graph *graph);
// FIN: Prototipos locales

// INICIO: Sintaxis del menu principal
void menu(void) {
    setlocale(LC_ALL, "es_MX.UTF-8"); // Prepara la terminal para utilizar caracteres hispanos
    // NOTA: El sistema debe tener este locale descargado, de lo contrario, lo ignorara y usara el default del sistema
    struct graph *graph = NULL; // Apuntador de tipo grafo que apunta a NULL
    mensaje_principal();
    char character = '\0';
    while (1) {
        character = user_input(character);
        switch (character) {
            case FILE_READING:
                graph = file_reading(graph);
                break;
            case GENERAL_GRAPH_PRINT:
                general_print_graph(graph);
                break;
            case NODE_PRINT:
                node_print(graph);
                break;
            case EDGE_PRINT:
                edge_print(graph);
                break;
            case PHRASE_PREDICTION:
                phrase_prediction(graph);
                break;
            case COMMON_PHRASES: // TODO
                puts("Todo....");
                break;
            case CLEANING_GRAPH:
                cleaning_graph(graph);
                graph = NULL;
                break;
            case EXIT:
                exit(0);
            default:
                default_case();
                break;
        }
    }
}
// FIN: Sintaxis del menu principal

// INICIO: Logica de casa case del menu principal
static char user_input(char opcion) { // Funcion para capturar la seleccion del usuario para cada case
    indicaciones(); // Se imprimen las indicaciones
    opcion = '\0'; // Se inicializa la variable opcion en '\0' para prevenir basura
    scanf(" %c", &opcion); // Se captura y se guarda
    limpiando_buffer(); // Se limpia la terminal
    return opcion;
}

static struct graph *file_reading(struct graph *graph) { // Funcion para abrir un archivo dentro del directiorio ../texts/
    if (!graph) { // En caso de que el grafo no haya sido inicializado
        graph = initialise_graph();
    }
    char *name_file = input_from_user(); // Nombre del archivo.txt a abrir
    FILE *fp = fopen(name_file, "r");
    free(name_file);
    if (!fp) { // En caso de que el archivo no exista
        puts(BOLD RED"Error: " RESET "Archivo no encontrado");
        printf("\n");
        return graph;
    }
    opening_file(graph, fp); // Se llama a un encapsulador de funciones de 01_Reading.c/.h
    puts("Archivo leido con " GREEN "exito" RESET);
    printf("\n");
    return graph;
}

static char *input_from_user(void) { // Funcion para determinar el archivo que el usuario desea abrir para crear un grafo
    char buffer[100] = {0};
    printf("Ingrese el nombre del archivo a analizar: ");
    scanf(" %99[^\n]", buffer); // Se captura el nombre del archivo
    limpiando_buffer(); // Se limpia el buffer
    int total_size = 0;
    total_size = strlen(buffer) + strlen(PATH) + 1; // Se determina el tamanio entre el nombre del .txt que quiere el usuario junto al tamanio de la ruta PATH (../texts/)
    char *new_input = (char *)calloc(total_size, sizeof(char)); // Se crea con memoria dinamica una variable para almacenar ambos strings
    if (!new_input) { // En caso de un error de memoria
        exit(-1);
    }
    strcpy(new_input, PATH); // Copiamos primero la ruta al archivo
    strcat(new_input, buffer); // Concatenamos el input del usuario a la ruta ya existente
    return new_input; // Se reorna el nuevo string creado
}

static void opening_file(struct graph *graph, FILE *fp) { // Funcion que envuelve la logica para leer un archivo y determinar probabilidades de frases
    reading_file(fp, graph); // Funcion de 01_Reading.c/.h
    graph_probability(graph); // Funcion de 02_Node-STR.c/h
    fclose(fp); // Se cierra de manera segura el archivo
}

static void general_print_graph(struct graph *graph) { // Funcion para imprimir de manera general toda la informacion del grafo
    if (!graph) {
        // En caso de que el gafo no contenga elementos
        puts(BOLD RED "Error: " RESET "Grafo vacio\n");
        return;
    }
    if (!graph->start) {
        puts(BOLD RED "Error: " RESET "El grafo no cuenta con nodos inicializados!\n");
        return;
    }
    printf("\n");
    printf("\t\t Imprimiendo detalles " BOLD RED"totales" RESET " del grafo\n");
    print_graph(graph); // Funcion de 02_Node-STR.c/.h

}

static void node_print(struct graph *graph) { // Funcion para imprimir informacion exclusiva de los nodos del grafo
    if (!graph) { // En caso de que el grafo este vacio
        puts(BOLD RED "Error: " RESET "El grafo no cuenta con nodos inicializados!\n");
        return;
    }
    if (!graph->start) { // En caso de que el gafo exista pero aun no tenga nodos asignados
        puts(BOLD RED "Error: " RESET "El grafo no cuenta con nodos inicializados!\n");
        return;
    }else {
        node_focus_print(graph->start, 0); // Funcion de 02_Node-STR.c/.h
    }
}

static void edge_print(struct graph *graph) { // Funcion para imprimir informacion exclusiva de las aristas del grafo
    if (!graph) { // En caso de que el grafo este vacio
        puts(BOLD RED "Error: " RESET "El grafo no esta inicializado\n");
        return;
    }
    if (!graph->start) { // En caso de que el grafo no tenga nodos asignados
        puts(BOLD RED "Error: " RESET "El grafo no cuenta con nodos inicializados!\n");
        return;
    }
    if (!graph->start->first_connection) { // En caso de que los nodos asignados no tengan aristas a otros nodos
        puts(BOLD RED "Error: " RESET "El grafo no cuenta con conexiones asignadas!\n");
        return;
    }
    edge_focus_print(graph->start); // Funcion de 02_Node-STR.c/.h
}

static void cleaning_graph(struct graph *graph) { // Funcion para liberar la memoria del grafo de manera segura
    if (!graph) {
        puts(BOLD RED "Error: " RESET "El grafo no esta inicializado\n");
        return;
    }
    stalin_graph(graph); // Funcion de 02_Node-STR.c/.h
    puts("Grafo purgado con " GREEN "exito" RESET);
    printf("\n");
}

static void phrase_prediction(struct graph *graph) { // Funcion para utilizar Dijkstra en la prediccion de frases
    if (!graph) { // En caso de que el grafo no este inicializado
        puts(BOLD RED "Error: " RESET "El grafo no esta inicializado\n");
        return;
    }
    if (!graph->start) { // En caso de que el grafo no tenga nodos inicializados
        puts(BOLD RED "Error: " RESET "El grafo no cuenta con nodos inicializados!\n");
        return;
    }
    char phrase[100];
    puts("Ingrese una frase " BOLD RED "[menor a 100 palabras]" RESET); // Se captura una frase del usuario para predecir
        scanf(" %99[^\n]", phrase);
    limpiando_buffer(); // Se limpia el buffer en caso de que se haya ingresado informacion que pudiera causar desbordamiento
    char *ptr_phrase = word_process(phrase); // La frase del usuario se procesa y guarda en un apuntador
    Dijkstra(graph, ptr_phrase); // Funcion de 03_Dijkstra.c/.h
    free(ptr_phrase);
}

static char *word_process(char phrase[100]) { // Funcion para procesar la frase que el usuario ingreso
    int str_size = strlen(phrase) + 1; // Se determina la longitud total de la frase
    char *new_word = (char *) calloc(str_size, sizeof(char)); // Se crea con memoria dinamica un apuntador que contendra la frase
    if (!new_word) { // En caso de haber un error de memoria
        exit(-1);
    }
    strcpy(new_word, phrase); // Se copia la frase del usuario en la variable
    return new_word;
}

static void default_case(void) { // En caso de que el usuario haya ingresado un caracter que no corresponde a las opciones del menu
    puts(BOLD RED "Entrada invalida!"RESET);
    puts("Presione cualquier tecla para " BOLD GREEN "continuar..." RESET); // Ligera pantalla de "carga" para indicar que ese caracter no existia
    getchar();
    limpiando_buffer();
    terminal_cleaning(); // Se limpia la terminal
}
// FIN: Logica de casa case del menu principal

// INICIO: Funciones de decoracion de 05_Utils.c
static void mensaje_principal(void) { // Funcion para imprimir en la terminal el mensaje de bienvenida
    titulo_principal();
    puts("Bienvenido al programa de grafos de Dieg" MAGENTA "OS" RESET );
    puts("La finalidad de este programa es desarrollar un modelo predictivo de frases de acuerdo al enriquecimiento"
         "mediante lectura de diversos textos!");
    puts("Supported Languages: " BOLD YELLOW "English," RESET BOLD CYAN" Spanish." RESET "\n");
}

static void titulo_principal(void) { // Funcion adicional para imprimir las letras del nombre de la asignatura con diferentes colores de manera pseudoaleatoria
    srand(time(NULL)); // Se inicializa la semilla en NULL
    char titulo[20] = "Estructura de Datos"; // String a manipular
    int numero = 0; // Contador
    printf("\t\t\t"); // Tabulaciones para centrar el titulo
    for (int i=0; i<19; i++) {
        numero = (rand()%6)+1; // Se obtiene un numero de manera pseudoaleatoria del 1 al 6
        switch (numero) { // Switch para imprimir el caracter de la posicion actual con el color al que corresponda el numero pseudo-aleatorio
            case 1: // Secuencia de rojos
                printf(RED "%c" RESET, titulo[i]);
                break;
            case 2: // Secuencia de verdes
                printf(GREEN "%c" RESET, titulo[i]);
                break;
            case 3: // Secuencia de cyans
                printf(CYAN "%c" RESET, titulo[i]);
                break;
            case 4: // Secuencia de morados/violetas
                printf(MAGENTA "%c" RESET, titulo[i]);
                break;
            case 5: // Secuencia de amarillos
                printf(YELLOW "%c" RESET, titulo[i]);
                break;
            case 6: // Secuencia de azules
                printf(BLUE "%c" RESET, titulo[i]);
                break;
            default: // Algo raro ocurrio, se cierra el programa por seguridad
                exit(-1);
        }
    }
    printf(BOLD YELLOW" ------> " RESET " Proyecto "BOLD RED "final!" RESET); // Impresion del titulo del proyecto
    printf("\n");
}

static void indicaciones(void) { // Funcion para imprimir que hace cada numero dentro del menu principal
    puts("\t" RED "~~~~~" RESET BOLD YELLOW "|"RESET BOLD GREEN " Opciones disponibles " RESET BOLD YELLOW "|" RESET RED"~~~~~" RESET);
    puts(RED"["RESET YELLOW"1"RESET RED"]"RESET": Lectura de un archivo");
    puts(RED"["RESET YELLOW"2"RESET RED"]"RESET": Impresion general del grafo");
    puts(RED"["RESET YELLOW"3"RESET RED"]"RESET": Impresion exclusiva de nodos y probabilidades");
    puts(RED"["RESET YELLOW"4"RESET RED"]"RESET": Impresion exclusiva de conexiones (aristas) entre cada palabra del grafo");
    puts(RED"["RESET YELLOW"5"RESET RED"]"RESET": Captura de frase y prediccion de la siguiente palabra (A)");
    puts(RED"["RESET YELLOW"6"RESET RED"]"RESET": Mostrar las frases mas comunes mediante una palabra de entrada");
    puts(RED"["RESET YELLOW"7"RESET RED"]"RESET": Limpiar al grafo actual");
    puts(RED"["RESET YELLOW"8"RESET RED"]"RESET": " BOLD RED "Salir" RESET);
    printf("Opcion a elegir: ");
}

static void terminal_cleaning(void) { // Funcion para limpiar la terminal
    for (int i=0; i<30; i++) {
        printf("\n"); // Solo imprime 30 veces un salto de linea para simular que se limpio
    } // jejejeje
}

static void limpiando_buffer(void) { // Funcion para eliminar residuos de scanf que se desbordaron en la terminal sin que truene el programa
    while (getchar() !='\n'){} // Recibe todos los caracteneres que no sean saltos de linea y los consume
}
// Fin: Funciones de decoracion de 05_Utils.c