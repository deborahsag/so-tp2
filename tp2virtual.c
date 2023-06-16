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

    if (debug) printf("Iniciando tabela de paginas\n");
    Page *page_table = init_page();

    FILE *file = fopen(argv[2], "r");
    unsigned addr;
    char rw;

    while (fscanf(file, "%x %c", &addr, &rw) != EOF) {
        rw = tolower(rw);
        
        if (debug) printf("\nEndereco: %x, leitura/escrita: %c\n", addr, rw);
        
        if (strcmp(argv[1], "lru") == 0) {
            sub_lru(addr, rw, page_table);
        }
        else if (strcmp(argv[1], "2a") == 0) {
            sub_2a(addr, rw, page_table);
        }
        else if (strcmp(argv[1], "fifo") == 0) {
            sub_fifo(addr, rw, page_table);
        }
        else if (strcmp(argv[1], "random") == 0) {
            sub_random(addr, rw, page_table);
        }
    }
    
    fclose(file);

    if (debug){
        bool hit = search_table(addr, page_table);
        printf("\nProcura ultimo referenciado: %x, Hit: %d\n\n", addr, hit);
    }
    
    printf("Paginas lidas: \n");
    printf("Paginas escritas: \n");

    return 0;
}
