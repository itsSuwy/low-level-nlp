//
// Created by suwy on 5/25/26.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/02_Node-STR.h"

static struct node *initialise_node(void);
static struct edge *initialise_edge(void);
static void initialise_word_stack(struct graph *graph);
static struct node *word_repeated(struct node *node, char *word);
static struct node *filling_node(struct node *node,char *word);
static void list_of_nodes(struct graph *graph, struct node *node);
static struct node *node_to_graph(struct graph *graph, char *word);
static struct edge *is_there_a_connection(struct edge *edge, struct node *node_2);
static void reset_stack(struct stack_nodes *stack);
static void change_values(struct stack_nodes *stack);
static void assign_edge(struct node *node_1, struct node *node_2);
static void node_probability(struct node *node, int n_elements);
static void edge_probability(struct node *node, int n_elements);
static void assign_probability(struct edge *edge, int n_elements, float probability_node);
static void new_edge(struct node *node_1, struct node *node_2);
static void impresion(struct node *node);
static void impresion_edges(struct edge *edge);
static void stalin_node(struct node *node);
static void stalin_edge(struct edge *edge);
static void connections_print(struct edge *edge, char *word);

struct stack_nodes {
    struct node *first_node;
    struct node *second_node; // The word that its supposed to point first_word
};

// START: of initializations
struct graph *initialise_graph(void) { // The initialization of a graph
    struct graph *graph = (struct graph *)calloc(1,sizeof(struct graph));
    if (!graph) {
        exit(-1);
    }
    initialise_word_stack(graph);
    return graph;
}

static struct node *initialise_node(void){ // The initialization of a node
    struct node *node = (struct node *)calloc(1,sizeof(struct node));
    if (!node) {
        exit(-1);
    }
    return node;
}

static struct edge *initialise_edge(void) { // The initialization of an edge
    struct edge *edge = (struct edge *)calloc(1,sizeof(struct edge));
    if (!edge) {
        exit(-1);
    }
    return edge;
}

static void initialise_word_stack(struct graph *graph) {
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

static void change_values(struct stack_nodes *stack) {
    stack->first_node = stack->second_node;
    stack->second_node = NULL;
}

static void reset_stack(struct stack_nodes *stack) {
    stack->first_node = NULL;
    stack->second_node = NULL;
}

static struct node *node_to_graph(struct graph *graph, char *word) {
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

static struct node *word_repeated(struct node *node, char *word) { // Indica si ya existe un nodo con la palabra asignada
    if (!node) {
        return NULL; // No existe nadie repetido
    }
    if (strcmp(node->word, word)==0) { // Existe alguien repetido
        return node; // Retornamos al nodo repetido
    }
    return word_repeated(node->next_node, word);
}

static struct node *filling_node(struct node *node,char *word) { // En caso de que sea la primera vez que aparece la palabra
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

static void list_of_nodes(struct graph *graph, struct node *node) {
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

static void assign_edge(struct node *node_1, struct node *node_2) {
    struct edge *aux = is_there_a_connection(node_1->first_connection, node_2);
    if (!aux) { // No existe conexion
        new_edge(node_1, node_2);
    }else {
        aux->occurrences++;
    }
}

static void new_edge(struct node *node_1, struct node *node_2) {
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

static struct edge *is_there_a_connection(struct edge *edge, struct node *node_2) {
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

static void node_probability(struct node *node, int n_elements) { // Se determina la probabilidad general del Nodo
    if (!node) {
        return;
    }
    node->probability = (float)node->occurrences / (float)n_elements;
    return node_probability(node->next_node, n_elements);
}

static void edge_probability(struct node *node, int n_elements){ // Se recorre cada nodo
    if (!node) {
        return;
    }
    assign_probability(node->first_connection, n_elements, node->probability); // Recibe el nodo actual
    return edge_probability(node->next_node, n_elements);
}

static void assign_probability(struct edge *edge, int n_elements, float probability_node) { // Modifica las probabilidades de cada conexion del nodo actual
    if (!edge) {
        return;
    }
    float edge_probability = 0;
    edge_probability = (float)edge->occurrences / (float)n_elements;
    edge->probability = edge_probability / probability_node;
    return assign_probability(edge->next_edge, n_elements, probability_node);
}

void print_graph(struct graph *graph) {
    impresion(graph->start);
    printf("Se registraron con exito %i palabras\n", graph->n_elements);
}

static void impresion(struct node *node) {
    if (!node) {
        return;
    }
    printf("|--- [%s] ---| -> (%i) apariciones + %.2f%% de probabilidad\n", node->word, node->occurrences, node->probability*100);
    if (!node->first_connection) { // Carece de conexiones la palabra
        puts("\t|-> Palabra carente de conexiones\n");
    }else {
        impresion_edges(node->first_connection);
        printf("\n");
    }
    impresion(node->next_node);
}

static void impresion_edges(struct edge *edge) {
    if (!edge) {
        return;
    }
    printf("\t|-> (x%i) -> %s\n",edge->occurrences,edge->node->word);
    printf("\t\t|-> %.2f%% de probabilidad\n", edge->probability*100);
    impresion_edges(edge->next_edge);
}

void node_focus_print(struct node *node, int n) {
    if (!node) {
        printf("\n");
        return;
    }
    printf("Nodo numero: %i | Palabra: %s | P(A): %.2f%% | Apariciones: %i\n", n+1, node->word, node->probability * 100, node->occurrences);
    return node_focus_print(node->next_node, n+1);
}

void edge_focus_print(struct node *node) {
    if (!node) {
        printf("\n");
        return;
    }
    if (!node->first_connection) {
        printf("[%s] ---> Carece de conexiones", node->word);
        return edge_focus_print(node->next_node);
    }
    connections_print(node->first_connection, node->word);
    return edge_focus_print(node->next_node);
}

static void connections_print(struct edge *edge, char *word) {
    if (!edge) {
        return;
    }
    printf("[%s] ---> [%s] | %i conexiones | %.2f%% de probabilidad\n",word, edge->node->word, edge->occurrences, edge->probability*100);
    return connections_print(edge->next_edge, word);
}

// Funciones para liberar memoria del grafo
void stalin_graph(struct graph *graph) {
    if (!graph) {
        puts("No hay grafo por liberar");
        return;
    }
    stalin_node(graph->start);
    graph->start = NULL;
    graph->end = NULL;
    free(graph->stack);
    free(graph);
    puts("Grafo liberado con exito");
}

static void stalin_node(struct node *node) {
    if (!node) {
        return;
    }
    struct node *next_node = node->next_node;
    free(node->word); // Se libera la memoria dinamica de la palabra
    stalin_edge(node->first_connection); // Se librean todas las conexiones realizadas
    free(node); // Se libera al nodo actual
    stalin_node(next_node);
}

static void stalin_edge(struct edge *edge) {
    if (!edge) {
        return;
    }
    struct edge *next_edge = edge->next_edge;
    free(edge);
    return stalin_edge(next_edge);
}