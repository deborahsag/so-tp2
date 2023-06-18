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
    int last_acc;
    int altered;
    int page_size;
    int table_size;
    int max_table_size;
    Page *prev;
    Page *next;
    Page *last;
};


Page* init_page() {
/* Inicia uma celula nova da lista */
    Page *page = malloc (sizeof(Page));
    page->last_acc = 0;
    page->altered = 0;
    page->prev = NULL;
    page->next = NULL;
    page->last = NULL;
    page->table_size = 0;
    return page;
}


void insert_table_end(unsigned addr, Page* page_table) {
/* Insere uma pagina dado seu endereco no fim da lista */
    Page *page = init_page();
    page->addr = addr;

    if (page_table->next == NULL) {
        page_table->next = page;
        page->prev = page_table;
    } else {
        page_table->last->next = page;
        page->prev = page_table->last;
    }

    page_table->last = page;
    page_table->table_size += 1;
}


void swap_for_new(Page* old, Page* new, Page* page_table) {
/* Substitui uma pagina na tabela por outra, dada a pagina a ser substituida, a nova pagina e a tabela de pagina */
    new->next = old->next;
    old->next = new;
    new->prev = old;

    if (new->next != NULL) {
        new->next->prev = new;
    }

    if (old->prev == NULL) {
        page_table = new;
    } else {
        old->prev->next = new;
    }

    if (old->next != NULL) {
        old->next->prev = new;
    }
    free(old);
}


int is_full(Page* page_table) {
/* Retorna se a tabela de paginas esta cheia */
    if (page_table->table_size >= page_table->max_table_size) return 1;
    else return 0;
}


Page* search_table(unsigned addr, Page* page_table) {
/* Retorna a pagina dado seu endereco */    
    Page *page = page_table;
    while(page->next != NULL){
        page = page->next;
        if (page->addr == addr){
            return page;
        }
    }
    return NULL;
}


void print_table(Page* page_table) {
/* Imprime os enderecos da tabela de paginas */
    printf("Tabela:\n");
    Page *page = page_table;
    while(page->next != NULL) {
        page = page->next;
        printf("%x\n", page->addr);
    }
    printf("\n");
}


void print_backwards(Page* page_table) {
/* Imprime os enderecos de tras para frente para testar a lista duplamente encadeada */
    printf("Tabela de tras para frente:\n");
    Page *page = page_table->last;
    while(page->prev != NULL) {
        printf("%x\n", page->addr);
        page = page->prev;
    }
    printf("\n");
}


void free_table(Page* page_table) {
    Page *aux = page_table;
    while(page_table != NULL) {
        aux = page_table;
        page_table = page_table->next;
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


Report sub_lru(FILE *file, Page* page_table, int debug) {
/* Algoritmo de substituicao Last Recently Used (LRU) */
    Report report = {0, 0};
    Page *page_search = init_page();
    unsigned addr;
    char rw;

    while (fscanf(file, "%x %c", &addr, &rw) != EOF) {
        rw = tolower(rw);
        addr = page_addr(addr, page_table->page_size);

        if (debug) printf("Endereco: %x, modo: %c\n", addr, rw);

        page_search = search_table(addr, page_table);
        if (page_search != NULL) {
            // Achou a pagina
        }
        else {
            report.page_faults++;

            if (debug) printf("Page fault\n");

            if (!is_full(page_table)) {
                insert_table_end(addr, page_table);
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


Report sub_2a(FILE *file, Page* page_table, int debug) {
/* Algoritmo de substituicao Segunda Chance (2a) */
    Report report = {0, 0};
    Page *page_search = init_page();
    unsigned addr;
    char rw;

    while (fscanf(file, "%x %c", &addr, &rw) != EOF) {
        rw = tolower(rw);
        addr = page_addr(addr, page_table->page_size);

        if (debug) printf("Endereco: %x, modo: %c\n", addr, rw);

        page_search = search_table(addr, page_table);
        if (page_search != NULL) {
            // Achou a pagina
        }
        else {
            report.page_faults++;

            if (debug) printf("Page fault\n");
            
            if (!is_full(page_table)) {
                insert_table_end(addr, page_table);
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


Report sub_fifo(FILE *file, Page* page_table, int debug){
/* Algoritmo de substituicao First In First Out (FIFO) */
    Report report = {0, 0};
    Page *page_search = init_page();
    unsigned addr;
    char rw;

    while (fscanf(file, "%x %c", &addr, &rw) != EOF) {
        rw = tolower(rw);
        addr = page_addr(addr, page_table->page_size);

        if (debug) printf("Endereco: %x, modo: %c\n", addr, rw);

        page_search = search_table(addr, page_table);
        if (page_search != NULL) {
            // Achou a pagina
        }
        else {
            report.page_faults++;

            if (debug) printf("Page fault\n");
            
            if (!is_full(page_table)) {
                insert_table_end(addr, page_table);
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


Report sub_random(FILE *file, Page* page_table, int debug){
/* Algoritmo de substituicao Aleatorio (Random) */
    Report report = {0, 0};
    Page *page_search = init_page();
    unsigned addr;
    char rw;

    while (fscanf(file, "%x %c", &addr, &rw) != EOF) {
        rw = tolower(rw);
        addr = page_addr(addr, page_table->page_size);

        if (debug) printf("Endereco: %x, modo: %c\n", addr, rw);

        page_search = search_table(addr, page_table);
        if (page_search != NULL) {
            // Achou a pagina
        }
        else {
            report.page_faults++;

            if (debug) printf("Page fault\n");
            
            if (!is_full(page_table)) {
                insert_table_end(addr, page_table);
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
