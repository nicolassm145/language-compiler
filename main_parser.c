#include <stdio.h>
#include "sintatico.h"

int main(int argc, char *argv[]) {
    printf("========================================\n");
    printf("  ANALISADOR SINTATICO - LINGUAGEM X25a\n");
    printf("========================================\n\n");
    
    if (argc < 2) {
        printf("Uso: %s <arquivo.x25a>\n", argv[0]);
        printf("\nExemplos:\n");
        printf("  %s teste1.x25a\n", argv[0]);
        printf("  %s teste2.x25a\n", argv[0]);
        return 1;
    }
    
    const char *arquivo = argv[1];
    printf("Arquivo de entrada: %s\n\n", arquivo);
    
    // Cria o parser
    AnalisadorSintatico *parser = criarParser(arquivo);
    if (!parser) {
        printf("ERRO: Nao foi possivel abrir o arquivo!\n");
        return 1;
    }
    
    printf("Total de tokens: %d\n", parser->total);
    printf("========================================\n\n");
    
    // Executa a analise sintatica
    int resultado = analisar(parser);
    
    printf("========================================\n");
    
    // Destroi o parser
    destruirParser(parser);
    
    return resultado ? 0 : 1;
}
