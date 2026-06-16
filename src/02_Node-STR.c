//
// Created by suwy on 5/25/26.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/02_Node-STR.h"

// INICIO: Prototipos locales de funciones
static void initialise_word_stack(struct graph *graph);
static struct node *node_to_graph(struct graph *graph, char *word);
static struct edge *is_there_a_connection(struct edge *edge, struct node *node_2);
static void assign_edge(struct node *node_1, struct node *node_2);
static void node_probability(struct node *node, int n_elements);
static void edge_probability(struct node *node, int n_elements);
static void assign_probability(struct edge *edge, int n_elements, float probability_node);
static void impresion(struct node *node);
static void impresion_edges(struct edge *edge);
static void stalin_node(struct node *node);
static void stalin_edge(struct edge *edge);
static void connections_print(struct edge *edge, char *word);
// FIN: Prototipos locales de funciones


// START: of initializations
struct graph *initialise_graph(void) { // Se inicializa y retorna un grafo creado con memoria dinamica
    struct graph *graph = (struct graph *)calloc(1,sizeof(struct graph));
    if (!graph) { // Se detecta un error de memoria
        exit(-1); // Se cierra el programa por seguridad
    }
    initialise_word_stack(graph); // Inicializa la cola de palabras que estan conectadas entre si
    return graph;
}

struct node *initialise_node(void){ // Se inicializa y retorna un nodo del grafo
    struct node *node = (struct node *)calloc(1,sizeof(struct node));
    if (!node) { // Se detecta un error de memoria
        exit(-1); // Se cierra elprograma por seguridad
    }
    return node;
}

struct edge *initialise_edge(void) { // Se inicializa y retorna una arista del nodo
    struct edge *edge = (struct edge *)calloc(1,sizeof(struct edge));
    if (!edge) { // Se detecta un error de memoria
        exit(-1); // Se cierra el programa por seguridad
    }
    return edge;
}

static void initialise_word_stack(struct graph *graph) { // Solo se inicializa el contenedor de dos palabras conectadas dentro de un grafo ya creado
    graph->stack = (struct stack_nodes *)calloc(1,sizeof(struct stack_nodes));
    if (!graph->stack) { // Se detecta un error de memoria
        exit(-1); // Se cierra el programa por seguridad
    }
}
// END: of initilizations

// INICIO: Configuracion de los nodos
void aduana(struct graph *graph, char *word) { // Administra el orden en como llegan dos palabras que presentan una conexion de A -> B pero NO de B -> A
    if (!word) { // Se llego a la palabra final por ende no conecta con ningun B (o almenos de manera explicita)
        reset_stack(graph->stack); // Funcion para reiniciar el stack de palabras
        return;
    }
    graph->n_elements++; // Como llego una nueva palabra, se actualizan la cantidad de elementos dentro del grafo
    struct node *node = node_to_graph(graph, word); // Se busca o crea un nodo del grafo con esa palabra
    if (!graph->stack->first_node) { // En caso de que no haya nadie en la primera posicion del contenedor de palabras
        graph->stack->first_node = node; // Se asigna como primera posicion
    }else {
        graph->stack->second_node = node; // Se asigna como segunda posicion
        // Como el contenedor (quien recibe dos palabras) ya esta lleno, se crea una arista de palabra en primera posicion (A) hacia la palabra de la segunda posicion (B)
        assign_edge(graph->stack->first_node, graph->stack->second_node); // Se crea la arista y se la asigna a A
        change_values(graph->stack); // Se remueve (NO se libera) la palabra de la primera posicion y su lugar lo toma la palabra de la segunda posicion
    }
}

void change_values(struct stack_nodes *stack) { // Funcion para cambiar de lugar las palabras dentro del contenedor
    stack->first_node = stack->second_node; // La palabra en la primera posicion ahora es la palabra de la segunda posicion
    stack->second_node = NULL; // La segunda palabra queda apuntando a NULL por seguridad
}

void reset_stack(struct stack_nodes *stack) { // Funcion que se encarga de reiniciar el contenedor de palabras
    stack->first_node = NULL;
    stack->second_node = NULL;
    // Ambas posiciones ahora apuntan a NULL por seguridad
}

static struct node *node_to_graph(struct graph *graph, char *word) { // Funcion para enviar el grafo un nodo
    struct node *repeated_node = word_repeated(graph->start, word); // Primero busca si no exista ya un nodo con la palabra asignada
    if (!repeated_node) { // No existe la palabra repetida
        struct node *node = initialise_node(); // Se crea elnodo
        node = filling_node(node, word); // Se le asigna el valor de la palabra
        list_of_nodes(graph,node); // Se adjunta el nodo a una lista enlazada de todos los nodos del grafo
        return node;
    }else{ // Si existe alguien con esa palabra
        repeated_node->occurrences++; // Se actualiza la cantidad de apariciones de la palabra
        return repeated_node;
    }
    // Ambos escenarios retornan el nodo con la palabra asignada
}

struct node *word_repeated(struct node *node, char *word) { // Indica si ya existe un nodo con la palabra asignada
    if (!node) {
        return NULL; // No existe nadie repetido
    }
    if (strcmp(node->word, word)==0) { // Existe alguien repetido
        return node; // Retornamos al nodo repetido
    }
    return word_repeated(node->next_node, word); // Recursion
}

struct node *filling_node(struct node *node,char *word) { // Funcion para llenar la informacion del nodo con la palabra asignada
    int size = 0;
    size = strlen(word); // Tamanio en bytes que tiene la palabra
    node->word = (char *)calloc(size+1,sizeof(char)); // La palabra del nodo asignado se crea con memoria dinamica y +1 para darle espacio al '\0'
    if (!node->word) { // Se detecta un error de memoria
        exit(-1); // Se cierra el programa por seguridad
    }
    strcpy(node->word,word); // Copiamos la palabra dentro de la direccion
    node->occurrences=1; // Se inicializa en 1 el numero de apariciones
    return node;
}

void list_of_nodes(struct graph *graph, struct node *node) { // Funcion para asignar en una lista enlazada todos los nodos del grafo
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

static void assign_edge(struct node *node_1, struct node *node_2) { // Funcion para asignar una arista que conecta nodo_1 a nodo_2 pero NO conecta a nodo_2 con nodo_1
    struct edge *aux = is_there_a_connection(node_1->first_connection, node_2); // Se busca si ya existe una conexion entre nodo 1 y nodo 2
    if (!aux) { // No existe conexion
        new_edge(node_1, node_2);
    }else {
        aux->occurrences++; // Se incrementa la cantidad de conexiones entre nodo 1 y nodo 2
    }
}

void new_edge(struct node *node_1, struct node *node_2) { // Funcion para crear una nueva conexion entre nodos
    struct edge *edge = initialise_edge(); // Se inicializa la arista
    edge->node = node_2; // La arista apunta a node_2
    edge->occurrences = 1; // Se actualiza las apariciones a 1
    if (!node_1->first_connection) { // En caso de que sea la primera conexion de node_1
        node_1->first_connection = edge;
        node_1->last_connection = edge;
    }else{ // En caso de que haya mas de una conexion
        node_1->last_connection->next_edge = edge;
        node_1->last_connection=edge;
    }
}

static struct edge *is_there_a_connection(struct edge *edge, struct node *node_2) { // Funcion que evalua si ya edge ya tiene una conexion hacia  node_2
    if (!edge) { // No hay nadie apuntando a node_2
        return NULL;
    }
    if (edge->node == node_2) { // Alguien ya apunta a node_2
        return edge;
    }
    return is_there_a_connection(edge->next_edge, node_2); // Recursion
}

// INICIO: Calculo de probabilidades
void graph_probability(struct graph *graph) { // Funcion global encargada de determinar probabilidades de nodos como de conexiones
    node_probability(graph->start, graph->n_elements); // Obtiene las probabilidades de P(B)
    edge_probability(graph->start, graph->n_elements); // Obtiene las probabilidades de P(A|B) = P(BnA)/P(B)
}

static void node_probability(struct node *node, int n_elements) { // Funcion que determina la probabilidad general del Nodo
    if (!node) { // Se llego al final
        return;
    }
    node->probability = (float)node->occurrences / (float)n_elements; // Se dividen las apariciones del nodo entre los elementos totales del grafo
    return node_probability(node->next_node, n_elements); // Recursion
}

static void edge_probability(struct node *node, int n_elements){ // Calcula las probabilidades de las conexiones de cada nodo
    if (!node) {
        return;
    }
    assign_probability(node->first_connection, n_elements, node->probability); // Calcula la probabilidad condicional de cada conexión
    return edge_probability(node->next_node, n_elements);
}

static void assign_probability(struct edge *edge, int n_elements, float probability_node) { // Funcion que determina la probabilidad de una conexion hacia cada nodo
    if (!edge) { // Se llego a la conexion final
        return;
    }
    float edge_probability = 0;
    edge_probability = (float)edge->occurrences / (float)n_elements; // De le asigna la probabilidad de que la arista aparezca
    edge->probability = edge_probability / probability_node; // Probabilidad de tomar esa conexión dado el nodo actual
    return assign_probability(edge->next_edge, n_elements, probability_node); // Recursion
}
// FIN: Calculo de probabilidades

// INICIO: Impresion del grafo

void print_graph(struct graph *graph) { // Funcion que envuelve la logica de impresion junto a un mensaje que indica la cantidad de palabras del grafo
    impresion(graph->start);
    printf("Se registraron con exito %i palabras", graph->n_elements);
    printf("\n\n");
}

static void impresion(struct node *node) { // Funcion que imprime cada nodo junto a sus descripciones
    if (!node) { // Se llego al nodo final
        return;
    }
    printf("|--- [%s] ---| -> (%i) apariciones + %.2f%% de probabilidad\n", node->word, node->occurrences, node->probability*100);
    if (!node->first_connection) { // Carece de conexiones la palabra
        puts("\t|-> Palabra carente de conexiones\n");
    }else {
        impresion_edges(node->first_connection);
        printf("\n");
    }
    impresion(node->next_node); // Recursion al siguiente nodo
}

static void impresion_edges(struct edge *edge) { // Funcion que imprime los detalles de cada conexion
    if (!edge) { // Se llego a la conexion final
        return;
    }
    printf("\t|-> (x%i) -> %s\n",edge->occurrences,edge->node->word);
    printf("\t\t|-> %.2f%% de probabilidad\n", edge->probability*100);
    impresion_edges(edge->next_edge); // Recursion a la siguiente arista
}

void node_focus_print(struct node *node, int n) { // Funcion exclusiva para imprimir informacion de nodos
    if (!node) { // Se llego al nodo final
        printf("\n");
        return;
    }
    printf("Nodo numero: %i | Palabra: %s | P(A): %.2f%% | Apariciones: %i\n", n+1, node->word, node->probability * 100, node->occurrences);
    return node_focus_print(node->next_node, n+1);
}

void edge_focus_print(struct node *node) { // Funcion exclusiva para imprimir informacion de aristas
    if (!node) { // Se llego al nodo final
        printf("\n");
        return;
    }
    if (!node->first_connection) { // El nodo actual no tiene conexiones
        printf("[%s] ---> Carece de conexiones\n", node->word);
        return edge_focus_print(node->next_node);
    }
    connections_print(node->first_connection, node->word); // Se imprime la informacion de cada arista
    return edge_focus_print(node->next_node); // Recusrsion al siguiente nodo
}

static void connections_print(struct edge *edge, char *word) { // Funcion para imprimir informacion de aristas / conexiones
    if (!edge) { // Se llego a la arista final
        return;
    }
    printf("[%s] ---> [%s] | %i conexiones | %.2f%% de probabilidad\n",word, edge->node->word, edge->occurrences, edge->probability*100);
    return connections_print(edge->next_edge, word); // Recursion a la siguiente arista
}
// FIN: Impresion del grafo

// INICIO: Funciones para liberar memoria del grafo
void stalin_graph(struct graph *graph) { // Funcion encargada de liberar la memoria del grafo
    if (!graph) { // El grafo esta vacio
        puts("No hay grafo por liberar");
        return;
    }
    stalin_node(graph->start); // Se libera la memoria de los nodos del grafo
    graph->start = NULL; // Se actualiza las posiciones del grafo por seguridad
    graph->end = NULL;
    free(graph->stack); // Se libera el stack de palabras
    free(graph); // Se libera el grafo de manera general
}

static void stalin_node(struct node *node) { // Funcion para liberar la memoria de cada nodo
    if (!node) { // Se llego al nodo final
        return;
    }
    struct node *next_node = node->next_node; // Se guarda de manera temporal la posicion del siguiente nodo
    free(node->word); // Se libera la memoria dinamica de la palabra
    stalin_edge(node->first_connection); // Se liberan todas las conexiones realizadas
    free(node); // Se libera al nodo actual
    stalin_node(next_node); // Recursion al siguiente nodo
}

static void stalin_edge(struct edge *edge) { // Funcion para liberar la memoria de todas las conexiones
    if (!edge) { // Ya no hay mas conexiones
        return;
    }
    struct edge *next_edge = edge->next_edge; // Se guarda de manera temporal la siguiente conexion
    free(edge); // Se libera la conexion
    return stalin_edge(next_edge); // Se avanza a la siguiente conexion
}
// FIN: Funciones para liberar memoria del grafo