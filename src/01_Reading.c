//
// Created by suwy on 5/25/26.
//

#include "../include/01_Reading.h"

#define max_size 46

// Electroencefalografista <- 25 letters
// Anticonstitucionalísimamente <- 29 letters
// Pneumonoultramicroscopicsilicovolcanoconiosis <- 45 letters

void reading_file(struct graph *graph) {
    //FILE *fp = fopen("../texts/lamigala.txt", "r");
    //FILE *fp = fopen("../texts/Prueba.txt", "r");
    //FILE *fp = fopen("../texts/Prueba_2.txt", "r");
    //FILE *fp = fopen("../texts/Prueba_3.txt", "r");
    FILE *fp = fopen("../texts/Prueba_4.txt", "r");

    if (!fp) {
        puts("El archivo no existe");
        puts("Cerrando el programa por precaucion");
        exit(-1);
    }
    char *line = NULL;
    size_t len = 0;
    while (getline(&line,&len,fp) != -1) {
        char *string = word_package();
        extraction(line, string, '\0', graph);
    }
    free(line);
    fclose(fp);
}

char *word_package(void) { // Se crea con memoria dinamica un apuntador que contendra los caracteres
    char *string = (char *)calloc(2, sizeof(char)); // Se inicializa con 2 espacios, uno para el caracter y otro para \0
    if (!string) {
        puts("Memory problem");
        exit(-1);
    }
    return string;
}

void extraction(char *original_string, char *word, char character, struct graph *graph) {
    if (*original_string == '\0') { // La linea llego a su fin
        if (word[0] != '\0') {
            aduana(graph, word);
        }
        aduana(graph, NULL);
        free(word);
        return;
    }
    int n = 0;
    n = validation(original_string, word);
    switch (n) {
        case 0: { // La palabra esta completa y requiere subirse
            aduana(graph, word);
            free(word);
            char *new_word = word_package();
            extraction(original_string+1, new_word, '\0', graph);
            break;
        }
        case 1: { // La palabra esta vacia y no hace falta subirla
            extraction(original_string+1, word, '\0', graph); // Se reiniciar el valor del caracter
            break;
        }
        case 2: { // La palabra esta llena pero aun puede expandirse mas
            character = pop(original_string);
            char *modified_word = merge(word, character);
            extraction(original_string+1, modified_word, character, graph);
            break;
        }
        case 4: // La palabra fue cortada por un signo de puntuacion
            aduana(graph, word);
            aduana(graph, NULL);
            free(word);
            char *new_word = word_package();
            extraction(original_string+1, new_word, '\0', graph);
            break;
        case 5: { // Comportamiento para letras especiales hispanas
            char *spanish_word = special_word();
            spanish_word = fill_special_word(original_string, spanish_word);
            char *modified_word = spanish_merge(word,spanish_word);
            free(spanish_word);
            extraction(original_string+2,modified_word,character,graph);
            break;
        }
        default:
            exit(-1);
    }
}

char *spanish_merge(char *word, char *spanish_character) {
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

unsigned char spanish_mayus_to_minus(unsigned char letter) {
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
}

char *fill_special_word(char *original_string, char *spanish_word) {
    spanish_word[0] = (char)*original_string;
    unsigned char auxiliar = '\0';
    auxiliar = (unsigned char)*(original_string+1);
    auxiliar = spanish_mayus_to_minus(auxiliar);
    spanish_word[1] = (char)auxiliar;
    spanish_word[2] = '\0';
    return spanish_word;
}

char *special_word(void) {
    char *word = (char *)calloc(3, sizeof(char)); // Se le asignan tres espacios
    if (!word) {
        exit(-1);
    }
    return word;
}

char pop(char *original_string) { // Se extrae el caracter de la linea en la posicion actual
    char value = '\0';
    value = *original_string;
    return value;
}

int validation(char *original_string, char *word) { // En caso de que se detecte un signo de puntuacion, se corta la imagen
    if ((unsigned char)*original_string == 0xC3) { // Se detecto una letra nativa del espaniol
        return 5;
    }
    if (*original_string == ' ') { // Se corta la palabra pero esta aun apunta a la siguiente
        if (word[0] != '\0') { // La palabra tiene contenido en su interior
            return 0;
        }else{
            return 1; // La palabra no tiene contenido en su interior
        }
    }
    if (*original_string == ',' || *original_string == '.' || *original_string == ';' || *original_string == ':' || *original_string == '?' || *original_string == '!' || *original_string == '\n') {
        if (word[0] != '\0') { // Se corta la palabra pero esta ya NO apunta a la siguiente
            return 4;
        }
        return 1; // La palabra esta vacia, no hace falta subirla al grafo
    }
    return 2; // No se encontro un signo de puntuacion lo que indica que la palabra puede crecer
}

char *merge(char *word, char character) { // Funcion para expandir el tamanio de nuestro apuntador original y asi pueda almacenar mas caracteres
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