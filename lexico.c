#include "lexico.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ========================================
   LISTA DE PALAVRAS-CHAVE
   ======================================== */
const char *palavras_chave[] = {
    "LEIA", "ESCREVA", "SE", "ENTAO", "ENTÃO",
    "SENAO", "SENÃO", "FIM", "FACA", "FAÇA", "ENQUANTO"
};

const TipoToken tipos_palavras_chave[] = {
    TOKEN_LEIA, TOKEN_ESCREVA, TOKEN_SE, TOKEN_ENTAO, TOKEN_ENTAO,
    TOKEN_SENAO, TOKEN_SENAO, TOKEN_FIM, TOKEN_FACA, TOKEN_FACA, TOKEN_ENQUANTO
};

/* ========================================
   FUNÇÃO: Lê próximo caractere do arquivo
   ======================================== */
void ler_char(AnalisadorLexico *a) {
    if (a->fim) return;
    
    a->ch = fgetc(a->arquivo);
    
    if (a->ch == EOF) {
        a->fim = 1;
    } else if (a->ch == '\n') {
        a->linha++;
        a->coluna = 0;
    } else {
        a->coluna++;
    }
}

/* ========================================
   FUNÇÃO: Pula espaços, tabs e quebras de linha
   ======================================== */
void pular_espacos(AnalisadorLexico *a) {
    while (!a->fim && (a->ch == ' ' || a->ch == '\t' || 
                       a->ch == '\n' || a->ch == '\r')) {
        ler_char(a);
    }
}

/* ========================================
   FUNÇÃO: Pula comentários [...]
   ======================================== */
void pular_comentario(AnalisadorLexico *a) {
    ler_char(a);  /* Pula o '[' */
    
    while (!a->fim && a->ch != ']') {
        ler_char(a);
    }
    
    if (a->ch == ']') {
        ler_char(a);  /* Pula o ']' */
    }
}

/* ========================================
   FUNÇÃO: Verifica se lexema é palavra-chave
   Retorna: tipo do token (palavra-chave ou ID)
   ======================================== */
TipoToken eh_palavra_chave(const char *lexema) {
    int i;
    int total = 11;  /* Total de palavras-chave */
    
    for (i = 0; i < total; i++) {
        if (strcmp(lexema, palavras_chave[i]) == 0) {
            return tipos_palavras_chave[i];
        }
    }
    
    return TOKEN_ID;  /* Não é palavra-chave, é identificador */
}

/* ========================================
   FUNÇÃO: Lê um identificador ou palavra-chave
   Estados: INICIAL → ID1 → ID2 → ID3 → FIM
   ======================================== */
Token ler_identificador(AnalisadorLexico *a) {
    Token t;
    int i = 0;
    
    t.linha = a->linha;
    t.coluna = a->coluna;
    
    /* Lê letras (maiúsculas ou minúsculas) */
    while (!a->fim && isalpha(a->ch) && i < 99) {
        t.lexema[i++] = a->ch;
        ler_char(a);
    }
    t.lexema[i] = '\0';
    
    /* Verifica se é palavra-chave */
    t.tipo = eh_palavra_chave(t.lexema);
    
    /* Se for ID, verifica se tem só minúsculas e máx 3 letras */
    if (t.tipo == TOKEN_ID) {
        int j;
        int tem_maiuscula = 0;
        
        for (j = 0; t.lexema[j] != '\0'; j++) {
            if (isupper(t.lexema[j])) {
                tem_maiuscula = 1;
                break;
            }
        }
        
        /* Identificador deve ter: só minúsculas e máx 3 letras */
        if (tem_maiuscula || i > 3) {
            t.tipo = TOKEN_ERRO;
        }
    }
    
    return t;
}

/* ========================================
   FUNÇÃO: Lê um número
   Estados: INICIAL → NUM → NUM → ... → FIM
   ======================================== */
Token ler_numero(AnalisadorLexico *a) {
    Token t;
    int i = 0;
    
    t.linha = a->linha;
    t.coluna = a->coluna;
    t.tipo = TOKEN_NUM;
    
    /* Lê todos os dígitos */
    while (!a->fim && isdigit(a->ch) && i < 99) {
        t.lexema[i++] = a->ch;
        ler_char(a);
    }
    t.lexema[i] = '\0';
    
    return t;
}

/* ========================================
   FUNÇÃO: Lê uma string '...'
   Estados: INICIAL → STRING → ... → FIM
   ======================================== */
Token ler_string(AnalisadorLexico *a) {
    Token t;
    int i = 0;
    
    t.linha = a->linha;
    t.coluna = a->coluna;
    t.tipo = TOKEN_STRING;
    
    ler_char(a);  /* Pula o ' inicial */
    
    /* Lê até encontrar outro ' */
    while (!a->fim && a->ch != '\'' && i < 99) {
        t.lexema[i++] = a->ch;
        ler_char(a);
    }
    
    if (a->ch == '\'') {
        ler_char(a);  /* Pula o ' final */
    } else {
        t.tipo = TOKEN_ERRO;  /* String não fechada */
    }
    
    t.lexema[i] = '\0';
    return t;
}

/* ========================================
   FUNÇÃO: Cria o analisador léxico
   ======================================== */
AnalisadorLexico* criar_analisador(const char *nome_arquivo) {
    AnalisadorLexico *a;
    
    a = (AnalisadorLexico*)malloc(sizeof(AnalisadorLexico));
    if (!a) return NULL;
    
    a->arquivo = fopen(nome_arquivo, "r");
    if (!a->arquivo) {
        free(a);
        return NULL;
    }
    
    a->linha = 1;
    a->coluna = 0;
    a->fim = 0;
    
    ler_char(a);  /* Lê primeiro caractere */
    
    return a;
}

/* ========================================
   FUNÇÃO: Destrói o analisador
   ======================================== */
void destruir_analisador(AnalisadorLexico *a) {
    if (a) {
        if (a->arquivo) fclose(a->arquivo);
        free(a);
    }
}

/* ========================================
   FUNÇÃO PRINCIPAL: Retorna próximo token
   Implementa o AFD completo
   ======================================== */
Token proximo_token(AnalisadorLexico *a) {
    Token t;
    
    /* Pula espaços e comentários */
    while (!a->fim) {
        pular_espacos(a);
        if (a->ch == '[') {
            pular_comentario(a);
        } else {
            break;
        }
    }
    
    /* Fim de arquivo? */
    if (a->fim) {
        t.tipo = TOKEN_EOF;
        t.lexema[0] = '\0';
        t.linha = a->linha;
        t.coluna = a->coluna;
        return t;
    }
    
    t.linha = a->linha;
    t.coluna = a->coluna;
    
    /* É letra? → Identificador ou palavra-chave */
    if (isalpha(a->ch)) {
        return ler_identificador(a);
    }
    
    /* É dígito? → Número */
    if (isdigit(a->ch)) {
        return ler_numero(a);
    }
    
    /* É string '...'? */
    if (a->ch == '\'') {
        return ler_string(a);
    }
    
    /* É atribuição :=? */
    if (a->ch == ':') {
        t.lexema[0] = ':';
        ler_char(a);
        
        if (a->ch == '=') {
            t.lexema[1] = '=';
            t.lexema[2] = '\0';
            t.tipo = TOKEN_ATRIB;
            ler_char(a);
        } else {
            t.lexema[1] = '\0';
            t.tipo = TOKEN_ERRO;  /* ':' sozinho é erro */
        }
        return t;
    }
    
    /* Operadores e delimitadores simples */
    t.lexema[0] = a->ch;
    t.lexema[1] = '\0';
    
    switch (a->ch) {
        case '+': t.tipo = TOKEN_MAIS;      break;
        case '-': t.tipo = TOKEN_MENOS;     break;
        case '*': t.tipo = TOKEN_MULT;      break;
        case '/': t.tipo = TOKEN_DIV;       break;
        case '<': t.tipo = TOKEN_MENOR;     break;
        case '=': t.tipo = TOKEN_IGUAL;     break;
        case ',': t.tipo = TOKEN_VIRGULA;   break;
        case '(': t.tipo = TOKEN_ABRE_PAR;  break;
        case ')': t.tipo = TOKEN_FECHA_PAR; break;
        default:  t.tipo = TOKEN_ERRO;      break;
    }
    
    ler_char(a);
    return t;
}

/* ========================================
   FUNÇÃO: Retorna nome do token (para debug)
   ======================================== */
const char* nome_token(TipoToken tipo) {
    switch (tipo) {
        case TOKEN_LEIA:      return "LEIA";
        case TOKEN_ESCREVA:   return "ESCREVA";
        case TOKEN_SE:        return "SE";
        case TOKEN_ENTAO:     return "ENTAO";
        case TOKEN_SENAO:     return "SENAO";
        case TOKEN_FIM:       return "FIM";
        case TOKEN_FACA:      return "FACA";
        case TOKEN_ENQUANTO:  return "ENQUANTO";
        case TOKEN_ID:        return "ID";
        case TOKEN_NUM:       return "NUM";
        case TOKEN_STRING:    return "STRING";
        case TOKEN_ATRIB:     return "ATRIB";
        case TOKEN_MAIS:      return "MAIS";
        case TOKEN_MENOS:     return "MENOS";
        case TOKEN_MULT:      return "MULT";
        case TOKEN_DIV:       return "DIV";
        case TOKEN_MENOR:     return "MENOR";
        case TOKEN_IGUAL:     return "IGUAL";
        case TOKEN_VIRGULA:   return "VIRGULA";
        case TOKEN_ABRE_PAR:  return "ABRE_PAR";
        case TOKEN_FECHA_PAR: return "FECHA_PAR";
        case TOKEN_EOF:       return "EOF";
        case TOKEN_ERRO:      return "ERRO";
        default:              return "DESCONHECIDO";
    }
}

/* ========================================
   FUNÇÃO: Processa arquivo e salva tokens
   ======================================== */
void salvar_tokens(const char *arq_entrada, const char *arq_saida) {
    AnalisadorLexico *a;
    FILE *saida;
    Token t;
    int total = 0;
    int erros = 0;
    
    a = criar_analisador(arq_entrada);
    if (!a) {
        printf("ERRO: não consegui abrir '%s'\n", arq_entrada);
        return;
    }
    
    saida = fopen(arq_saida, "w");
    if (!saida) {
        printf("ERRO: não consegui criar '%s'\n", arq_saida);
        destruir_analisador(a);
        return;
    }
    
    /* Cabeçalho */
    fprintf(saida, "=== ANÁLISE LÉXICA - LINGUAGEM X25a ===\n");
    fprintf(saida, "Arquivo: %s\n", arq_entrada);
    fprintf(saida, "========================================\n\n");
    fprintf(saida, "%-6s %-8s %-15s %s\n", "Linha", "Coluna", "Token", "Lexema");
    fprintf(saida, "------------------------------------------------\n");
    
    /* Processa todos os tokens */
    do {
        t = proximo_token(a);
        
        if (t.tipo != TOKEN_EOF) {
            fprintf(saida, "%-6d %-8d %-15s %s\n", 
                    t.linha, t.coluna, nome_token(t.tipo), t.lexema);
            total++;
            if (t.tipo == TOKEN_ERRO) erros++;
        }
        
    } while (t.tipo != TOKEN_EOF);
    
    /* Rodapé */
    fprintf(saida, "\n========================================\n");
    fprintf(saida, "Total de tokens: %d\n", total);
    fprintf(saida, "Erros: %d\n", erros);
    
    if (erros == 0)
        fprintf(saida, "✓ Análise OK!\n");
    else
        fprintf(saida, "✗ Análise com erros!\n");
    
    fclose(saida);
    destruir_analisador(a);
    
    printf("Tokens encontrados: %d\n", total);
    printf("Erros: %d\n", erros);
    printf("Resultado: %s\n", arq_saida);
}
