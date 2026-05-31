//
// Created by suwy on 5/25/26.
//

#ifndef LOW_LEVEL_NLP_01_READING_H
#define LOW_LEVEL_NLP_01_READING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/02_Node-STR.h"

void reading_file(struct graph *graph);
char *word_package(void);
void extraction(char *original_string, char *word, char character, struct graph *graph);
char pop(char *original_string);
char *merge(char *word, char character);
int validation(char *original_string, char *word);



#endif //LOW_LEVEL_NLP_01_READING_H
