#ifndef SINTATICO_H
#define SINTATICO_H
#include "lexico.h"

typedef struct {
    Token *tokens;       
    int posicao;         
    int total;           
    Token atual;         
    int erros;          
} AnalisadorSintatico;


AnalisadorSintatico* criarAnalisador(const char *arquivo_tokens);
void destruirAnalisador(AnalisadorSintatico *analisador);
int analisar(AnalisadorSintatico *analisador);
void avancaToken(AnalisadorSintatico *analisador);
void erro(AnalisadorSintatico *analisador, const char *mensagem);
int esperado(AnalisadorSintatico *analisador, TipoToken tipo);

#endif
