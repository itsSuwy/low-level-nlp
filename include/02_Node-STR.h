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
};

struct node {
    char *word;
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


#endif //LOW_LEVEL_NLP_02_NODE_STR_H
