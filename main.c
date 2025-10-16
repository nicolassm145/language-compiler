#include <stdio.h>
#include <string.h>
#include "lexico.h"

int main(int argc, char *argv[]) {
    printf("========================================\n");
    printf("  ANALISADOR LÉXICO - LINGUAGEM X25a\n");
    printf("========================================\n\n");
    
    /* Verifica argumentos */
    if (argc < 2) {
        printf("Uso: %s <arquivo.x25a> [saida.txt]\n", argv[0]);
        printf("\nExemplos:\n");
        printf("  %s teste1.x25a\n", argv[0]);
        printf("  %s teste1.x25a tokens.txt\n", argv[0]);
        return 1;
    }
    
    /* Define nomes dos arquivos */
    const char *arquivo_entrada = argv[1];
    char arquivo_saida[256];
    
    if (argc >= 3) {
        strcpy(arquivo_saida, argv[2]);
    } else {
        /* Nome padrão: adiciona .tokens.txt */
        snprintf(arquivo_saida, sizeof(arquivo_saida), "%s.tokens.txt", arquivo_entrada);
    }
    
    printf("Arquivo de entrada: %s\n", arquivo_entrada);
    printf("Arquivo de saída: %s\n\n", arquivo_saida);
    
    /* Executa a análise léxica */
    salvar_tokens(arquivo_entrada, arquivo_saida);
    
    printf("\n========================================\n");
    
    return 0;
}
