//
// Created by suwy on 5/25/26.
//
#include "../include/01_Reading.h"
#include "../include/03_Dijkstra.h"
#include "../include/02_Node-STR.h"
#include "../include/05_Utils.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define USER_WORD 1
#define REPEATED_WORD 2
#define PREDICTION_WORD 3

// START: Registros locales

struct dijkstra_base_needed {
    struct dijkstra *dijkstra;
    struct sequence_control *sequence_control;
    struct stack_nodes *stack;
};

struct sequence_control {
    struct sequence *start;
    struct sequence *end;
};

struct sequence {
    struct sequence *next;
    struct node *node;
    int type_of_word;
};
// END: Registros locales

// INICIO: Prototipos de funciones locales
static void order_sequence(struct sequence_control *sequence_control, struct node *node, int type_of_word);
static void send_to_Dijkstra(struct dijkstra_base_needed *Dijkstra_base, struct graph *graph);
static void new_aduana(struct node *node, struct dijkstra_base_needed *Dijkstra_base, struct graph *graph);
static void best_node(struct edge *currect_edge, struct node *node, struct stack_nodes *stack);
static struct node*highest_probability_node(struct node *current, struct node *best);
static void dijkstra_selection(struct node *node_destination, struct dijkstra_base_needed *Dijkstra_base, struct graph *graph);
static void reconstruction(struct node *node, struct sequence_control *sequence_control, int control_node);
static void path(struct node *node, struct sequence_control *sequence_control, int control_node);
static void dijkstra_algorithm(struct stack_nodes *stack, struct dijkstra_base_needed *Dijkstra_base, struct graph *graph);
static void path_construction(struct stack_nodes *stack, struct sequence_control *sequence_control);
static void reset_dijkstra(struct node *node);
static void Dijkstra_work(struct dijkstra_base_needed *Dijkstra_base, struct graph *graph);
static void extraction(char *original_string, char *word, struct graph *graph, struct dijkstra_base_needed *Dijkstra_base);
static struct edge *predict_next_word(struct edge *edge, struct edge *best_connection);
static bool prediction(struct node *node, struct sequence_control *sequence_control);
static void print_Dijkstra(struct sequence *sequence, bool success);
static void stalin_purge_Dijkstra(struct dijkstra *dijkstra);
static void stalin_purge(struct dijkstra_base_needed *Dijkstra_base, struct node *node);
static void control_flow(struct graph *graph, char *word, struct dijkstra_base_needed *Dijkstra_base);
static bool duplicated_word(struct sequence_control *sequence_control, struct node *node);
// FIN: Prototipos de funciones locales

// START: Inicializaciones
static void initialise_dijkstra(struct dijkstra_base_needed *needed) {
    needed->dijkstra = (struct dijkstra *)calloc(1,sizeof(struct dijkstra));
    if (!needed->dijkstra) {
        exit(-1);
    }
}

static void initialise_sequence_control(struct dijkstra_base_needed *needed) {
    needed->sequence_control = (struct sequence_control *)calloc(1,sizeof(struct sequence_control));
    if (!needed->sequence_control) {
        exit(-1);
    }
}

static void initialise_stack(struct dijkstra_base_needed *needed) {
    needed->stack = (struct stack_nodes *)calloc(1,sizeof(struct stack_nodes));
    if (!needed->stack) {
        exit(-1);
    }
}

static struct sequence *initialise_sequence_aux(void) {
    struct sequence *sequence = (struct sequence *)calloc(1,sizeof(struct sequence));
    if (!sequence) {
        exit(-1);
    }
    return sequence;
}

static void initialise_dijsktre_from_node(struct node *node) {
    node->Dijkstra_assigned = (struct dijkstra *)calloc(1,sizeof(struct dijkstra));
    if (!node->Dijkstra_assigned) {
        exit(-1);
    }
}

static void initialise_all_dijkstra(struct node *node) {
    if (!node) {
        return;
    }
    initialise_dijsktre_from_node(node);
    return initialise_all_dijkstra(node->next_node);
}

static struct dijkstra_base_needed *general_initialise(void) {
    struct dijkstra_base_needed *needed = (struct dijkstra_base_needed *)calloc(1,sizeof(struct dijkstra_base_needed));
    if (!needed) {
        exit(-1);
    }
    initialise_dijkstra(needed);
    initialise_sequence_control(needed);
    initialise_stack(needed);
    return needed;
}
// END: Inicializaciones

void Dijkstra(struct graph *graph, char *word) { //Funcion que envuelve toda la logica de DIjkstra
    char *ptr_word = word_package();
    struct dijkstra_base_needed *Dijkstra_base = general_initialise();
    extraction(word, ptr_word, graph, Dijkstra_base);
    bool success = prediction(Dijkstra_base->sequence_control->end->node, Dijkstra_base->sequence_control);
    print_Dijkstra(Dijkstra_base->sequence_control->start, success);
    stalin_purge(Dijkstra_base, graph->start);
}

static void extraction(char *original_string, char *word, struct graph *graph, struct dijkstra_base_needed *Dijkstra_base) { // Funcion que extrae las palabras del input del usuario
    if (*original_string == '\0') { // La linea llego a su fin
        if (word[0] != '\0') {
            control_flow(graph, word, Dijkstra_base);
        }
        control_flow(graph, NULL, Dijkstra_base);
        free(word);
        return;
    }
    int n = 0, illegal_character = 0;
    illegal_character = spanish_special_characters(original_string);
    n = validation_V2(original_string, word);
    switch (n) {
        case READY_TO_PUSH: { // La palabra esta completa y requiere subirse
            control_flow(graph,word,Dijkstra_base);
            free(word);
            char *new_word = word_package();
            return extraction(original_string+1, new_word, graph, Dijkstra_base);
        }
        case WORD_EMPTY: { // La palabra esta vacia y no hace falta subirla
            if (illegal_character > 0) { // En caso de que el cut haya sido provocado por un signo especial del espaniol
                return extraction(original_string+illegal_character, word, graph,Dijkstra_base); // Se omiten los caracteres especiales (pueden ser 2 o tres)
            }
            return extraction(original_string+1, word, graph,Dijkstra_base);
            break;
        }
        case EXPANDIBLE_WORD: { // La palabra esta llena pero aun puede expandirse mas
            char character = '\0';
            character = pop(original_string);
            char *modified_word = merge(word, character);
            return extraction(original_string+1, modified_word, graph,Dijkstra_base);
        }
        case CUT_WORD: {
            // La palabra fue cortada por un signo de puntuacion
            control_flow(graph, word, Dijkstra_base);
            control_flow(graph, NULL, Dijkstra_base);
            free(word);
            char *new_word = word_package();
            if (illegal_character > 0) { // En caso de que el cut haya sido provocado por un signo especial del espaniol
                return extraction(original_string+illegal_character, new_word, graph,Dijkstra_base); // Se omiten los caracteres especiales (pueden ser 2 o tres)
            }
            return extraction(original_string+1, new_word, graph,Dijkstra_base);
        }
        case SPANISH_WORD: { // Comportamiento para letras especiales hispanas
            char *spanish_word = special_word();
            spanish_word = fill_special_word(original_string, spanish_word);
            char *modified_word = spanish_merge(word,spanish_word);
            free(spanish_word);
            return extraction(original_string+2,modified_word,graph,Dijkstra_base);
        }
        default:
            exit(-1); // Esto nunca se deberia cumplir, pero como freno de seguridad se asigna
    }
}

static void control_flow(struct graph *graph, char *word, struct dijkstra_base_needed *Dijkstra_base) { // Se encarga de administrar las palabras que entran a Dijkstra
    if (!word) { // En caso de que la palabra haya sido NULL
        bool duplicated = duplicated_word(Dijkstra_base->sequence_control, Dijkstra_base->stack->first_node); // Evalua si la primera palabra ya existia en la secuencia
        if (duplicated == false) { // En caso de que no exista se agrega
            order_sequence(Dijkstra_base->sequence_control, Dijkstra_base->stack->first_node, USER_WORD); // Se ingresa la palabra en formto de palabra de usuario
            reset_stack(Dijkstra_base->stack); // Se reiniciar el stack
        }
        return;
    }
    struct node *node = word_repeated(graph->start, word); // Busca en el grafo si ya existe un nodo con esa palabra asignada
    if (!node) { // En caso de no existir la crea
        node = initialise_node();
        node = filling_node(node, word);
        graph->n_elements++;
        list_of_nodes(graph, node);
    }
    new_aduana(node, Dijkstra_base, graph); // Envia el nodo a la nueva aduana
}

static void order_sequence(struct sequence_control *sequence_control, struct node *node, int type_of_word) { // Funcion que inserta la secuencia de palabras para la impresion final
    if (!node) { // En caso de recibir una palabra NULL no hace nada por seguridad
        return;
    }
    struct sequence *sequence = initialise_sequence_aux(); // Genera una elista de tipo sequence
    sequence->node = node;
    sequence->type_of_word = type_of_word; // Indica el tipo de palabra en la secuencia: Si fue del usuario o generada via Dijkstra
    if (!sequence_control->start) { // Carece de elementos asignados
        sequence_control->start = sequence;
        sequence_control->end = sequence;
        return;
    }
    sequence_control->end->next = sequence;
    sequence_control->end = sequence;
    // Se puede refactorizar, pero por tiempo no le movere ya. TODO
}

static void new_aduana(struct node *node, struct dijkstra_base_needed *Dijkstra_base, struct graph *graph) { // Funcion que almacena en el contenedor los nodos que llegan
    if (!Dijkstra_base->stack->first_node) { // Es la primera palabra
        Dijkstra_base->stack->first_node = node;
        return;
    }
    if (!Dijkstra_base->stack->second_node) { // Es la segunda palabra
        Dijkstra_base->stack->second_node = node;
        send_to_Dijkstra(Dijkstra_base, graph); // Se envian los elementos que Dijkstra necesita para trabajar
        change_values(Dijkstra_base->stack); // Se resetean los valores del contenedor (stack)
    }
}

static void send_to_Dijkstra(struct dijkstra_base_needed *Dijkstra_base, struct graph *graph) { // Funcion que recibe parametros para que funcione DIjkstra
    if (!Dijkstra_base->stack->first_node->first_connection) { // No tiene ningun tipo de conexion, no vale la pena usar Dijkstra
        new_edge(Dijkstra_base->stack->first_node, Dijkstra_base->stack->second_node); // Se forza una conexion entre palabras
        bool duplicated = duplicated_word(Dijkstra_base->sequence_control, Dijkstra_base->stack->first_node);
        if (duplicated == false) {
            order_sequence(Dijkstra_base->sequence_control, Dijkstra_base->stack->first_node, USER_WORD);
        }
        return;
    }
    Dijkstra_work(Dijkstra_base,graph); // Inicia la logica de dijkstra
}

static void Dijkstra_work(struct dijkstra_base_needed *Dijkstra_base, struct graph *graph) { // Funcion que encapsula todas las funciones que requiere DIjkstra para funcionar
    initialise_all_dijkstra(graph->start); // Inicializa los parametros de Dijkstra dentro de cada nodo
    dijkstra_algorithm(Dijkstra_base->stack, Dijkstra_base, graph); // Creacion del camino mas optimo de DIjkstra
    path_construction(Dijkstra_base->stack, Dijkstra_base->sequence_control); // Se reinvierte el orden de la salida de las palabras
    reset_dijkstra(graph->start); // Se reinician los parametros de DIjkstra
}

static void dijkstra_algorithm(struct stack_nodes *stack, struct dijkstra_base_needed *Dijkstra_base, struct graph *graph) { // Funcion que activa los parametros del nodo de origen para enviarselos al algoritmo
    stack->first_node->Dijkstra_assigned->probability = 1; // Se indica que es el nodo de origen
    stack->first_node->Dijkstra_assigned->previous_node = NULL; // Se indica que antes de el no hay otro nodo
    dijkstra_selection(stack->second_node, Dijkstra_base, graph); // Funcion que reconstruye el camino de DIjkstra
}

static void dijkstra_selection(struct node *node_destination, struct dijkstra_base_needed *Dijkstra_base, struct graph *graph) { // Funcion encargada de encontrar el mejor camino
    struct node *node = highest_probability_node(graph->start, NULL); // Busca el nodo con mayor probabilidad que no haya sido visitado
    if (!node || node_destination->Dijkstra_assigned->visited == true) { // Evalua si encontro el nodo destino o si ya no hay mas nodos
        return;
    }
    node->Dijkstra_assigned->visited = true; // Marca el nodo actual como ya visitado
    best_node(node->first_connection, node, Dijkstra_base->stack);
    dijkstra_selection(node_destination, Dijkstra_base, graph); // Recursion
}

static struct node*highest_probability_node(struct node *current, struct node *best) { // Funcion que busca al nodo con la mayor probabilidad segun Dijkstra
    if (!current) {
        return best;
    }
    if (current->Dijkstra_assigned->visited == false) {
        if (!best) {
            best = current;
        }
        else if (current->Dijkstra_assigned->probability > best->Dijkstra_assigned->probability) { // En caso de que la probabilidad fue mejor
            best = current;
        }
    }
    return highest_probability_node(current->next_node, best);
}

// best_node -> Evalua todas las conexiones de un nodo y determina si existe una ruta con una probabilidad acumulada mejor que la registrada actualmente
static void best_node(struct edge *currect_edge, struct node *node, struct stack_nodes *stack) {
    if (!currect_edge) { // Ya no quedan conexiones por revisar
        return;
    }
    if (stack->second_node->Dijkstra_assigned->visited == true) { // Si el nodo destino ya fue visitado, no tiene sentido seguir buscando mas aristas
        return;
    }
    float probability = 0;
    probability = node->Dijkstra_assigned->probability * currect_edge->probability; // Calcula la probabilidad de la arista actual con la del nodo de Dijkstra
    if (probability > currect_edge->node->Dijkstra_assigned->probability) { // Si la probabilidad fue mejor actualiza el camino de Dijkstra
        currect_edge->node->Dijkstra_assigned->probability = probability;
        currect_edge->node->Dijkstra_assigned->previous_node = node;
    }
    return best_node(currect_edge->next_edge, node, stack);
}

static void path_construction(struct stack_nodes *stack, struct sequence_control *sequence_control) { // Funcion encargada de controlar el camino que se pudo haber generado por Dijkstra
    if (stack->second_node->Dijkstra_assigned->probability == 0) { // En caso de que Dijkstra no encontro conexiones, se fuerza una conexion directa
        new_edge(stack->first_node, stack->second_node);
        bool duplicated = duplicated_word(sequence_control, stack->first_node);
        if (duplicated == false) { // Si la palabra aun no aparece en la secuencia de impresion
            order_sequence(sequence_control, stack->first_node, USER_WORD);
        }
        return;
    }
    path(stack->second_node, sequence_control, 0); // Reconstruye e inserta el camino encontrado por Dijkstra
}

static void path(struct node *node, struct sequence_control *sequence_control, int control_node) { // Funcion encargada de recorrer el camino generado por Dijkstra
    if (!node) {
        return;
    }
    path(node->Dijkstra_assigned->previous_node, sequence_control, control_node+1); // Recorre recursivamente el camino desde el origen hasta el destino
    reconstruction(node, sequence_control, control_node); // Recibe el nodo y lo organiza por medio de LIFO
}

static void reconstruction(struct node *node, struct sequence_control *sequence_control, int control_node) { // Funcion encargada de organizar la secuencia de impresion del camino por DIjkstra
    if (sequence_control->end && strcmp(sequence_control->end->node->word, node->word) == 0){ // Evita insertar dos veces seguidas la misma palabra
        return;
    }
    struct sequence *sequence = initialise_sequence_aux(); // Se genera un registro que almacena al nodo, el siguiente registro y el tripo de dato para impresion
    sequence->node = node;

    if (!node->Dijkstra_assigned->previous_node) { // Es el nodo origen del camino generado por Dijkstra
        sequence->type_of_word = USER_WORD;
    }
    else if (control_node == 0) { // Es la primera iteracion de la recursion, por ende es la palabra B ingresada por el usuario
        sequence->type_of_word = USER_WORD;
    }else {
        sequence->type_of_word = REPEATED_WORD; // Forma parte de las palabras intermedias entre A y B
    }
    if (!sequence_control->start) { // Evalua si la secuencia de control esta vacia
        sequence_control->start = sequence; // Inserta ambos al inicio y al final
        sequence_control->end = sequence;
        return;
    }
    sequence_control->end->next = sequence; // De lo contrario inserta los elementos al final
    sequence_control->end = sequence;
}

static void reset_dijkstra(struct node *node) { // Funcion encargada de liberar la memoria asignada para el apartado de Dijkstra que contiene el nodo
    if (!node) {
        return;
    }
    free(node->Dijkstra_assigned);
    node->Dijkstra_assigned = NULL; // Asi se evita contener basura o caminos de anteriores iteraciones
    return reset_dijkstra(node->next_node);
}

static bool prediction(struct node *node, struct sequence_control *sequence_control) { // Funcion para evaluar si se puede predecir la siguiente palabra
    if (!node->first_connection) { // En caso de que el ultimo dentro de la secuencia no tenga conextiones
        return false; // No se pudo generar prediccion
    }
    if (!sequence_control->start || !sequence_control->end) { // En caso de que la secuencia de control haya quedado vacia que se supone no deberia pasar
        exit(-1); // Se cierra por seguridad el programa
    }
    struct edge *edge = predict_next_word(node->first_connection, node->first_connection); // Se selecciona la conexion con mayor probabilidad de la frase
    struct sequence *sequence = initialise_sequence_aux();
    sequence->node = edge->node; // Se asigna la frase predecida
    sequence->type_of_word = PREDICTION_WORD; // Junto a un indicador de prediccion
    sequence_control->end->next = sequence;
    sequence_control->end = sequence;
    return true; // Se indica que si se pudo predecir la siguiente frase
}

static struct edge *predict_next_word(struct edge *edge, struct edge *best_connection) { // Funcion que busca la conexion con mayor probabilidad
    if (!edge) { // Se retorna la mejor conexion
        return best_connection;
    }
    if (edge->probability > best_connection->probability) { // Evalua si la conexion actual es mejor
        best_connection = edge;
    }
    return predict_next_word(edge->next_edge, best_connection);
}

static void print_Dijkstra(struct sequence *sequence, bool success) { // Funcion que imprime la secuencia generada por Dijkstra junto a los colores correspondientes
    if (!sequence) { // Se llego al final
        if (success == true) {
            puts(YELLOW "<-" RESET " " YELLOW "[" RESET MAGENTA "Palabra predecida" RESET YELLOW "]" RESET);
        }else{
            printf("|"RED " No se pudo predecir una frase" RESET);
        }
        printf("\n");
        puts("|---> "BOLD GREEN "Verde" RESET ": Palabra ingresada por el usuario");
        puts("|---> "BOLD RED"Rojo" RESET ": Palabras / frases sugeridas");
        puts("|---> "BOLD CYAN"Cyan" RESET ": Palabra predecida");
        printf("\n");
        return;
    }
    switch (sequence->type_of_word) { // Imprime el formato segun el tipo de palabra que fue
        case REPEATED_WORD:
            printf(RED "%s " RESET, sequence->node->word);
            break;
        case PREDICTION_WORD:
            printf(CYAN "%s " RESET, sequence->node->word);
            break;
        case USER_WORD:
            printf(GREEN "%s " RESET, sequence->node->word);
            break;
        default:
            puts("Error numero 52");
            exit(-1);
    }
    return print_Dijkstra(sequence->next, success);
}

static void stalin_purge_sequence(struct sequence *sequence) {
    if (!sequence) {
        return;
    }
    struct sequence *next_sequence = sequence->next;
    free(sequence);
    return stalin_purge_sequence(next_sequence);
}

static void stalin_purge_sequence_control(struct sequence_control *sequence_control) {
    stalin_purge_sequence(sequence_control->start);
    free(sequence_control);
}

static void stalin_purge_Dijkstra_from_node(struct node *node) {
    if (!node) {
        return;
    }
    stalin_purge_Dijkstra(node->Dijkstra_assigned);
    node->Dijkstra_assigned = NULL;
    stalin_purge_Dijkstra_from_node(node->next_node);
}

static void stalin_purge_Dijkstra(struct dijkstra *dijkstra) {
    if (!dijkstra) {
        return;
    }
    struct dijkstra *next_dijkstra = dijkstra->next;
    free(dijkstra);
    return stalin_purge_Dijkstra(next_dijkstra);
}

static void stalin_purge(struct dijkstra_base_needed *Dijkstra_base, struct node *node) {
    stalin_purge_sequence_control(Dijkstra_base->sequence_control);
    stalin_purge_Dijkstra_from_node(node);
    free(Dijkstra_base->dijkstra);
    free(Dijkstra_base->stack);
    free(Dijkstra_base);
}

static bool duplicated_word(struct sequence_control *sequence_control, struct node *node) { // Funcion encargada de borrar palabras
    if (!sequence_control->end || !node || !sequence_control->end) {
        return false;
    }
    if (strcmp(sequence_control->end->node->word, node->word) == 0) {
        return true;
    }
    return false;
}