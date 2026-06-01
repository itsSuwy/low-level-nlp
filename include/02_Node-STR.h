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

struct stack_nodes {
    struct node *first_node;
    struct node *second_node; // The word that its supposed to point first_word
    //bool kill;
};

struct graph *initialise_graph(void);
struct node *initialise_node(void);
struct edge *initialise_edge(void);
void initialise_word_stack(struct graph *graph);
struct node *word_repeated(struct node *node, char *word);
struct node *filling_node(struct node *node,char *word);
void list_of_nodes(struct graph *graph, struct node *node);
struct node *node_to_graph(struct graph *graph, char *word);
struct edge *is_there_a_connection(struct edge *edge, struct node *node_2);
void new_edge(struct node *node_1, struct node *node_2);
void reset_stack(struct stack_nodes *stack);
void change_values(struct stack_nodes *stack);
void assign_edge(struct node *node_1, struct node *node_2);
void aduana(struct graph *graph, char *word);

#endif //LOW_LEVEL_NLP_02_NODE_STR_H
