#include "sintatico.h"
#include <stdlib.h>
#include <string.h>

void comandos(AnalisadorSintatico *p);
void comando(AnalisadorSintatico *p);
void exprArit(AnalisadorSintatico *p);
void exprBool(AnalisadorSintatico *p);
void termo(AnalisadorSintatico *p);

// Avanca para o proximo
void avancaToken(AnalisadorSintatico *p) {
    if (p->posicao < p->total - 1) {
        p->posicao++;
        p->atual = p->tokens[p->posicao];
    }
}

// Se tiver erro
void erro(AnalisadorSintatico *p, const char *mensagem) {
    printf("ERRO (linha %d, coluna %d): %s\n", 
           p->atual.linha, p->atual.coluna, mensagem);
    p->erros++;
}

// Verifica se é igual o tipo esperado
int esperado(AnalisadorSintatico *p, TipoToken tipo) {
     if (p->atual.tipo == tipo) {
        avancaToken(p);
        return 1;  
    }
    return 0;  
}

// Gramatica
// Programa -> Comandos
void programa(AnalisadorSintatico *p) {
    comandos(p);
    if (p->atual.tipo != TOKEN_EOF) 
        erro(p, "Cade o fim");
}

// Comandos -> Comando OutrosComandos
// OutrosComandos -> , Comando OutrosComandos | ε
void comandos(AnalisadorSintatico *p) {
    comando(p); 
    // Se tem virgula, continua
    while (p->atual.tipo == TOKEN_VIRGULA) {
        avancaToken(p);
        // Tava dando erro antes 
        if (p->atual.tipo == TOKEN_ENQUANTO || p->atual.tipo == TOKEN_SENAO || p->atual.tipo == TOKEN_FIM) 
            break;
        comando(p);
    }
}

// Atribuicao -> id := ExpressaoAritmetica
void atribuicao(AnalisadorSintatico *p) {
    if (!esperado(p, TOKEN_ID)) {
        erro(p, "Esperado id na atribuicao");
        return;
    }
    if (!esperado(p, TOKEN_ATRIB)) {
        erro(p, "Esperado ':=' na atribuicao");
        return;
    }
    exprArit(p);
}

// Leitura -> LEIA id
void leitura(AnalisadorSintatico *p) {
    if (!esperado(p, TOKEN_LEIA)) {
        erro(p, "Esperado LEIA");
        return;
    }
    
    if (!esperado(p, TOKEN_ID)) {
        erro(p, "Esperado identificador apos LEIA");
        return;
    }
}

// Escrita -> ESCREVA itemEscrita
// itemEscrita -> id | num | string
void escrita(AnalisadorSintatico *p) {
    if (!esperado(p, TOKEN_ESCREVA)) {
        erro(p, "Esperado ESCREVA");
        return;
    }
    // Aceita os 3
    if (p->atual.tipo == TOKEN_ID || p->atual.tipo == TOKEN_NUM || p->atual.tipo == TOKEN_STRING) 
        avancaToken(p);
    else
        erro(p, "Esperado id, num ou string apos ESCREVA");
}

// SeEntao -> SE ExpressãoBool ENTAO Comandos ParteSenão FIM
// ParteSenão -> SENAO Comandos | ε
void seEntao(AnalisadorSintatico *p) {
    if (!esperado(p, TOKEN_SE)) {
        erro(p, "Esperado SE");
        return;
    }
    exprBool(p);
    
    if (!esperado(p, TOKEN_ENTAO)) {
        erro(p, "Esperado ENTAO");
        return;
    }
    
    comandos(p);
    // SENAO e opcional
    if (p->atual.tipo == TOKEN_SENAO) {
        avancaToken(p);
        comandos(p);
    }

    if (!esperado(p, TOKEN_FIM)) {
        erro(p, "Esperado FIM");
        return;
    }
}

// façaEnquanto -> Faça Comandos Enquanto ExpressãoBool
void facaEnquanto(AnalisadorSintatico *p) {
    if (!esperado(p, TOKEN_FACA)) {
        erro(p, "Esperado FACA");
        return;
    }
    
    comandos(p);
    
    if (!esperado(p, TOKEN_ENQUANTO)) {
        erro(p, "Esperado ENQUANTO");
        return;
    }
    
    exprBool(p);
}

// Comando -> Atribuicao | Leitura | Escrita | SeEntao | FaçaEnquanto
void comando(AnalisadorSintatico *p) {
    switch (p->atual.tipo) {
        case TOKEN_ID:
            atribuicao(p);
            break;
        case TOKEN_LEIA:
            leitura(p);
            break; 
        case TOKEN_ESCREVA:
            escrita(p);
            break;   
        case TOKEN_SE:
            seEntao(p);
            break;
        case TOKEN_FACA:
            facaEnquanto(p);
            break;
        default:
            erro(p, "Nao tem");
            avancaToken(p);
            break;
    }
}


// Precisa para funfar
// Termo -> Fator restoTermo
// restoTermo -> * Fator restoTermo | / Fator restoTermo | ε

// Fator -> id | num
void fator(AnalisadorSintatico *p) {
    if (p->atual.tipo == TOKEN_ID || p->atual.tipo == TOKEN_NUM) 
        avancaToken(p);
    else 
        erro(p, "Esperado identificador ou numero");
}

// Termo -> Fator restoTermo
void termo(AnalisadorSintatico *p) {
    fator(p);
    // restoTermo -> * Fator restoTermo | / Fator restoTermo | ε
    while (p->atual.tipo == TOKEN_MULT || p->atual.tipo == TOKEN_DIV) {
        avancaToken(p);
        fator(p);
    }
}

// ExpressãoArit -> Termo restoExpr
// restoExpr -> + Termo restoExpr | - Termo restoExpr | ε
void exprArit(AnalisadorSintatico *p) {
    termo(p);
    // restoExpr -> + Termo | - Termo | ε
    while (p->atual.tipo == TOKEN_MAIS || p->atual.tipo == TOKEN_MENOS) {
        avancaToken(p);
        termo(p);
    }
}

// ExpressãoBool -> ExpressãoArit Operador ExpressãoArit
// Operador -> < | =
void exprBool(AnalisadorSintatico *p) {
    exprArit(p);
    // Operador relacional
    if (p->atual.tipo == TOKEN_MENOR || p->atual.tipo == TOKEN_IGUAL) {
        avancaToken(p);
        exprArit(p);
    } else 
        erro(p, "Esperado operador relacional (< ou =)");
    
}


// Cria o analisador sintatico 
AnalisadorSintatico* criarAnalisador(const char *arquivo_tokens) {
    Token t;

    AnalisadorSintatico *p = (AnalisadorSintatico*)malloc(sizeof(AnalisadorSintatico));
    if (!p) return NULL;
    // Pega tokens do analisador lexico
    AnalisadorLexico *lex = criaAnalisador(arquivo_tokens);
    if (!lex) {
        free(p);
        return NULL;
    }
    // Conta qnts tokes tem
    int capacidade = 1000;
    p->tokens = (Token*)malloc(capacidade * sizeof(Token));
    p->total = 0;
    
    do {
        t = proximoToken(lex);
        if (p->total >= capacidade) {
            capacidade *= 2;
            p->tokens = (Token*)realloc(p->tokens, capacidade * sizeof(Token));
        }

        p->tokens[p->total++] = t;
    
    } while (t.tipo != TOKEN_EOF);
    
    liberaMemoria(lex);
    p->posicao = 0;
    p->atual = p->tokens[0];
    p->erros = 0;
    
    return p;
}

// Destroi o analisador sintatico
void destruirAnalisador(AnalisadorSintatico *p) {
    if (p) {
        free(p->tokens);
        free(p);
    }
}

// Começa a analisew
int analisar(AnalisadorSintatico *p) {
    programa(p);
    
    if (p->erros == 0) {
        printf("Analise sintatica concluida SEM ERROS!\n");
        printf("Programa VALIDO segundo a gramatica X25a.\n");
        return 1;
    } else {
        printf("Analise sintatica concluida COM ERROS!\n");
        printf("Total de erros encontrados: %d\n", p->erros);
        return 0;
    }
}
