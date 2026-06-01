//
// Created by suwy on 5/25/26.
//

#include <stdio.h>
#include "../include/01_Reading.h"
#include "../include/02_Node-STR.h"

void impresion(struct node *node);
void impresion_edges(struct edge *edge);

int main(int argc, char *argv[]) {
    struct graph *graph = initialise_graph();
    reading_file(graph);
    impresion(graph->start);

    return 0;
}

void impresion(struct node *node) {
    if (!node) {
        return;
    }
    printf("Palabra asignada: %s\n", node->word);
    printf("Apariciones: %i\n", node->occurrences);
    printf("Apunta a:\n");
    impresion_edges(node->first_connection);
    printf("\n");
    impresion(node->next_node);
}

void impresion_edges(struct edge *edge) {
    if (!edge) {
        return;
    }
    printf("\t%s\n", edge->node->word);
    printf("\tSe repite %i veces\n", edge->occurrences);
    printf("\n");
    impresion_edges(edge->next_edge);
}