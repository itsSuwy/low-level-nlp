//
// Created by suwy on 5/25/26.
//
// Bibliotecas necesarias
#include "../include/01_Reading.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void extraction(char *original_string, char *word, struct graph *graph); // Unico prototipo de funcion local

void reading_file(FILE *source, struct graph *graph) { // Funcion que lee linea por linea un archivo.txt
    char *line = NULL; // Se inicializa en NULL el ptr de cada linea
    size_t len = 0;
    while (getline(&line,&len,source) != -1) { // Lee cada linea del archivo hasta que se tope con un EOF (-1)
        char *string = word_package(); // Se inicializa un contendor para cada letra de la palabra
        extraction(line, string, graph); // Funcion para recibir la linea completa convertida en string y extrae letra por letra
    }
    free(line); // Se libera la memoria dinamica asignada a la linea leida
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
    char *word = (char *)calloc(3, sizeof(char)); // Las letras admitidas tienen un largo de 2 bytes + 1 de '\0', requieren 3 bytes
    if (!word) { // Error de memoria
        exit(-1); // Se cierra el programa por seguiridad
    }
    return word;
}

// END: Formatos para guardar palabras

/*
   Funcion recursiva encargada de recorrer caracter por caracter del arreglo,
   controlando los posibles estados que pueda presentar el codigo respecto a los datos que procesa
*/
static void extraction(char *original_string, char *word, struct graph *graph) {
    if (*original_string == '\0') { // La linea llego a su fin
        if (word[0] != '\0') { // Si la palabra tenia caracteres asignados
            aduana(graph, word); // Se envia a la aduana
        }
        aduana(graph, NULL); // Aduana recibe la alerta de que es la palabra final -> no tiene conexiones
        free(word); // Se libera la memoria asignada a la palabra
        return;
    }
    int n = 0, illegal_character = 0;
    illegal_character = spanish_special_characters(original_string); // Se valida si en la posicion actual hay caracteres hispanos
    n = validation_V2(original_string, word); // Validation evalua a que caso corresponde la letra actual
    switch (n) {
        case READY_TO_PUSH: { // Se llego a un signo de puntuacion, la palabra esta completa y debe subirse al grafo pasando por la aduana
            aduana(graph, word);
            free(word); // Se libera la memoria asignada
            char *new_word = word_package(); // Se crea un nuevo contenedor
            return extraction(original_string+1, new_word, graph); // Se avanza una posicion en el string
        }
        case WORD_EMPTY: { // Se detecto un signo de puntuacion (normal o hispano) pero la palabra se encuentra vacia, no hace falta subirla al grafo
            if (illegal_character > 0) { // En caso de que el cut haya sido provocado por un signo especial del espaniol
                return extraction(original_string+illegal_character, word, graph); // Se omiten los caracteres especiales (puede ser 1 o 2) los espacios a saltar
            }
            return extraction(original_string+1, word, graph); // En caso de que no hubiera caracter hispano
            break;
        }
        case EXPANDIBLE_WORD: { // La palabra esta llena pero se puede expandir con la letra en la posicion actual porque no es signo de puntuacion
            char character = '\0';
            character = pop(original_string); // Se extrae la letra de la posicion actual
            char *modified_word = merge(word, character); // Se hace adjunta la letra a una nueva posicion dentro del contenedor actual
            return extraction(original_string+1, modified_word, graph); // Se avanza a la siguiente posicion
        }
        case CUT_WORD: { // La palabra fue cortada por un signo de puntuacion
            aduana(graph, word); // Se envia la palabra a la aduana
            aduana(graph, NULL); // La aduana ahora recibe un NULL -> No apunta a nadie mas la palabra actual
            free(word); // Se libera la memoria
            char *new_word = word_package();
            if (illegal_character > 0) { // En caso de que el cut haya sido provocado por un signo especial del espaniol
                return extraction(original_string+illegal_character, new_word, graph);
            }
            return extraction(original_string+1, new_word, graph);
        }
        case SPANISH_WORD: { // Comportamiento para letras especiales hispanas
            char *spanish_word = special_word(); // Se crea un contendor para almacenar la letra especial
            spanish_word = fill_special_word(original_string, spanish_word); // Se le asigna el caracter actual y siguiente del string
            char *modified_word = spanish_merge(word,spanish_word); // Se asigna a la palabra actual el caracter hispano
            free(spanish_word); // Se libera la memoria del contenedor especial
            return extraction(original_string+2,modified_word,graph); // Se omite la siguiente posicion
        }
        default: // Esto nunca se deberia cumplir, pero como freno de seguridad se asigna una salida
            exit(-1);
    }
}

int validation_V2(char *original_string, char *word) { // Valida posibles escenarios de la palabra
    if ((unsigned char)*original_string == 0xC3) { // En caso de que se detecte que es una letra hispana
        return SPANISH_WORD;
    }
    if ((unsigned char)*original_string == 0XC2 || (unsigned char)*original_string == 0xE2) { // Se detecta caracteres nativos del espaniol pero ilegales para el programa
        if (word[0] != '\0') { // En caso de que la palabra tenga informacion
            return CUT_WORD; // Se debe cortar la frase
        }else {
            return WORD_EMPTY;
        }
    }
    switch (*original_string) { // Evalua todos los caracteres ilegales
        case ' ':
            if (word[0] != '\0') {
                return READY_TO_PUSH; // Se puede subir la palabra al grafo
            }else {
                return WORD_EMPTY; // La palabra carece de informacion para subirse al grafo
            }
        // En caso de que se detecten signos que indiquen que la palabra no conecta con otra
        case '-':
            if (word[0] != '\0') {
                return EXPANDIBLE_WORD; // Se puede expandir
            }else {
                return WORD_EMPTY; // La palabra carece de informacion para subirse al grafo
            }
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
        //case '\n':
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