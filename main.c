#include <stdio.h>
#include <string.h>
#include "lexico.h"

int main(int argc, char *argv[]) {
    const char *arquivo_entrada = argv[1];
    char arquivo_saida[256];
    
    if (argc >= 3) 
        strcpy(arquivo_saida, argv[2]);
    else 
        snprintf(arquivo_saida, sizeof(arquivo_saida), "%s.Results.txt", arquivo_entrada);
    
    
    salvaTokens(arquivo_entrada, arquivo_saida);
    return 0;
}
