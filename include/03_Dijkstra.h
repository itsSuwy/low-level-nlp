//
// Created by suwy on 5/25/26.
//

#ifndef LOW_LEVEL_NLP_03_DIJKSTRA_H
#define LOW_LEVEL_NLP_03_DIJKSTRA_H

#include <stdbool.h>

struct dijkstra {
    struct dijkstra *next;
    struct node *current_node;
    struct node *previous_node;
    float probability;
    bool visited;
};

void Dijkstra(struct graph *graph, char *word);

#endif //LOW_LEVEL_NLP_03_DIJKSTRA_H
