//
// Created by suwy on 5/25/26.
//

#include <string.h>
#include <stdlib.h>
#include "../include/02_Node-STR.h"

// START: of initializations
struct graph *initialise_graph(void) { // The initialization of a graph
    struct graph *graph = (struct graph *)calloc(1,sizeof(struct graph));
    if (!graph) {
        exit(-1);
    }
    initialise_word_stack(graph);
    return graph;
}

struct node *initialise_node(void){ // The initialization of a node
    struct node *node = (struct node *)calloc(1,sizeof(struct node));
    if (!node) {
        exit(-1);
    }
    return node;
}

struct edge *initialise_edge(void) { // The initialization of an edge
    struct edge *edge = (struct edge *)calloc(1,sizeof(struct edge));
    if (!edge) {
        exit(-1);
    }
    return edge;
}

void initialise_word_stack(struct graph *graph) {
    graph->stack = (struct stack_nodes *)calloc(1,sizeof(struct stack_nodes));
    if (!graph->stack) {
        exit(-1);
    }
    return;
}
// END: of initilizations

// INICIO: Configuracion de los nodos
void aduana(struct graph *graph, char *word) {
    if (!word) { // Se llego a la palabra final
        reset_stack(graph->stack);
        return;
    }
    graph->n_elements++; // Como llego una nueva palabra, se actualizan la cantidad de elementos
    struct node *node = node_to_graph(graph, word);
    if (!graph->stack->first_node) { // En caso de que no haya nadie en la primera posicion
        graph->stack->first_node = node;
    }else { // En caso de que toque usar la segunda posicion
        graph->stack->second_node = node;
        assign_edge(graph->stack->first_node, graph->stack->second_node);
        change_values(graph->stack);
    }
}

void change_values(struct stack_nodes *stack) {
    stack->first_node = stack->second_node;
    stack->second_node = NULL;
}

void reset_stack(struct stack_nodes *stack) {
    stack->first_node = NULL;
    stack->second_node = NULL;
}

struct node *node_to_graph(struct graph *graph, char *word) {
    struct node *repeated_node = word_repeated(graph->start, word);
    if (!repeated_node) { // No existe la palabra repetida
        struct node *node = initialise_node();
        node = filling_node(node, word);
        list_of_nodes(graph,node);
        return node;
    }else{ // Si existe alguien con esa palabra
        repeated_node->occurrences++; // Se actualiza la cantidad de apariciones de la palabra
        return repeated_node;
    }
}

struct node *word_repeated(struct node *node, char *word) { // Indica si ya existe un nodo con la palabra asignada
    if (!node) {
        return NULL; // No existe nadie repetido
    }
    if (strcmp(node->word, word)==0) { // Existe alguien repetido
        return node; // Retornamos al nodo repetido
    }
    return word_repeated(node->next_node, word);
}

struct node *filling_node(struct node *node,char *word) { // En caso de que sea la primera vez que aparece la palabra
    int size = 0;
    size = strlen(word);
    node->word = (char *)calloc(size+1,sizeof(char)); // +1 para darle espacio al '\0'
    if (!node->word) {
        exit(-1);
    }
    strcpy(node->word,word); // Copiamos la palabra dentro de la direccion
    node->occurrences=1; // Se inicializa en 1 el numero de apariciones
    return node;
}

void list_of_nodes(struct graph *graph, struct node *node) {
    if (!graph->start) { // Es el primer nodo que entra al grafo
        graph->start = node;
        graph->end = node;
    }else{ // Ya habia alguien asignado
        graph->end->next_node = node;
        graph->end = node;
    }
}
// FIN: Configuracion de los nodos

// INICIO: Configuracion de las conexiones

void assign_edge(struct node *node_1, struct node *node_2) {
    struct edge *aux = is_there_a_connection(node_1->first_connection, node_2);
    if (!aux) { // No existe conexion
        new_edge(node_1, node_2);
    }else {
        aux->occurrences++;
    }
}

void new_edge(struct node *node_1, struct node *node_2) {
    struct edge *edge = initialise_edge();
    edge->node = node_2;
    edge->occurrences = 1;
    if (!node_1->first_connection) { // En caso de que sea la primera conexion
        node_1->first_connection = edge;
        node_1->last_connection = edge;
    }else{ // En caso de que haya mas de una conexion
        node_1->last_connection->next_edge = edge;
        node_1->last_connection=edge;
    }
}

struct edge *is_there_a_connection(struct edge *edge, struct node *node_2) {
    if (!edge) { // No hay nadie apuntando a node_2
        return NULL;
    }
    if (edge->node == node_2) { // Alguien ya apunta a node_2
        return edge;
    }
    return is_there_a_connection(edge->next_edge, node_2);
}

// Calculo de probabilidades

void graph_probability(struct graph *graph) {
    node_probability(graph->start, graph->n_elements); // Obtiene las probabilidades de P(B)
    edge_probability(graph->start, graph->n_elements); // Obtiene las probabilidades de P(A|B) = P(BnA)/P(B)
}

void node_probability(struct node *node, int n_elements) { // Se determina la probabilidad general del Nodo
    if (!node) {
        return;
    }
    node->probability = (float)node->occurrences / (float)n_elements;
    return node_probability(node->next_node, n_elements);
}

void edge_probability(struct node *node, int n_elements){ // Se recorre cada nodo
    if (!node) {
        return;
    }
    assign_probability(node->first_connection, n_elements, node->probability); // Recibe el nodo actual
    return edge_probability(node->next_node, n_elements);
}

void assign_probability(struct edge *edge, int n_elements, float probability_node) { // Modifica las probabilidades de cada conexion del nodo actual
    if (!edge) {
        return;
    }
    float edge_probability = 0;
    edge_probability = (float)edge->occurrences / (float)n_elements;
    edge->probability = edge_probability / probability_node;
    return assign_probability(edge->next_edge, n_elements, probability_node);
}