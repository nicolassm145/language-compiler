#include <stdio.h>
#include "sintatico.h"

int main(int argc, char *argv[]) {
    const char *arquivo = argv[1];
    AnalisadorSintatico *parser = criarParser(arquivo);
    if (!parser) {
        printf("ERRO: Nao foi possivel abrir o arquivo!\n");
        return 1;
    }
    
    printf("Total de tokens: %d\n", parser->total);
    int resultado = analisar(parser);
    
    destruirParser(parser);
    return resultado ? 0 : 1;
}
