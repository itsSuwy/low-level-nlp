//
// Created by suwy on 5/25/26.
//

#ifndef LOW_LEVEL_NLP_02_NODE_STR_H
#define LOW_LEVEL_NLP_02_NODE_STR_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct graph { // El grafo contiene un registro interno que funciona para almacenar de manera temporal cada palabra
    int n_elements; // Cantidad de palabras que recibe
    struct node *start; // Inicio de la cola de cada nodo del grafo
    struct node *end; // Final de la cola de cada nodo del grafo
    struct word_stack *stack;  // Almacena de manera temporal las palabras que entran
};

struct node {
    char *word;
    int occurrences;
    float probability;
    struct node *next; // Lista enlazada de nodos dentro del grafo
    struct edge *first_connection; // Primera conexion del nodo
    struct edge *last_connection; // Ultima conexion
};

struct edge {
    int occurrences; // Cantidad de veces que se repite la conexion
    struct edge *next_edge; // Siguiente conexion del nodo
    struct node *node; // Nodo al que apunta la conexion
    float probability;
};

struct word_stack {
    char *first_word;
    char *second_word; // The word that its supposed to point first_word
    bool kill;
};
struct graph *initialise_graph(void);
struct node *initialise_node(void);
struct edge *initialise_edge(void);
void initialise_word_stack(struct graph *graph);

#endif //LOW_LEVEL_NLP_02_NODE_STR_H
