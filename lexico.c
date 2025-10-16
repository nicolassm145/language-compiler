#include "lexico.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *palavras_chave[] = {
    "LEIA", "ESCREVA", "SE", "ENTAO", "ENTÃO",
    "SENAO", "SENÃO", "FIM", "FACA", "FAÇA", "ENQUANTO"
};

TipoToken tipos_palavras_chave[] = {
    TOKEN_LEIA, TOKEN_ESCREVA, TOKEN_SE, TOKEN_ENTAO, TOKEN_ENTAO,
    TOKEN_SENAO, TOKEN_SENAO, TOKEN_FIM, TOKEN_FACA, TOKEN_FACA, TOKEN_ENQUANTO
};

// Le o proximo caracter
void leChar(AnalisadorLexico *a) {
    if (a->fim) 
        return;

    a->atual = fgetc(a->arquivo);

    if (a->atual == EOF) {
        a->fim = 1;
    } else if (a->atual == '\n') {
        a->linha++;
        a->coluna = 0;
    } else {
        a->coluna++;
    }
}

// Ignora todos os tipos de espaços
void ignoraEspacos(AnalisadorLexico *a) {
    while (!a->fim && (a->atual == ' ' || a->atual == '\t' || a->atual == '\n' || a->atual == '\r')) 
        leChar(a);
}

// Ignora comentários 
void ignoraComentario(AnalisadorLexico *a) {
    leChar(a);  

    while (!a->fim && a->atual != ']') 
        leChar(a);

    if (a->atual == ']') 
        leChar(a);  
}

// Compara os lexemas com as Palavras chave
TipoToken comparaID(const char *lexema) {
    int i;
    int total = 11;  
    
    for (i = 0; i < total; i++) 
        if (strcmp(lexema, palavras_chave[i]) == 0) 
            return tipos_palavras_chave[i];
        
    return TOKEN_ID;  // Retorna q é ID
}

// Le ID
Token leIdentificador(AnalisadorLexico *a) {
    Token t;
    int i = 0;
    
    t.linha = a->linha;
    t.coluna = a->coluna;
    
    // Le o todo o "ID",
    while (!a->fim && isalpha(a->atual) && i < 99) {
        t.lexema[i++] = a->atual;
        leChar(a);
    }
    t.lexema[i] = '\0';
    
    // Apos isso verifica se ele pode ser uma palavra chave
    t.tipo = comparaID(t.lexema);

    // Se passar, ele agr verifica se é valido
    // "apresentam-se como sequencias de uma a três letras minúsculas"
    if (t.tipo == TOKEN_ID) {
        int j;
        int maiucula = 0;
        
        for (j = 0; t.lexema[j] != '\0'; j++) {
            if (isupper(t.lexema[j])) {
                maiucula = 1;
                break;
            }
        }

        // Verifica se tem o tamanho correto
        if (maiucula || i > 3) 
            t.tipo = TOKEN_ERRO;
    }
    return t;
}

// Le numeros
Token leNumero(AnalisadorLexico *a) {
    Token t;
    int i = 0;
    
    t.linha = a->linha;
    t.coluna = a->coluna;
    t.tipo = TOKEN_NUM;
    
    while (!a->fim && isdigit(a->atual) && i < 99) {
        t.lexema[i++] = a->atual;
        leChar(a);
    }
    t.lexema[i] = '\0';
    
    return t;
}

// Le as strings
Token leString(AnalisadorLexico *a) {
    Token t;
    int i = 0;
    
    t.linha = a->linha;
    t.coluna = a->coluna;
    t.tipo = TOKEN_STRING;
    
    leChar(a);  // Pula o ' inicial
    
    // Le ate encontrar outro 
    while (!a->fim && a->atual != '\'' && i < 99) {
        t.lexema[i++] = a->atual;
        leChar(a);
    }
    
    if (a->atual == '\'') 
        leChar(a);  // Pula o ' final
    else 
        t.tipo = TOKEN_ERRO;  // Não fecharam a string
    
    t.lexema[i] = '\0';
    return t;
}

// Cria o analisador lexico
AnalisadorLexico* criaAnalisador(const char *nome_arquivo) {
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
    leChar(a);  

    return a;
}

// Libera memória
void liberaMemoria(AnalisadorLexico *a) {
    if (a) {
        if (a->arquivo) fclose(a->arquivo);
        free(a);
    }
}

// Pega o prox token
Token proximoToken(AnalisadorLexico *a) {
    Token t;
    
    // Pula todos os espaços e comentarios
    while (!a->fim) {
        ignoraEspacos(a);
        if (a->atual == '[') 
            ignoraComentario(a);
        else 
            break;
    }
    
    // Verifica se é o final
    if (a->fim) {
        t.tipo = TOKEN_EOF;
        t.lexema[0] = '\0';
        t.linha = a->linha;
        t.coluna = a->coluna;
        return t;
    }
    
    t.linha = a->linha;
    t.coluna = a->coluna;
    
    // Verifica se é ID ou palavra chave
    if (isalpha(a->atual)) 
        return leIdentificador(a);
    
    // Verifica se é numero
    if (isdigit(a->atual)) 
        return leNumero(a);
    
    // Ignora se for string
    if (a->atual == '\'') 
        return leString(a);
    
    // Verifica se é atribuicao :=
    if (a->atual == ':') {
        t.lexema[0] = ':';
        leChar(a);
        
        if (a->atual == '=') {
            t.lexema[1] = '=';
            t.lexema[2] = '\0';
            t.tipo = TOKEN_ATRIB;
            leChar(a);
        } else {
            t.lexema[1] = '\0';
            t.tipo = TOKEN_ERRO;  // ':' sozinho e erro
        }
        return t;
    }
    
    // Operadores 
    t.lexema[0] = a->atual;
    t.lexema[1] = '\0';
    
    switch (a->atual) {
        case '+': 
            t.tipo = TOKEN_MAIS;      
        break;
        case '-': 
            t.tipo = TOKEN_MENOS;     
            break;
        case '*': 
            t.tipo = TOKEN_MULT;      
            break;
        case '/': 
            t.tipo = TOKEN_DIV;       
            break;
        case '<': 
            t.tipo = TOKEN_MENOR;     
            break;
        case '=': 
            t.tipo = TOKEN_IGUAL;     
            break;
        case ',': 
            t.tipo = TOKEN_VIRGULA;   
            break;
        default:  
            t.tipo = TOKEN_ERRO;     
            break;
    }
    leChar(a);
    return t;
}

// Retorna o nome do token
const char* nomeToken(TipoToken tipo) {
    switch (tipo) {
        case TOKEN_LEIA:      
            return "LEIA";
        case TOKEN_ESCREVA:   
            return "ESCREVA";
        case TOKEN_SE:        
            return "SE";
        case TOKEN_ENTAO:     
            return "ENTAO";
        case TOKEN_SENAO:     
            return "SENAO";
        case TOKEN_FIM:       
            return "FIM";
        case TOKEN_FACA:      
            return "FACA";
        case TOKEN_ENQUANTO:  
            return "ENQUANTO";
        case TOKEN_ID:        
            return "ID";
        case TOKEN_NUM:      
            return "NUM";
        case TOKEN_STRING:    
            return "STRING";
        case TOKEN_ATRIB:     
            return "ATRIB";
        case TOKEN_MAIS:      
            return "MAIS";
        case TOKEN_MENOS:     
            return "MENOS";
        case TOKEN_MULT:      
            return "MULT";
        case TOKEN_DIV:       
            return "DIV";
        case TOKEN_MENOR:     
            return "MENOR";
        case TOKEN_IGUAL:     
            return "IGUAL";
        case TOKEN_VIRGULA:   
            return "VIRGULA";
        case TOKEN_EOF:       
            return "EOF";
        case TOKEN_ERRO:      
            return "ERRO";
        default:              
            return "DESCONHECIDO";
    }
}

// Impreme os tokens em um arquivo
void salvaTokens(const char *arq_entrada, const char *arq_saida) {
    AnalisadorLexico *a;
    FILE *saida;
    Token t;
    int total = 0;
    int erros = 0;
    
    a = criaAnalisador(arq_entrada);

    saida = fopen(arq_saida, "w");
   
    // Cabecalho
    fprintf(saida, "=== ANALISE LEXICA - LINGUAGEM X25a ===\n");
    fprintf(saida, "Arquivo: %s\n", arq_entrada);
    fprintf(saida, "========================================\n\n");
    fprintf(saida, "%-6s %-8s %-15s %s\n", "Linha", "Coluna", "Token", "Lexema");
    fprintf(saida, "------------------------------------------------\n");
 
    // Imprime todos os tokens
    do {
        t = proximoToken(a);
        
        if (t.tipo != TOKEN_EOF) {
            fprintf(saida, "%-6d %-8d %-15s %s\n", 
                    t.linha, t.coluna, nomeToken(t.tipo), t.lexema);
            total++;
            if (t.tipo == TOKEN_ERRO) erros++;
        }
        
    } while (t.tipo != TOKEN_EOF);
    
    if (erros == 0)
        fprintf(saida, "Deu certo\n");
    else
        fprintf(saida, "Deu errado\n");
    
    fclose(saida);
    liberaMemoria(a);

}
