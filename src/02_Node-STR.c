//
// Created by suwy on 5/25/26.
//

#include "../include/02_Node-STR.h"
#include "../include/01_Reading.h"



struct auxiliar_words *auxiliar_str(void) {
    struct auxiliar_words *auxiliar = (struct auxiliar_words*) calloc(1,sizeof(struct auxiliar_words));
    if (!auxiliar) {
        exit(-1);
    }
    return auxiliar;
}

void initialize_pointers(struct auxiliar_words *auxiliar, char *word) {
    int size_ptr = 0, place = 0;
    size_ptr = strlen(word);
    place = which_way(auxiliar);
    switch (place) {
        case 1:{
            auxiliar->first_word = (char *)calloc(size_ptr+1,sizeof(char));
            strcpy(auxiliar->first_word,word);
            break;
        }
        case 2:{
            auxiliar->next_word = (char *)calloc(size_ptr+1,sizeof(char));
            strcpy(auxiliar->next_word,word);
            // Additional push
            break;
        }
    }
}