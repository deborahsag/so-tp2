#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


typedef struct Page Page;
struct Page {
    unsigned addr;
    int last_acc;
    int altered;
    int table_size;
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


bool is_full(int size, Page* page_table) {
/* Retorna se a tabela de paginas esta cheia */
    if (page_table->table_size >= size) return true;
    else return false;
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


unsigned page_addr(unsigned addr, int page_size){
/* Identifica a pagina a partir dos s bits menos significativos do endereco, baseado no tamanho da pagina */
    int temp = page_size;
    int s = 0;
    while (temp > 1) {
        temp = temp >> 1;
        s++;
    }
    return addr >> s;
}


void sub_lru(unsigned addr, char rw, Page* page_table){
    return;
}


void sub_2a(unsigned addr, char rw, Page* page_table){
    return;
}


void sub_fifo(unsigned addr, char rw, Page* page_table){
    return;
}


void sub_random(unsigned addr, char rw, Page* page_table){
    return;
}
