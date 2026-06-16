//
// Created by suwy on 5/25/26.
//

#ifndef LOW_LEVEL_NLP_01_READING_H
#define LOW_LEVEL_NLP_01_READING_H

// Bibliotecas necesarias
#include "../include/02_Node-STR.h"
#include <stdio.h>

// Constantes publicas
#define READY_TO_PUSH 0
#define WORD_EMPTY 1
#define EXPANDIBLE_WORD 2
#define CUT_WORD 3
#define SPANISH_WORD 4
#define SPANISH_TWO_BYTES_DIGIT 5
#define SPANISH_THREE_BYTES_DIGITS 6

// Funciones publicas
void reading_file(FILE *source, struct graph *graph);
char *word_package(void);
char *special_word(void);
int validation_V2(char *original_string, char *word);
int spanish_special_characters(char *original_string);
char pop(char *original_string);
char *merge(char *word, char character);
char *fill_special_word(char *original_string, char *spanish_word);
char *spanish_merge(char *word, char *spanish_character);
unsigned char spanish_mayus_to_minus(unsigned char letter);

#endif //LOW_LEVEL_NLP_01_READING_H
