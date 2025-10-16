#ifndef SINTATICO_H
#define SINTATICO_H
#include "lexico.h"

// Estrutura do analisador sintatico
typedef struct {
    Token *tokens;       // Array de tokens
    int posicao;         // Posicao atual no array
    int total;           // Total de tokens
    Token atual;         // Token atual sendo analisado
    int erros;           // Contador de erros
} AnalisadorSintatico;

// Funcoes principais
AnalisadorSintatico* criarParser(const char *arquivo_tokens);
void destruirParser(AnalisadorSintatico *parser);
int analisar(AnalisadorSintatico *parser);

// Funcoes auxiliares
void avancaToken(AnalisadorSintatico *parser);
void erro(AnalisadorSintatico *parser, const char *mensagem);
int mesmoTipo(AnalisadorSintatico *parser, TipoToken tipo);

#endif
