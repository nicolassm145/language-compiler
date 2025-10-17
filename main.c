#include <stdio.h>
#include <string.h>
#include "lexico.h"
#include "sintatico.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo.x25a>\n", argv[0]);
        printf("\nEste programa executa:\n");
        printf("  1. Analise Lexica (gera tokens)\n");
        printf("  2. Analise Sintatica (valida estrutura)\n");
        return 1;
    }
    
    const char *arquivo_entrada = argv[1];
    char arquivo_saida[256];

    if (argc >= 3) 
        strcpy(arquivo_saida, argv[2]);
    else 
        snprintf(arquivo_saida, sizeof(arquivo_saida), "%s.Results.txt", arquivo_entrada);
    
    printf("Arquivo de entrada: %s\n\n", arquivo_entrada);

    
    printf("Análise Lexica\n");
    salvaTokens(arquivo_entrada, arquivo_saida);
    printf("Tokens salvos em: %s\n\n", arquivo_saida);
      
    AnalisadorSintatico *analisador = criarAnalisador(arquivo_entrada);
    if (!analisador) {
        printf("ERRO: Nao foi possivel criar analisador sintatico!\n");
        return 1;
    }
    
    
    analisar(analisador);
    printf("\n\nTotal de tokens: %d\n\n", analisador->total);
    destruirAnalisador(analisador);
    
   
}
