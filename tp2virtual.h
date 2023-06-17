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
};


Page* init_page() {
/* Inicia uma celula nova da tabela de paginas */
    Page *page = malloc (sizeof(Page));
    page->last_acc = 0;
    page->altered = 0;
    page->prev = NULL;
    page->next = NULL;
    page->table_size = 0;
    return page;
}


void insert_table_end(unsigned addr, Page* page_table) {
/* Insere uma pagina na tabela de paginas baseado em seu endereco */
    Page *page = init_page();
    page->addr = addr;

    Page *last = page_table;
    while (last->next != NULL) {
        last = last->next;
    }

    last->next = page;
    page->prev = last;
    page_table->table_size += 1;
}


void swap_page(Page* old, Page* new, Page* page_table) {
/* Substitui uma pagina na tabela por outra dada a pagina anterior da que esta sendo substituida, a nova pagina e a tabela de pagina */
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
/* Retorna a pagina anterior  */    
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
    while(page->next != NULL){
        page = page->next;
        printf("%x\n", page->addr);
    }
    printf("\n");
}


void print_last_page_addr(Page *page_table) {
/* Imprime o endereco da ultima pagina da tabela */
    Page *last = page_table;
    while (last->next != NULL) {
        last = last->next;
    }
    unsigned data = last->addr;
    printf("Ultimo inserido: %x\n", data);
}


unsigned page_number(unsigned addr, int page_size){
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
