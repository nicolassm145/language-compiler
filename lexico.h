#ifndef LEXICO_H
#define LEXICO_H

#include <stdio.h>

/* ========================================
   TIPOS DE TOKENS
   ======================================== */
typedef enum {
    /* Palavras-chave */
    TOKEN_LEIA,
    TOKEN_ESCREVA,
    TOKEN_SE,
    TOKEN_ENTAO,
    TOKEN_SENAO,
    TOKEN_FIM,
    TOKEN_FACA,
    TOKEN_ENQUANTO,
    
    /* Identificadores e literais */
    TOKEN_ID,           /* Variável (1-3 letras minúsculas) */
    TOKEN_NUM,          /* Número */
    TOKEN_STRING,       /* 'texto' */
    
    /* Operadores */
    TOKEN_ATRIB,        /* := */
    TOKEN_MAIS,         /* + */
    TOKEN_MENOS,        /* - */
    TOKEN_MULT,         /* * */
    TOKEN_DIV,          /* / */
    TOKEN_MENOR,        /* < */
    TOKEN_IGUAL,        /* = */
    
    /* Delimitadores */
    TOKEN_VIRGULA,      /* , */
    TOKEN_ABRE_PAR,     /* ( */
    TOKEN_FECHA_PAR,    /* ) */
    
    /* Especiais */
    TOKEN_EOF,          /* Fim de arquivo */
    TOKEN_ERRO          /* Erro */
} TipoToken;

/* ========================================
   ESTRUTURA DO TOKEN
   ======================================== */
typedef struct {
    TipoToken tipo;     /* Tipo do token */
    char lexema[100];   /* Texto do token */
    int linha;          /* Linha no arquivo */
    int coluna;         /* Coluna no arquivo */
} Token;

/* ========================================
   ESTRUTURA DO ANALISADOR
   ======================================== */
typedef struct {
    FILE *arquivo;
    char ch;            /* Caractere atual */
    int linha;
    int coluna;
    int fim;            /* 1 se chegou no fim */
} AnalisadorLexico;

/* ========================================
   FUNÇÕES PRINCIPAIS
   ======================================== */

/* Cria o analisador abrindo o arquivo */
AnalisadorLexico* criar_analisador(const char *nome_arquivo);

/* Fecha o arquivo e libera memória */
void destruir_analisador(AnalisadorLexico *analisador);

/* Retorna o próximo token do arquivo */
Token proximo_token(AnalisadorLexico *analisador);

/* Retorna o nome do tipo do token (para debug) */
const char* nome_token(TipoToken tipo);

/* Processa arquivo completo e salva tokens */
void salvar_tokens(const char *arquivo_entrada, const char *arquivo_saida);

#endif
