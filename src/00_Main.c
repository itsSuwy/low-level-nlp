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
    graph_probability(graph);
    printf("Se registraron con exito %i palabras totales\n", graph->n_elements);
    impresion(graph->start);
    return 0;
}

void impresion(struct node *node) {
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

void impresion_edges(struct edge *edge) {
    if (!edge) {
        return;
    }
    printf("\t|-> (x%i) -> %s\n",edge->occurrences,edge->node->word);
    printf("\t\t|-> %.2f%% de probabilidad\n", edge->probability*100);
    impresion_edges(edge->next_edge);
}