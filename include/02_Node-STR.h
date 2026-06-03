//
// Created by suwy on 5/25/26.
//

#ifndef LOW_LEVEL_NLP_02_NODE_STR_H
#define LOW_LEVEL_NLP_02_NODE_STR_H

struct graph { // El grafo contiene un registro interno que funciona para almacenar de manera temporal cada palabra
    int n_elements; // Cantidad de palabras que recibe
    struct node *start; // Inicio de la cola de cada nodo del grafo
    struct node *end; // Final de la cola de cada nodo del grafo
    struct stack_nodes *stack;  // Almacena de manera temporal las palabras que entran
};

struct node {
    char *word;
    int occurrences;
    float probability;
    struct node *next_node; // Lista enlazada de nodos dentro del grafo
    struct edge *first_connection; // Primera conexion del nodo
    struct edge *last_connection; // Ultima conexion
};

struct edge {
    int occurrences; // Cantidad de veces que se repite la conexion
    float probability;
    struct edge *next_edge; // Siguiente conexion del nodo
    struct node *node; // Nodo al que apunta la conexion
};

// Funciones publicas
struct graph *initialise_graph(void);
void aduana(struct graph *graph, char *word);
void graph_probability(struct graph *graph);
void print_graph(struct graph *graph);
void stalin_graph(struct graph *graph);

#endif //LOW_LEVEL_NLP_02_NODE_STR_H
