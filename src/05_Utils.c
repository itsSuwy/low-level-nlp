//
// Created by suwy on 6/2/26.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include "../include/02_Node-STR.h"
#include "../include/01_Reading.h"
#include "../include/05_Utils.h"

#define FILE_READING '1'
#define GENERAL_GRAPH_PRINT '2'
#define NODE_PRINT '3'
#define EDGE_PRINT '4'
#define PHRASE_PREDICTION '5'
#define COMMON_PHRASES '6'
#define AUTOSUGGESTION '7'
#define CLEANING_GRAPH '8'
#define EXIT '9'
#define max_size 46
#define PATH "../texts/"

#define RESET "\033[0m"
#define BOLD "\033[1m"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"



// Electroencefalografista <- 25 letters
// Anticonstitucionalísimamente <- 29 letters
// Pneumonoultramicroscopicsilicovolcanoconiosis <- 45 letters

static char *input_from_user(void);
static void limpiando_buffer(void);
static void indicaciones(void);
static char user_input(char opcion);
static void mensaje_principal(void);
static void terminal_cleaning(void);
static void titulo_principal(void);

void menu(void) {
    struct graph *graph = NULL;
    mensaje_principal();
    char character = '\0';
    while (1) {
        character = user_input(character);
        switch (character) {
            case FILE_READING:
                break;
            case GENERAL_GRAPH_PRINT:
                break;
            case NODE_PRINT:
                break;
            case EDGE_PRINT:
                break;
            case PHRASE_PREDICTION:
                break;
            case COMMON_PHRASES:
                break;
            case AUTOSUGGESTION:
                break;
            case CLEANING_GRAPH:
                break;
            case EXIT:
                exit(0);
            default:
                puts("Entrada invalida!");
                puts("Presione cualquier tecla para continuar...");
                scanf(" %c", &character);
                limpiando_buffer();
                terminal_cleaning();
                break;
        }
    }
}

static void opening_file(struct graph *graph) {
    char *name_file = input_from_user();
    FILE *fp = fopen(name_file, "r");
    if (!fp) {
        free(name_file);
        puts("Archivo no encontrado");
        return;
    }
    reading_file(fp, graph);
    graph_probability(graph);
    free(name_file);
    fclose(fp);
}

static char *input_from_user(void) {
    char buffer[100] = {0};
    puts("Ingrese el nombre del archivo a analizar");
        scanf(" %99[^\n]", buffer);
    int total_size = 0;
    total_size = strlen(buffer) + strlen(PATH) + 1; // El uno representa el espacio para '\0'
    char *new_input = (char *)calloc(total_size, sizeof(char));
    if (!new_input) {
        exit(-1);
    }
    strcpy(new_input, PATH); // Copiamos primero la ruta
    strcat(new_input, buffer); // Concatenamos el input
    return new_input;
}

static void mensaje_principal(void) {
    titulo_principal();
    puts("Bienvenido al programa de grafos de Dieg" MAGENTA "OS" RESET );
    puts("La finalidad de este programa es desarrollar un modelo predictivo de frases de acuerdo al enriquecimiento"
         "mediante lectura de diversos textos!");
    puts("Supported Languages: English, Spanish.\n");
}

static void titulo_principal(void) {
    char titulo[20] = "Estructura de Datos";
    srand(time(NULL));
    int numero = 0;
    printf("\t\t\t");
    for (int i=0; i<19; i++) {
        numero = (rand()%6)+1;
        switch (numero) {
            case 1: // Secuencia de rojos
                printf(RED "%c" RESET, titulo[i]);
                break;
            case 2:
                printf(GREEN "%c" RESET, titulo[i]);
                break;
            case 3:
                printf(CYAN "%c" RESET, titulo[i]);
                break;
            case 4:
                printf(MAGENTA "%c" RESET, titulo[i]);
                break;
            case 5:
                printf(YELLOW "%c" RESET, titulo[i]);
                break;
            case 6:
                printf(BLUE "%c" RESET, titulo[i]);
                break;
            default:
                exit(-1);
        }
    }
    printf(BOLD YELLOW" ------> " RESET " Proyecto "BOLD RED "final!" RESET);
    printf("\n");
}

static void indicaciones(void) {
    puts("\t~~~~~ Opciones disponibles ~~~~~");
    puts("[1]: Lectura de un archivo");
    puts("[2]: Impresion general del grafo");
    puts("[3]: Impresion exclusiva de nodos y probabilidades");
    puts("[4]: Impresion exclusiva de conexiones (aristas) entre cada palabra del grafo");
    puts("[5]: Captura de frase y prediccion de la siguiente palabra (A)");
    puts("[6]: Mostrar las frases mas comunes mediante una palabra de entrada");
    puts("[7]: Ingresar una palabra + funcion de autocompletado");
    puts("[8]: Limpiar al grafo actual");
    puts("[9]: Salir");
    printf("Opcion a elegir: ");
}

static char user_input(char opcion) {
    indicaciones();
    opcion = '\0';
    scanf(" %c", &opcion);
    limpiando_buffer();
    printf("\n");
    return opcion;
}

static void terminal_cleaning(void) {
    for (int i=0; i<30; i++) {
        printf("\n");
    }
}

static void limpiando_buffer(void) {
    while (getchar() !='\n'){}
}