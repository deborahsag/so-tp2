#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct Report Report;
struct Report{
    int page_faults;
    int dirty_pages;
};


typedef struct Cell Cell;
struct Cell {
    unsigned addr;
    int page_size;
    int table_size;
    int max_table_size;
    Cell *prev;
    Cell *next;
    Cell *last;
};


Cell* init_frame() {
/* Inicia uma celula nova da lista */
    Cell *frame = malloc (sizeof(Cell));
    frame->prev = NULL;
    frame->next = NULL;
    frame->last = NULL;
    frame->table_size = 0;
    return frame;
}


void insert_end_list(unsigned addr, Cell* list) {
/* Insere um quadro dado seu endereco no fim da lista */
    Cell *frame = init_frame();
    frame->addr = addr;

    if (list->next == NULL) {
        list->next = frame;
        frame->prev = list;
    } else {
        list->last->next = frame;
        frame->prev = list->last;
    }

    list->last = frame;
    list->table_size += 1;
}


void remove_top_list(Cell* list) {
/* Elimina o quadro do topo da lista */
    Cell* first = list->next;
    list->next = list->next->next;
    list->next->prev = list;
    free(first);
}


void swap_for_new(Cell* old, Cell* new, Cell* list) {
/* Substitui um quadro na lista por outro, dado o quadro a ser substituido, o novo quadro e a lista duplamente encadeada */
    new->next = old->next;
    old->next = new;
    new->prev = old;

    if (new->next != NULL) {
        new->next->prev = new;
    }

    if (old->prev == NULL) {
        list = new;
    } else {
        old->prev->next = new;
    }

    if (old->next != NULL) {
        old->next->prev = new;
    }
    free(old);
}


int is_full(Cell* list) {
/* Retorna se a lista esta cheia */
    if (list->table_size >= list->max_table_size) return 1;
    else return 0;
}


Cell* search_list(unsigned addr, Cell* list) {
/* Retorna o quadro dado seu endereco */    
    Cell *frame = list;
    while(frame->next != NULL){
        frame = frame->next;
        if (frame->addr == addr){
            return frame;
        }
    }
    return NULL;
}


void print_linked_list(Cell* list) {
/* Imprime os enderecos da lista duplamente encadeada */
    printf("\nTabela:\n");
    Cell *frame = list;
    while(frame->next != NULL) {
        frame = frame->next;
        printf("%x\n", frame->addr);
    }
    printf("\n");
}


void print_backwards(Cell* list) {
/* Imprime os enderecos de tras para frente para testar a lista duplamente encadeada */
    printf("\nTabela de tras para frente:\n");
    Cell *frame = list->last;
    while(frame->prev != NULL) {
        printf("%x\n", frame->addr);
        frame = frame->prev;
    }
    printf("\n");
}


void free_list(Cell* list) {
/* Desaloca lista duplamente encadeada */
    Cell *aux = list;
    while(list != NULL) {
        aux = list;
        list = list->next;
        free(aux);
    }
}


unsigned page_addr(unsigned addr, int page_size) {
/* Identifica a pagina a partir dos s bits menos significativos do endereco, baseado no tamanho da pagina */
    int temp = page_size;
    int s = 0;
    while (temp > 1) {
        temp = temp >> 1;
        s++;
    }
    return addr >> s;
}


Report sub_lru(FILE *file, Cell* list, int debug) {
/* Algoritmo de substituicao Last Recently Used (LRU) */
    Report report = {0, 0};
    Cell *page_search = init_frame();
    unsigned addr;
    char rw;

    while (fscanf(file, "%x %c", &addr, &rw) != EOF) {
        rw = tolower(rw);
        addr = page_addr(addr, list->page_size);

        if (debug) printf("Endereco: %x, modo: %c\n", addr, rw);

        page_search = search_list(addr, list);
        if (page_search != NULL) {
            // Achou a pagina
        }
        else {
            report.page_faults++;

            if (debug) printf("Page fault\n");

            if (!is_full(list)) {
                insert_end_list(addr, list);
            }
            else {
            // Usa o algoritmo
            // O que eh dirty page???                  
            }
        }

    }

    free(page_search);

    return report;
}


Report sub_2a(FILE *file, Cell* list, int debug) {
/* Algoritmo de substituicao Segunda Chance (2a) */
    Report report = {0, 0};
    Cell *page_search = init_frame();
    unsigned addr;
    char rw;

    while (fscanf(file, "%x %c", &addr, &rw) != EOF) {
        rw = tolower(rw);
        addr = page_addr(addr, list->page_size);

        if (debug) printf("Endereco: %x, modo: %c\n", addr, rw);

        page_search = search_list(addr, list);
        if (page_search != NULL) {
            // Achou a pagina
        }
        else {
            report.page_faults++;

            if (debug) printf("Page fault\n");

            if (!is_full(list)) {
                insert_end_list(addr, list);
            }
            else {
            // Usa o algoritmo
            // O que eh dirty page???                  
            }
        }

    }

    free(page_search);

    return report;
}


Report sub_fifo(FILE *file, Cell* list, int debug){
/* Algoritmo de substituicao First In First Out (FIFO) */
    Report report = {0, 0};
    Cell *page_search = init_frame();
    unsigned addr;
    char rw;

    while (fscanf(file, "%x %c", &addr, &rw) != EOF) {
        rw = tolower(rw);
        addr = page_addr(addr, list->page_size);

        if (debug) printf("\nEndereco: %x, modo: %c\n", addr, rw);

        page_search = search_list(addr, list);
        if (page_search != NULL) {
            if (debug) printf("Encontrou a pagina\n");
        }
        else {
            report.page_faults++;

            if (debug) printf("Page fault\n");
            
            if (is_full(list)) {
                remove_top_list(list);
                report.dirty_pages++;

                if (debug) printf("Escrita em disco\n");
            }
            insert_end_list(addr, list);
        }

    }

    return report;
}


Report sub_random(FILE *file, Cell* list, int debug){
/* Algoritmo de substituicao Aleatorio (Random) */
    Report report = {0, 0};
    Cell *page_search = init_frame();
    unsigned addr;
    char rw;

    while (fscanf(file, "%x %c", &addr, &rw) != EOF) {
        rw = tolower(rw);
        addr = page_addr(addr, list->page_size);

        if (debug) printf("Endereco: %x, modo: %c\n", addr, rw);

        page_search = search_list(addr, list);
        if (page_search != NULL) {
            // Achou a pagina
        }
        else {
            report.page_faults++;

            if (debug) printf("Page fault\n");

            if (!is_full(list)) {
                insert_end_list(addr, list);
            }
            else {
            // Usa o algoritmo
            // O que eh dirty page???                  
            }
        }

    }

    free(page_search);

    return report;
}
