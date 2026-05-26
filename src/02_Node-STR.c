//
// Created by suwy on 5/25/26.
//

#include "../include/02_Node-STR.h"
#include "../include/01_Reading.h"

#include <stdlib.h>

struct auxiliar_words *auxiliar_str(void) {
    struct auxiliar_words *auxiliar = (struct auxiliar_words*) calloc(1,sizeof(struct auxiliar_words));
    if (!auxiliar) {
        exit(-1);
    }
    return auxiliar;
}
