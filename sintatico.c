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
    printf("ERRO SINTATICO (linha %d, coluna %d): %s\n", 
           p->atual.linha, p->atual.coluna, mensagem);
    printf("Token encontrado: %s (%s)\n", 
           nomeToken(p->atual.tipo), p->atual.lexema);
    p->erros++;
}

// Verifica se é igual o tipo esperado
int mesmoTipo(AnalisadorSintatico *p, TipoToken tipo) {
    if (p->atual.tipo == tipo) {
        avancaToken(p);
        return 1;  
    }
    return 0;  
}

// Alias para mesmoTipo (compatibilidade)
int esperado(AnalisadorSintatico *p, TipoToken tipo) {
    return mesmoTipo(p, tipo);
}

// Gramatica
// PROGRAMA -> COMANDOS
void programa(AnalisadorSintatico *p) {
    comandos(p);

    if (p->atual.tipo != TOKEN_EOF) 
        erro(p, "Cade o fim");
}

// COMANDOS -> COMANDO RESTO_COMANDOS
// RESTO_COMANDOS -> , COMANDO RESTO_COMANDOS | ε
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

// ATRIBUICAO -> id := EXPR_ARIT
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

// LEITURA -> LEIA id
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

// ESCRITA -> ESCREVA ITEM_ESCRITA
// ITEM_ESCRITA -> id | num | string
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

// SE_ENTAO -> SE EXPR_BOOL ENTAO COMANDOS PARTE_SENAO FIM
// PARTE_SENAO -> SENAO COMANDOS | ε
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

// FACA_ENQUANTO -> FACA COMANDOS ENQUANTO EXPR_BOOL
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

// COMANDO -> ATRIBUICAO | LEITURA | ESCRITA | SE_ENTAO | FACA_ENQUANTO
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

// TERMO -> FATOR RESTO_TERMO
// RESTO_TERMO -> * FATOR RESTO_TERMO | / FATOR RESTO_TERMO | ε

// FATOR -> id | num
void fator(AnalisadorSintatico *p) {
    if (p->atual.tipo == TOKEN_ID || p->atual.tipo == TOKEN_NUM) 
        avancaToken(p);
    else 
        erro(p, "Esperado identificador ou numero");

}

// TERMO -> FATOR RESTO_TERMO
void termo(AnalisadorSintatico *p) {
    fator(p);
    
    // RESTO_TERMO -> * FATOR | / FATOR | ε
    while (p->atual.tipo == TOKEN_MULT || p->atual.tipo == TOKEN_DIV) {
        avancaToken(p);
        fator(p);
    }
}

// EXPR_ARIT -> TERMO RESTO_EXPR
// RESTO_EXPR -> + TERMO RESTO_EXPR | - TERMO RESTO_EXPR | ε
void exprArit(AnalisadorSintatico *p) {
    termo(p);
    
    // RESTO_EXPR -> + TERMO | - TERMO | ε
    while (p->atual.tipo == TOKEN_MAIS || p->atual.tipo == TOKEN_MENOS) {
        avancaToken(p);
        termo(p);
    }
}

// EXPR_BOOL -> EXPR_ARIT OP_REL EXPR_ARIT
// OP_REL -> < | =
void exprBool(AnalisadorSintatico *p) {
    exprArit(p);
    
    // Operador relacional
    if (p->atual.tipo == TOKEN_MENOR || p->atual.tipo == TOKEN_IGUAL) {
        avancaToken(p);
        exprArit(p);
    } else 
        erro(p, "Esperado operador relacional (< ou =)");
    
}

// ===== FUNCOES DE GERENCIAMENTO =====

// Cria o parser lendo tokens do arquivo do analisador lexico
AnalisadorSintatico* criarParser(const char *arquivo_tokens) {
    AnalisadorSintatico *p = (AnalisadorSintatico*)malloc(sizeof(AnalisadorSintatico));
    if (!p) return NULL;
    
    // Por enquanto, vamos pegar tokens diretamente do analisador lexico
    // Em vez de ler do arquivo .Results.txt
    AnalisadorLexico *lex = criaAnalisador(arquivo_tokens);
    if (!lex) {
        free(p);
        return NULL;
    }
    
    // Conta tokens primeiro
    int capacidade = 1000;
    p->tokens = (Token*)malloc(capacidade * sizeof(Token));
    p->total = 0;
    
    Token t;
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

// Destroi o parser
void destruirParser(AnalisadorSintatico *p) {
    if (p) {
        free(p->tokens);
        free(p);
    }
}

// Funcao principal de analise
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
