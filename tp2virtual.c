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
    printf("Técnica de reposição: %s\n", argv[1]);
    
    FILE *file = fopen(argv[2], "r");
    unsigned addr;
    char rw;

    while (fscanf(file, "%x %c", &addr, &rw) != EOF) {
        rw = tolower(rw);
        if (debug) printf("Addr: %x, mode: %c\n", addr, rw);
    }
    
    fclose(file);

    printf("Paginad lidas: \n");
    printf("Paginas escritas: \n");
    
}
