# Compilador para Linguagem X25a

## Descrição

Este projeto implementa um **compilador completo** para a linguagem X25a, incluindo:
- **Analisador Léxico (Scanner)** - Parte (a)
- **Gramática LL(1)** - Parte (b)
- **Analisador Sintático (Parser)** - Parte (c)

O analisador léxico utiliza um **Autômato Finito Determinístico (AFD)** implementado manualmente em C, e o analisador sintático utiliza a técnica **descendente recursiva preditiva LL(1)**.

## 🔧 Como Compilar

### Compilador Completo (Recomendado):

```bash
gcc -o compilador main.c lexico.c sintatico.c -Wall
```


### Analisadores Individuais :

**Apenas Léxico:**
```bash
gcc -o analisador mainLexico.c lexico.c -Wall
```

**Apenas Sintático:**
```bash
gcc -o sintatico mainSintatico.c sintatico.c lexico.c -Wall
```

**Explicação:**
- `gcc`: compilador C
- `-o compilador`: nome do executável gerado
- `main.c lexico.c sintatico.c`: arquivos a compilar
- `-Wall`: mostra todos os warnings (avisos)

## Como Executar

```bash
./compilador teste1.x25a
```

Isso vai:
1. Fazer **análise léxica**
2. Salvar tokens em `teste1.x25a.Results.txt`
3. Fazer **análise sintática** 
4. Reportar se o programa é válido ou tem erros


**Apenas Léxico:**
```bash
./analisador teste1.x25a
```

**Apenas Sintático:**
```bash
./sintatico teste1.x25a
```

## Formato das Saídas

### Saída do Analisador Léxico:

```
=== ANALISE LEXICA - LINGUAGEM X25a ===
Arquivo: teste1.x25a
========================================

Linha  Coluna   Token           Lexema
------------------------------------------------
2      1        LEIA            LEIA
2      6        ID              num
2      9        VIRGULA         ,
3      1        ID              fat
3      5        ATRIB           :=
...
========================================
Total de tokens: 39
Erros léxicos: 0
Deu certo
```

### Saída do Analisador Sintático:

**Programa válido:**
```

Analise sintatica concluida SEM ERROS!
Programa VALIDO segundo a gramatica X25a.

```


## Tokens Reconhecidos

### Palavras-chave:
- `LEIA`, `ESCREVA`
- `SE`, `ENTAO`, `SENAO`, `FIM`
- `FACA`, `ENQUANTO`

### Identificadores:
- Variáveis: 1 a 3 letras minúsculas (`x`, `num`, `fat`)

### Números:
- Inteiros: `0`, `1`, `123`, etc.

### Operadores:
- Atribuição: `:=`
- Aritméticos: `+`, `-`, `*`, `/`
- Relacionais: `<`, `=`

### Delimitadores:
- `,` (vírgula)

### Literais:
- Strings: `'texto entre aspas simples'`

### Comentários:
- `[comentário]` - são ignorados


##  Tratamento de Erros

### Erros Léxicos:

O analisador léxico detecta:
1. **Identificador muito longo** (mais de 3 letras consecutivas que não seja palavra-chave)
2. **':' sem '='** (dois pontos sozinho)
3. **String não fechada** (falta de `'` no final)
4. **Caracteres inválidos**


### Erros Sintáticos:

O analisador sintático detecta:
1. **Comando inválido** (token inesperado)
2. **Palavra-chave faltando** (esperado SE, ENTAO, etc.)
3. **Operador faltando** (esperado :=, <, =, etc.)
4. **Estrutura incorreta** (comando incompleto)

