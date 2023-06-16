#include "tp2virtual.h"

int main(int argc, char *argv[])  {
    
    // Ativa modo de depuracao
    bool debug;
    if (strcmp(argv[5], "debug") == 0) debug = true; else debug = false;
    
    if (debug) printf("Modo de depuracao\n\n");
    printf("Executando o simulador...\n");
    printf("Arquivo de entrada: %s\n", argv[2]);
    printf("Tamanho da memoria: %s KB\n", argv[4]);
    printf("Tamanho das páginas: %s KB\n", argv[3]);
    printf("Técnica de reposição: %s\n\n", argv[1]);
    
    if (debug) printf("Iniciando tabela de paginas \n\n");
    Page *page_table = init_page(debug);

    FILE *file = fopen(argv[2], "r");
    unsigned addr;
    char rw;

    while (fscanf(file, "%x %c", &addr, &rw) != EOF) {
        rw = tolower(rw);
        
        if (debug) printf("\nEndereco: %x, leitura/escrita: %c\n", addr, rw);
        
        if (debug) {
            if (rw =='w') {
                printf("Inserindo na lista...\n");
                insert_in_table(addr, page_table);
                print_last_page_addr(page_table);
            }
        }        
    }
    
    fclose(file);

    if (debug){
        bool hit = search_table(addr, page_table);
        printf("\nProcura ultimo referenciado: %x, Hit: %d\n", addr, hit);
    }
    

    printf("\n\nPaginas lidas: \n");
    printf("Paginas escritas: \n");

}
