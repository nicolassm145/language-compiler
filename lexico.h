#ifndef LEXICO_H
#define LEXICO_H
#include <stdio.h>

// Tokens
typedef enum {
    // Palavras reservadas
    TOKEN_LEIA,
    TOKEN_ESCREVA,
    TOKEN_SE,
    TOKEN_ENTAO,
    TOKEN_SENAO,
    TOKEN_FIM,
    TOKEN_FACA,
    TOKEN_ENQUANTO,
    // Ids
    TOKEN_ID,           // Variavel (1-3 letras minúsculas)
    TOKEN_NUM,          // Numero
    TOKEN_STRING,       // Tudo entre '...'
    // Operadores
    TOKEN_ATRIB,        // :=
    TOKEN_MAIS,         // +
    TOKEN_MENOS,        // -
    TOKEN_MULT,         // *
    TOKEN_DIV,          // /
    TOKEN_MENOR,        // <
    TOKEN_IGUAL,        // =
    // Delimitador
    TOKEN_VIRGULA,      // ,
    //TOKEN_ABRE_PAR,     // (
    //TOKEN_FECHA_PAR,    // )
    TOKEN_EOF,          // Fim de arquivo
    TOKEN_ERRO          // Erro
} TipoToken;

typedef struct {
    TipoToken tipo;     // Tipo do token
    char lexema[100];   // Texto do token       
    int linha;
    int coluna;
} Token;

// Analisador léxico
typedef struct {
    FILE *arquivo;
    char atual;            
    int linha;
    int coluna;
    int fim;            
} AnalisadorLexico;


/* Cria o analisador abrindo o arquivo */
AnalisadorLexico* criaAnalisador(const char *nome_arquivo);

/* Libera memória */
void liberaMemoria(AnalisadorLexico *analisador);

/* Retorna o próximo token do arquivo */
Token proximoToken(AnalisadorLexico *analisador);

/* Retorna o nome do tipo do token  */
const char* nomeToken(TipoToken tipo);

/* Processa arquivo completo e salva tokens */
void salvaTokens(const char *arquivo_entrada, const char *arquivo_saida);

#endif
