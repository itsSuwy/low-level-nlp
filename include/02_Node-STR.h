//
// Created by suwy on 5/25/26.
//

#ifndef LOW_LEVEL_NLP_02_NODE_STR_H
#define LOW_LEVEL_NLP_02_NODE_STR_H

#include <stdbool.h>

struct graph { // Cuerpo del grafo
    int n_elements; // Cantidad de palabras que recibe
    struct node *start; // Inicio de la cola de cada nodo del grafo
    struct node *end; // Final de la cola de cada nodo del grafo
    struct stack_nodes *stack;  // Almacena de manera temporal las palabras que entran
};

struct node { // Cuerpo de un nodo
    char *word; // Palabra que tiene asignada
    int occurrences;
    float probability;
    struct node *next_node; // Lista enlazada de nodos dentro del grafo
    struct edge *first_connection; // Primera conexion del nodo
    struct edge *last_connection; // Ultima conexion
    struct dijkstra *Dijkstra_assigned;
};

struct edge { // Cuerpo de una arista / conexion
    int occurrences; // Cantidad de veces que se repite la conexion
    float probability;
    struct edge *next_edge; // Siguiente conexion del nodo
    struct node *node; // Nodo al que apunta la conexion
    bool visited;
};

struct stack_nodes { // Contenedor temporal que almacenara las palabras que presenten conexiones
    struct node *first_node;
    struct node *second_node;
};

// Funciones publicas
struct graph *initialise_graph(void);
struct node *initialise_node(void);
struct edge *initialise_edge(void);
void aduana(struct graph *graph, char *word);
void graph_probability(struct graph *graph);
void print_graph(struct graph *graph);
void stalin_graph(struct graph *graph);
void node_focus_print(struct node *node, int n);
void edge_focus_print(struct node *node);
void change_values(struct stack_nodes *stack);
void reset_stack(struct stack_nodes *stack);
struct node *filling_node(struct node *node,char *word);
struct node *word_repeated(struct node *node, char *word);
void new_edge(struct node *node_1, struct node *node_2);
void list_of_nodes(struct graph *graph, struct node *node);

#endif //LOW_LEVEL_NLP_02_NODE_STR_H
