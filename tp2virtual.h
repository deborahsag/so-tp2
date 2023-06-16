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
    Page *next;
};


Page* init_page() {
/* Inicia uma celula nova da tabela de paginas */
    Page *page;
    page = malloc (sizeof(Page));
    page->last_acc = 0;
    page->altered = 0;
    page->next = NULL;
    page->table_size = 0;
    return page;
}


void insert_in_table(unsigned addr, Page* page_table) {
/* Insere uma pagina na tabela de paginas baseado em seu endereco */
    Page *page = init_page();
    page->addr = addr;

    Page *last = page_table;
    while (last->next != NULL) {
        last = last->next;
    }

    last->next = page;
    page_table->table_size += 1;
}


void remove_from_table(Page* page_table, Page* prev, Page* page) {
/* Remove uma celula da lista dada a cabeca da lista, a celula anterior e a celula a ser removida */    
    prev->next = page->next;
    page_table->table_size -= 1;
}


bool search_table(unsigned addr, Page* page_table) {
/* Retorna true para um hit, retorna false para um miss */    
    Page *page = page_table;
    while(page->next != NULL){
        page = page->next;
        if (page->addr == addr){
            return true;
        }
    }
    return false;
}


bool is_full(int size, Page* page_table) {
/* Retorna se a tabela de paginas esta cheia */
    if (page_table->table_size >= size) return true;
    else return false;
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
