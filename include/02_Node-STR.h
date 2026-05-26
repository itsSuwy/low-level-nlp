//
// Created by suwy on 5/25/26.
//

#ifndef LOW_LEVEL_NLP_02_NODE_STR_H
#define LOW_LEVEL_NLP_02_NODE_STR_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct graph {
    int n_elements;
    struct node *start;
    struct node *end;
};

struct node {
    char *word;
    int occurrences;
    float probability;
    struct node *next;
    struct edge *connection;
};

struct edge {
    struct edge *next_edge;
    struct node *next_node;
    float probability;
};

struct auxiliar_words {
    char *first_word;
    char *next_word;
    bool kill;
};

struct auxiliar_words *auxiliar_str(void);
int security_check(struct node *node, char *word);
void node_to_graph(struct graph *graph, struct auxiliar_words *auxiliar);
void update_occurrences(struct node *node);
void insert_element(struct graph *graph, struct node *node);

#endif //LOW_LEVEL_NLP_02_NODE_STR_H
