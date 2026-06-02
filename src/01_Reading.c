//
// Created by suwy on 5/25/26.
//

#include "../include/01_Reading.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#define max_size 46

#define READY_TO_PUSH 0
#define WORD_EMPTY 1
#define EXPANDIBLE_WORD 2
#define CUT_WORD 3
#define SPANISH_WORD 4
#define SPANISH_TWO_BYTES_DIGIT 5
#define SPANISH_THREE_BYTES_DIGITS 6

// Electroencefalografista <- 25 letters
// Anticonstitucionalísimamente <- 29 letters
// Pneumonoultramicroscopicsilicovolcanoconiosis <- 45 letters

char *word_package(void);
char *special_word(void);
void extraction(char *original_string, char *word, struct graph *graph);
int validation_V2(char *original_string, char *word);
char pop(char *original_string);
char *merge(char *word, char character);
char *spanish_merge(char *word, char *spanish_character);
unsigned char spanish_mayus_to_minus(unsigned char letter);
char *fill_special_word(char *original_string, char *spanish_word);
int spanish_special_characters(char *original_string);

void reading_file(struct graph *graph) {
    FILE *fp = fopen("../texts/Prueba_5.txt", "r");

    if (!fp) {
        puts("El archivo no existe");
        puts("Cerrando el programa por precaucion");
        exit(-1);
    }
    char *line = NULL;
    size_t len = 0;
    while (getline(&line,&len,fp) != -1) {
        char *string = word_package();
        extraction(line, string, graph);
    }
    free(line);
    fclose(fp);
}

// START: Formatos para guardar letras
char *word_package(void) { // Enfocado a letras NORMALES, reserva espacio para su estructura normal
    char *string = (char *)calloc(2, sizeof(char)); // Se inicializa con 2 espacios, uno para el caracter y otro para \0
    if (!string) {
        puts("Memory problem");
        exit(-1);
    }
    return string;
}

char *special_word(void) { // Enfocado a letras HISPANAS, reserva espacio para su estructura en hexadecimal
    char *word = (char *)calloc(3, sizeof(char));
    if (!word) {
        exit(-1);
    }
    return word;
}

// END: Formatos para guardar palabras

/*
   Funcion recursiva encargada de recorrer caracter por caracter del arreglo,
   controlando los posibles estados que pueda presentar el codigo respecto a los datos que procesa
*/
void extraction(char *original_string, char *word, struct graph *graph) {
    if (*original_string == '\0') { // La linea llego a su fin
        if (word[0] != '\0') {
            aduana(graph, word);
        }
        aduana(graph, NULL);
        free(word);
        return;
    }
    int n = 0, illegal_character = 0;
    illegal_character = spanish_special_characters(original_string);
    n = validation_V2(original_string, word);
    switch (n) {
        case READY_TO_PUSH: { // La palabra esta completa y requiere subirse
            aduana(graph, word);
            free(word);
            char *new_word = word_package();
            return extraction(original_string+1, new_word, graph);
        }
        case WORD_EMPTY: { // La palabra esta vacia y no hace falta subirla
            if (illegal_character > 0) { // En caso de que el cut haya sido provocado por un signo especial del espaniol
                return extraction(original_string+illegal_character, word, graph); // Se omiten los caracteres especiales (pueden ser 2 o tres)
            }
            return extraction(original_string+1, word, graph);
            break;
        }
        case EXPANDIBLE_WORD: { // La palabra esta llena pero aun puede expandirse mas
            char character = '\0';
            character = pop(original_string);
            char *modified_word = merge(word, character);
            return extraction(original_string+1, modified_word, graph);
        }
        case CUT_WORD: {
            // La palabra fue cortada por un signo de puntuacion
            aduana(graph, word);
            aduana(graph, NULL);
            free(word);
            char *new_word = word_package();
            if (illegal_character > 0) { // En caso de que el cut haya sido provocado por un signo especial del espaniol
                return extraction(original_string+illegal_character, new_word, graph); // Se omiten los caracteres especiales (pueden ser 2 o tres)
            }
            return extraction(original_string+1, new_word, graph);
        }
        case SPANISH_WORD: { // Comportamiento para letras especiales hispanas
            char *spanish_word = special_word();
            spanish_word = fill_special_word(original_string, spanish_word);
            char *modified_word = spanish_merge(word,spanish_word);
            free(spanish_word);
            return extraction(original_string+2,modified_word,graph);
        }
        default:
            exit(-1); // Esto nunca se deberia cumplir, pero como freno de seguridad se asigna
    }
}

int validation_V2(char *original_string, char *word) { // Valida posibles escenarios de la palabra
    if ((unsigned char)*original_string == 0xC3) { // En caso de que se detecte que es una letra hispana
        return SPANISH_WORD;
    }
    if ((unsigned char)*original_string == 0XC2 || (unsigned char)*original_string == 0xE2) { //
        if (word[0] != '\0') {
            return CUT_WORD; // Se debe cortar la frase
        }else {
            return WORD_EMPTY;
        }
    }
    switch (*original_string) {
        case ' ':
            if (word[0] != '\0') {
                return READY_TO_PUSH; // Se puede subir la palabra al grafo
            }else {
                return WORD_EMPTY; // La palabra carece de informacion para subirse al grafo
            }
        // En caso de que se detecten signos que indiquen que la palabra no conecta con otra
        case '.':
        case ',':
        case '/':
        case '?':
        case '!':
        case ':':
        case ';':
        case '{':
        case '}':
        case '[':
        case ']':
        case '(':
        case ')':
        case '=':
        case '-':
        case '%':
        case '^':
        case '$':
        case '#':
        case '@':
        case '"':
        case '~':
        case '`':
        case '<':
        case '>':
        case '_':
        case '*':
        case '&':
        case '+':
        case '\\': // <- Referencia la '\'
        case '|':
        case '\t':
        case '\n':
            if (word[0] != '\0') {
                return CUT_WORD; // La palabra esta llena y lista para subir al grafo
            }else {
                return WORD_EMPTY;
            }
        default:
            return EXPANDIBLE_WORD; // No se cumplio ningun caso, indica que la palabra puede recibir mas letras
    }
}

int spanish_special_characters(char *original_string) {
    if ((unsigned char)*original_string == 0xC2) { // <- Signos de apertura detectados
        return 2; // La funcion debera saltar 2 bytes
    }
    if ((unsigned char)*original_string == 0xE2) {
        return 3; // Se deberan saltar 3 bytes
    }
    return 0;
}
//START: Logica para caracteres normales

char pop(char *original_string) { // Se encarga de extraer y retornar un solo caracter del string original
    char value = '\0';
    value = *original_string;
    return value;
}

char *merge(char *word, char character) { // Agrega el caracter extraido del string original al string de la palabra
    char lower_character = '\0';
    lower_character = (char)tolower(character);
    if (word[0] != '\0' ) { // En caso de que en la posicion 1 ya tenga un caracter
        word = (char *)realloc(word, (strlen(word) + 2) *sizeof(char)); // Se inicializa con dos espacios, uno para el nuevo caracter y otro para \0
        if (!word) {
            exit(-1);
        }
        int i = 0;
        i = strlen(word); // Se calcula la longitud de la cadena actual
        word[i] = lower_character; // Se inserta el nuevo caracter en la penultima posicion
        word[i+1] = '\0'; // En la ultima posicion se agrega el \0
        return word;
    }
    *word = lower_character; // En la primera posicion del apuntador se guarda el caracter
    return word;
}

// END: Logica para caracteres normales

// START: Logica para caracteres hispanos

char *fill_special_word(char *original_string, char *spanish_word) { // Se encarga de llenar el arreglo dinamico con codigo en hexadecimal
    spanish_word[0] = (char)*original_string;
    unsigned char auxiliar = '\0';
    auxiliar = (unsigned char)*(original_string+1);
    auxiliar = spanish_mayus_to_minus(auxiliar);
    spanish_word[1] = (char)auxiliar;
    spanish_word[2] = '\0';
    return spanish_word;
}

unsigned char spanish_mayus_to_minus(unsigned char letter) { // Secuencia de posibles combinaciones de caracteres hispanos
    switch (letter) {
        case 0x91: // Ñ
            return 0xb1; // ñ minuscula
        case 0x81: // Á con acento
            return 0xa1; // á minuscula con acento
        case 0x84: // Ä con dieresis
            return 0xa4; //	ä minuscula con dieresis
        case 0x89: // É con acento
            return 0xa9; // é minuscula con acento
        case 0x8b: // Ë con dieresis
            return 0xab; // ë minuscula con dieresis
        case 0x8d: // Í con acento
            return 0xad; // í minuscula con acento
        case 0x8f: // Ï con dieresis
            return 0xaf; // ï minuscula con dieresis
        case 0x93: // Ó con acento
            return 0xb3; // ó minuscula con acento
        case 0x96: // Ö con dieresis
            return 0xb6; // ö minuscula con dieresis
        case 0x9a: // Ú con acento
            return 0xba; // ú minuscula con acento
        case 0x9c: // Ü con dieresis
            return 0xbc; // ü minuscula con dieresis
        default:
            return letter; // No hace falta modificar la letra
    }
} // Fuente: https://docs.genexus.com/en/wiki?10513,Unicode%2FUTF-8-Character+table

char *spanish_merge(char *word, char *spanish_character) { // Anexa la letra especial a la palabra original
    int size = 0;
    size = strlen(word);
    if (size == 0) { // Nuestra palabra esta vacia
        word = (char *)realloc(word, 3 *sizeof(char)); // Solo se le agrega un espacio mas a nuestro arreglo original
        if (!word) {
            exit(-1);
        }
        strcpy(word, spanish_character);
        return word;
    }
    word = (char *)realloc(word, (size+3) *sizeof(char));
    if (!word) {
        exit(-1);
    }
    word[size] = *spanish_character;
    word[size+1] = spanish_character[1];
    word[size+2] = '\0';
    return word;
}