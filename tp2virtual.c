#include "tp2virtual.h"

int main(int argc, char *argv[])  {
    
    /* Ativa modo de depuracao */
    bool debug;
    if (strcmp(argv[5], "debug") == 0) debug = true; else debug = false;
    if (debug) printf("Modo de depuracao\n\n");
    
    printf("Executando o simulador...\n");
    printf("Arquivo de entrada: %s\n", argv[2]);
    printf("Tamanho da memoria: %s KB\n", argv[4]);
    printf("Tamanho das páginas: %s KB\n", argv[3]);
    printf("Técnica de reposição: %s\n", argv[1]);
    
    int mem_size = atoi(argv[4]);
    int page_size = atoi(argv[3]);
    int max_table_size = mem_size / page_size;
    if (debug) printf("\nTamanho da tabela: %d\n\n", max_table_size);

    Page *page_table = init_page();
    page_table->max_size = max_table_size;
    if (debug) printf("Iniciando tabela de paginas\n\n");

    Report report;

    FILE *file = fopen(argv[2], "r");
    
    if (strcmp(argv[1], "lru") == 0) {
        report = sub_lru(file, page_table, debug);
    }
    else if (strcmp(argv[1], "2a") == 0) {
        report = sub_2a(file, page_table, debug);
    }
    else if (strcmp(argv[1], "fifo") == 0) {
        report = sub_fifo(file, page_table, debug);
    }
    else if (strcmp(argv[1], "random") == 0) {
        report = sub_random(file, page_table, debug);
    }
    
    fclose(file);
    free_table(page_table);
    
    printf("Paginas lidas: %d\n", report.page_faults);
    printf("Paginas escritas: %d\n", report.dirty_pages);

    return 0;
}
