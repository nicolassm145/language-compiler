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
    
    // Define nome do arquivo de saida dos tokens
    if (argc >= 3) 
        strcpy(arquivo_saida, argv[2]);
    else 
        snprintf(arquivo_saida, sizeof(arquivo_saida), "%s.Results.txt", arquivo_entrada);
    
    printf("========================================\n");
    printf("   COMPILADOR - LINGUAGEM X25a\n");
    printf("========================================\n\n");
    printf("Arquivo de entrada: %s\n\n", arquivo_entrada);
    
    // ==== ETAPA 1: ANALISE LEXICA ====
    printf("========================================\n");
    printf("ETAPA 1: ANALISE LEXICA\n");
    printf("========================================\n");
    
    salvaTokens(arquivo_entrada, arquivo_saida);
    printf("Tokens salvos em: %s\n\n", arquivo_saida);
    
    // ==== ETAPA 2: ANALISE SINTATICA ====
    printf("========================================\n");
    printf("ETAPA 2: ANALISE SINTATICA\n");
    printf("========================================\n");
    
    AnalisadorSintatico *analisador = criarAnalisador(arquivo_entrada);
    if (!analisador) {
        printf("ERRO: Nao foi possivel criar analisador sintatico!\n");
        return 1;
    }
    
    printf("Total de tokens: %d\n\n", analisador->total);
    int resultado = analisar(analisador);
    
    destruirAnalisador(analisador);
    
    printf("\n========================================\n");
    if (resultado) {
        printf("COMPILACAO CONCLUIDA COM SUCESSO!\n");
        printf("========================================\n");
        return 0;
    } else {
        printf("COMPILACAO FALHOU - VERIFIQUE OS ERROS\n");
        printf("========================================\n");
        return 1;
    }
}
