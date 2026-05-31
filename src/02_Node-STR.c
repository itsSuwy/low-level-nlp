//
// Created by suwy on 5/25/26.
//

#include "../include/02_Node-STR.h"
#include "../include/01_Reading.h"

// Start of initializations
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

struct edge *initialise_edge(void) { // The initialuzation of an edge
    struct edge *edge = (struct edge *)calloc(1,sizeof(struct edge));
    if (!edge) {
        exit(-1);
    }
    return edge;
}

void initialise_word_stack(struct graph *graph) {
    graph->stack = (struct word_stack *)calloc(1,sizeof(struct word_stack));
    if (!graph->stack) {
        exit(-1);
    }
    return;
}
/*
void assign_pointers(struct word_stack *stack, char *word) {
    int size_ptr = 0;
    if (!stack->first_word) {
        // Trabajamos con el apuntador a la primera palabra
    }else {
        // Trabajamos con el apuntador a la segunda palabra
    }
}

^/


// End of initializations of structures
/*
void initialize_pointers(struct graph *graph, struct auxiliar_words *auxiliar, char *word) {
    int size_ptr = 0, place = 0;
    size_ptr = strlen(word);
    place = which_way(auxiliar);
    switch (place) {
        case 1:{
            auxiliar->first_word = (char *)calloc(size_ptr+1,sizeof(char));
            strcpy(auxiliar->first_word,word);
            break;
        }
        case 2:{
            auxiliar->next_word = (char *)calloc(size_ptr+1,sizeof(char));
            strcpy(auxiliar->next_word,word);
            normal_push_to_graph(graph,auxiliar);
            break;
        }
    }
}

void normal_push_to_graph(struct graph *graph, struct auxiliar_words *auxiliar) {
    struct node *check = security_check(graph->start, auxiliar->first_word);
    if (!check) {
        graph->n_elements++;
        struct node *node_1 = filling_node(auxiliar->first_word);
        insert_element(graph,node_1);
        struct node *node_2 = next_word(graph,auxiliar->next_word);
    }else{
        update_occurrences(check);
        struct node *node_2 = next_word(graph, auxiliar->next_word);

    }
}

void connections(struct node *node_1, struct node *node_2){
    struct edge *edge = is_there_a_connection(node_1->connection, node_2->word);
    if (!edge) { // There is no connection
        struct edge *new_edge = initialise_edge();
        node_1->connection->last_edge->next_edge = new_edge;
    }else{
        edge->occurrences++;
    }
} // HE ARE HEEEEEEEERE

struct edge *is_there_a_connection(struct edge *edge, char *word_2) {
    if (!edge) {
        return NULL;
    }
    if (strcmp(word_2,edge->node->word)==0) {
        return edge;
    }
    return is_there_a_connection(edge->next_edge, word_2);
}

struct node *next_word(struct graph *graph, char *word) {
    struct node *check = security_check(graph->start, word);
    if (!check) {
        graph->n_elements++;
        struct node *node = filling_node(word);
        insert_element(graph,node);
        return node;
    }else {
        update_occurrences(check);
        return check;
    }
}

struct node *filling_node(char *word) {
    int size = 0;
    size = strlen(word);
    struct node *node = initialise_node();
    node->word = (char *)calloc(size+1,sizeof(char));
    if (!node->word) {
        exit(-1);
    }
    strcpy(node->word,word);
    node->occurrences++;
    //node->probability = (float)node->occurrences / graph->n_elements;
    return node;
}

void insert_element(struct graph *graph, struct node *node){
    if (!graph->start) {
        graph->start = node;
        graph->end = node;
    }else {
        graph->end->next = node;
        graph->end = node;
    }
}

struct node *security_check(struct node *node, char *word) {
    if (!node) {
        return NULL;
    }
    if (strcmp(node->word,word)==0) {
        return node;
    }
    return security_check(node->next, word);
}

void update_occurrences(struct node *node) {
    node->occurrences++;
}

void special_push_to_graph() {

}

void normal_kill(struct auxiliar_words *auxiliar){

    free(auxiliar->next_word);

}
*/