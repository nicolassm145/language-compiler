#include <stdio.h>
#include "sintatico.h"

int main(int argc, char *argv[]) {
    const char *arquivo = argv[1];
    AnalisadorSintatico *analisador = criarAnalisador(arquivo);
    if (!analisador) {
        printf("ERRO: Nao foi possivel abrir o arquivo!\n");
        return 1;
    }
    
    printf("Total de tokens: %d\n", analisador->total);
    int resultado = analisar(analisador);
    
    destruirAnalisador(analisador);
    return resultado ? 0 : 1;
}
