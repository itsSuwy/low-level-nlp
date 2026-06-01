//
// Created by suwy on 5/25/26.
//

#ifndef LOW_LEVEL_NLP_01_READING_H
#define LOW_LEVEL_NLP_01_READING_H

#include "../include/02_Node-STR.h"

void reading_file(struct graph *graph);
char *word_package(void);
char *special_word(void);
void extraction(char *original_string, char *word, struct graph *graph);
int validation_V2(char *original_string, char *word);
char pop(char *original_string);
char *merge(char *word, char character);
char *spanish_merge(char *word, char *spanish_character);
unsigned char spanish_mayus_to_minus(unsigned char letter);
char *fill_special_word(char *original_string, char *spanish_word);

#endif //LOW_LEVEL_NLP_01_READING_H
