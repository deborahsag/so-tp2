#include "tp2virtual.h"

int main(int argc, char *argv[])  {
    
    /* Ativa modo de depuracao */
    int debug = 0;
    // if (strcmp(argv[5], "debug") == 0) debug = 1; else debug = 0;
    if (debug) printf("Modo de depuracao\n\n");
    
    printf("Executando o simulador...\n");
    printf("Arquivo de entrada: %s\n", argv[2]);
    printf("Tamanho da memoria: %s KB\n", argv[4]);
    printf("Tamanho das páginas: %s KB\n", argv[3]);
    printf("Técnica de reposição: %s\n", argv[1]);
    
    int mem_size = atoi(argv[4]);
    int page_size = atoi(argv[3]);
    int max_list_size = mem_size / page_size;

    Cell *list = init_frame();
    list->page_size = page_size;
    list->max_list_size = max_list_size;

    if (debug) {
        printf("\nIniciando tabela de paginas\n");
        printf("Tamanho da pagina: %d KB\n", list->page_size);
        printf("Tamanho da tabela: %d paginas\n\n", list->max_list_size);
    }
    
    Report report;

    FILE *file = fopen(argv[2], "r");
    
    if (strcmp(argv[1], "lru") == 0) {
        report = sub_lru(file, list, debug);
    }
    else if (strcmp(argv[1], "2a") == 0) {
        report = sub_2a(file, list, debug);
    }
    else if (strcmp(argv[1], "fifo") == 0) {
        report = sub_fifo(file, list, debug);
    }
    else if (strcmp(argv[1], "random") == 0) {
        report = sub_random(file, list, debug);
    }
    
    fclose(file);

    free_back(list);
    
    if (debug) printf("\nFim da depuracao\n\n");

    printf("Paginas lidas: %d\n", report.page_faults);
    printf("Paginas escritas: %d\n", report.dirty_pages);

    return 0;
}
