//
// Created by suwy on 6/2/26.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/02_Node-STR.h"
#include "../include/01_Reading.h"
#include "../include/05_Utils.h"

#define PATH "../texts/"

static char *input_from_user(void);

void opening_file(struct graph *graph) {
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

