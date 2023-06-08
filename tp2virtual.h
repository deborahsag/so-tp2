#include <stdio.h>
#include <ctype.h>

unsigned find_page(unsigned addr, int page_size){
/* Identifica a pagina a partir dos s bits menos significativos do endereco, baseado no tamanho da pagina */

    int temp = page_size;
    int s = 0;
    while (temp > 1) {
        temp = temp >> 1;
        s++;
    }

    return addr >> s;
}

void sub_lru(unsigned addr, char rw){
    return;
}

void sub_2a(unsigned addr, char rw){
    return;
}

void sub_fifo(unsigned addr, char rw){
    return;
}

void sub_random(unsigned addr, char rw){
    return;
}
