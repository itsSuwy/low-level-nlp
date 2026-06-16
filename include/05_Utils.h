//
// Created by suwy on 6/2/26.
//

#ifndef LOW_LEVEL_NLP_05_UTILS_H
#define LOW_LEVEL_NLP_05_UTILS_H

// Parametros globales para compartir con otros archivos.c/.h
#define RESET "\033[0m" // Parametro para resetar el formato de la terminal
#define BOLD "\033[1m" // Parametro para volver el output de la terminal en negrita

// Diferentes colores que puede mostrar la terminal
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

void menu(void); // Unica funcion global compartida con el resto de archivos.c/.h (Aunque solo se usa en 00_Main.c)

#endif //LOW_LEVEL_NLP_05_UTILS_H
