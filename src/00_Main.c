//
// Created by suwy on 5/25/26.
//

#include <stdio.h>
#include "../include/01_Reading.h"
#include "../include/02_Node-STR.h"

int main(int argc, char *argv[]) {
    struct graph *graph = initialise_graph();
    reading_file(graph);
    graph_probability(graph);
    print_graph(graph);
    return 0;
}