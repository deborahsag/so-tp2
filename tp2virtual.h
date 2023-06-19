#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct Report Report;
struct Report{
    int page_faults;
    int dirty_pages;
};

typedef struct Page Page;
struct Page {
    unsigned addr;
    int valid;
    int dirty;
    int ref;
};

typedef struct Cell Cell;
struct Cell {
    Page page;
    int page_size;
    int list_size;
    int max_list_size;
    Cell *prev;
    Cell *next;
    Cell *last;
};


Cell* init_frame() {
/* Inicia uma celula da lista duplamente encadeada */
    Cell *frame = malloc (sizeof(Cell));
    frame->prev = NULL;
    frame->next = NULL;
    frame->last = NULL;
    frame->list_size = 0;
    return frame;
}


void insert_end_list(Page page, Cell *list) {
/* Insere uma nova pagina no fim de uma lista */
    Cell *frame = init_frame();
    frame->page = page;

    if (list->next == NULL) {
        list->next = frame;
        frame->prev = list;
    } else {
        list->last->next = frame;
        frame->prev = list->last;
    }

    list->last = frame;
    list->list_size += 1;
}


unsigned remove_top_list(Cell *list) {
/* Elimina o quadro do topo da lista */
    Cell* first = list->next;
    unsigned rm_addr = first->page.addr;
    list->next = list->next->next;
    list->next->prev = list;
    free(first);
    list->list_size--;
    return rm_addr;
}


unsigned remove_random_list(Cell *list) {
/* Elimina um quadro aleatorio da lista */
    int idx = random() % list->list_size;

    Cell* rm_cell = list->next;
    
    int count = 0;
    while (rm_cell != NULL) {
        if (count == idx) break;
        rm_cell = rm_cell->next;
        count++;
    }

    unsigned rm_addr = rm_cell->page.addr;

    if (rm_cell->prev == list) {
        list->next = rm_cell->next;
        list->next->prev = list;
    }
    else if (rm_cell->next == NULL) {
        list->last = rm_cell->prev;
        list->last->next = NULL;
    }
    else {
        rm_cell->prev->next = rm_cell->next;
        rm_cell->next->prev = rm_cell->prev;
    }

    free(rm_cell);

    list->list_size--;
    return rm_addr;
}


void move_to_bottom(Cell *frame, Cell *list) {
/* Move um quadro para o fim da lista */
    if (frame->next != NULL) {
        frame->prev->next = frame->next;
        frame->next->prev = frame->prev;
        frame->prev = list->last;
        frame->next = NULL;
        list->last->next = frame;
        list->last = frame;
    }
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
    if (list->list_size >= list->max_list_size) return 1;
    else return 0;
}


Cell* search_list(unsigned addr, Cell* list) {
/* Retorna o quadro dado seu endereco */    
    Cell *frame = list;
    while(frame->next != NULL){
        frame = frame->next;
        if (frame->page.addr == addr){
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
        printf("%x\n", frame->page.addr);
    }
    printf("\n");
}


void print_backwards(Cell* list) {
/* Imprime os enderecos de tras para frente para testar a lista duplamente encadeada */
    printf("\nTabela de tras para frente:\n");
    Cell *frame = list->last;
    while(frame->prev != NULL) {
        printf("%x\n", frame->page.addr);
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
    int temp = page_size * 1024;
    int s = 0;
    while (temp > 1) {
        temp = temp >> 1;
        s++;
    }
    return addr >> s;
}


Report sub_fifo(FILE *file, Cell* list, int debug){
/* Algoritmo de substituicao First In First Out (FIFO) */
    Page *table = malloc(2097152 * sizeof(Page));
    Report report = {0, 0};

    unsigned mem_addr;
    unsigned addr;
    char rw;

    while (fscanf(file, "%x %c", &mem_addr, &rw) != EOF) {
        
        addr = page_addr(mem_addr, list->page_size);

        if (debug) {
            printf("\nEndereco: %x, modo: %c\n", addr, rw);
            printf("Valido: %d\n", table[addr].valid);
            printf("Sujo: %d\n", table[addr].dirty);
        }

        if (table[addr].valid) {
            if (rw == 'W') {
                table[addr].dirty = 1;
            }
        }
        else { 
            report.page_faults++;
            if (is_full(list)) {
                unsigned rm_addr = remove_top_list(list);
                table[rm_addr].valid = 0;
                if (table[rm_addr].dirty) {
                    report.dirty_pages++; 
                }
            }
            table[addr].addr = addr;
            table[addr].valid = 1;
            insert_end_list(table[addr], list);             
        }
    }

    return report;
}

Report sub_lru(FILE *file, Cell* list, int debug) {
/* Algoritmo de substituicao Least Recently Used (LRU) */
    Page *table = malloc(2097152 * sizeof(Page));
    Report report = {0, 0};

    Cell *page_search;

    unsigned addr;
    char rw;

    while (fscanf(file, "%x %c", &addr, &rw) != EOF) {

        addr = page_addr(addr, list->page_size);

        if (debug) {
            printf("\nEndereco: %x, modo: %c\n", addr, rw);
            printf("Valido: %d\n", table[addr].valid);
            printf("Sujo: %d\n", table[addr].dirty);
        }

        if (table[addr].valid) {
            if (rw == 'W') {
                table[addr].dirty = 1;
            }
            page_search = search_list(addr, list);
            move_to_bottom(page_search, list);
        }
        else { 
            report.page_faults++;
            if (is_full(list)) {
                unsigned rm_addr = remove_top_list(list);
                table[rm_addr].valid = 0;
                if (table[rm_addr].dirty) {
                    report.dirty_pages++; 
                }
            }
            table[addr].addr = addr;
            table[addr].valid = 1;
            insert_end_list(table[addr], list);             
        }
    }

    return report;
}


Report sub_random(FILE *file, Cell* list, int debug){
/* Algoritmo de substituicao Aleatorio (Random) */
    Page *table = malloc(2097152 * sizeof(Page));
    Report report = {0, 0};

    unsigned addr;
    char rw;

    srandom(42);

    while (fscanf(file, "%x %c", &addr, &rw) != EOF) {

        addr = page_addr(addr, list->page_size);

        if (debug) {
            printf("\nEndereco: %x, modo: %c\n", addr, rw);
            printf("Valido: %d\n", table[addr].valid);
            printf("Sujo: %d\n", table[addr].dirty);
        }

        if (table[addr].valid) {
            if (rw == 'W') {
                table[addr].dirty = 1;
            }
        }
        else { 
            report.page_faults++;
            if (is_full(list)) {
                unsigned rm_addr = remove_random_list(list);
                table[rm_addr].valid = 0;
                if (table[rm_addr].dirty) {
                    report.dirty_pages++; 
                }
            }
            table[addr].addr = addr;
            table[addr].valid = 1;
            insert_end_list(table[addr], list);             
        }

    }

    return report;
}


Report sub_2a(FILE *file, Cell* list, int debug) {
/* Algoritmo de substituicao Segunda Chance (2a) */
    Page *table = malloc(2097152 * sizeof(Page));
    Report report = {0, 0};
    
    Cell *pointer = list;

    unsigned addr;
    char rw;

    while (fscanf(file, "%x %c", &addr, &rw) != EOF) {
        
        addr = page_addr(addr, list->page_size);

        if (debug) {
            printf("\nEndereco: %x, modo: %c\n", addr, rw);
            printf("Valido: %d\n", table[addr].valid);
            printf("Sujo: %d\n", table[addr].dirty);
        }

        if (table[addr].valid) {
            if (rw == 'W') {
                table[addr].dirty = 1;
            }
        }
        else { 
            report.page_faults++;
            table[addr].addr = addr;
            table[addr].valid = 1;
            table[addr].ref = 1;
            
            Cell *new = init_frame();
            new->page = table[addr];

            if (!is_full(list)) {
                insert_end_list(table[addr], list);   
                list->last->next = list->next;        
            } else {
                int ref = 1;
                while (ref == 1) {
                    while (pointer->prev == NULL) pointer = pointer->next;
                    if (pointer->page.ref == 1) {
                        pointer->page.ref = 0;
                        pointer = pointer->next;
                    } else {
                        ref = 0;
                    }
                }
                swap_for_new(pointer, new, list);
            }
        }
    }

    return report;
}

