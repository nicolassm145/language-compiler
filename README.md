# Compilador para Linguagem X25a

## Descrição

Este projeto implementa um **compilador completo** para a linguagem X25a, incluindo:
- **Analisador Léxico (Scanner)** - Parte (a)
- **Gramática LL(1)** - Parte (b)
- **Analisador Sintático (Parser)** - Parte (c)

O analisador léxico utiliza um **Autômato Finito Determinístico (AFD)** implementado manualmente em C, e o analisador sintático utiliza a técnica **descendente recursiva preditiva LL(1)**.

## Como Compilar

### Parte (a) - Analisador Léxico:

```bash
gcc -o analisador main.c lexico.c -Wall
```

### Parte (c) - Analisador Sintático:

```bash
gcc -o sintatico main_parser.c sintatico.c lexico.c -Wall
```

**Explicação:**
- `gcc`: compilador C
- `-o analisador` / `-o sintatico`: nome do executável gerado
- `main.c lexico.c` / `main_parser.c sintatico.c lexico.c`: arquivos a compilar
- `-Wall`: mostra todos os warnings (avisos)

## ▶️ Como Executar

### Analisador Léxico (Parte a):

```bash
./analisador teste1.x25a
```

Isso vai:
1. Ler o arquivo `teste1.x25a`
2. Fazer a análise léxica
3. Gerar o arquivo `teste1.x25a.Results.txt` com os tokens encontrados

### Analisador Sintático (Parte c):

```bash
./sintatico teste1.x25a
```

Isso vai:
1. Ler o arquivo `teste1.x25a`
2. Fazer análise léxica (gerar tokens)
3. Fazer análise sintática (validar estrutura)
4. Reportar se o programa é válido ou se há erros sintáticos

### Executar todos os testes:

```bash
# Análise léxica
./analisador teste1.x25a
./analisador teste2.x25a
./analisador teste3.x25a
./analisador teste4.x25a

# Análise sintática
./sintatico teste1.x25a
./sintatico teste2.x25a
./sintatico teste3.x25a
./sintatico teste4.x25a
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


## 🔍 Estrutura do Código

### Analisador Léxico

#### `lexico.h`
Define:
- Enumeração `TipoToken` com todos os tipos de tokens
- Estrutura `Token` (tipo, lexema, linha, coluna)
- Estrutura `AnalisadorLexico` (estado do analisador)
- Protótipos das funções

#### `lexico.c`
Implementa:
- `criaAnalisador()`: inicializa o analisador
- `proximoToken()`: retorna o próximo token (implementação do AFD)
- `leIdentificador()`: reconhece identificadores/palavras-chave
- `leNumero()`: reconhece números
- `leString()`: reconhece strings
- `ignoraComentario()`: ignora comentários
- `salvaTokens()`: processa arquivo e salva tokens

#### `main.c`
- Processa argumentos da linha de comando
- Chama o analisador léxico
- Exibe mensagens ao usuário

### Analisador Sintático

#### `sintatico.h`
Define:
- Estrutura `AnalisadorSintatico` (tokens, posição, erros)
- Protótipos das funções do parser

#### `sintatico.c`
Implementa (uma função por regra da gramática):
- `programa()`: regra inicial
- `comandos()`: sequência de comandos
- `comando()`: escolhe tipo de comando
- `atribuicao()`: id := expressão
- `leitura()`: LEIA id
- `escrita()`: ESCREVA saída
- `seEntao()`: SE-ENTAO-SENAO-FIM
- `facaEnquanto()`: FACA-ENQUANTO
- `exprArit()`: expressões aritméticas
- `termo()`: termos com * e /
- `fator()`: fatores (id, num, parênteses)
- `exprBool()`: expressões booleanas

#### `main_parser.c`
- Processa argumentos da linha de comando
- Chama o analisador léxico para gerar tokens
- Chama o analisador sintático para validar
- Exibe resultado da análise

## 💡 Como Funcionam os Analisadores

### Analisador Léxico (AFD)

O AFD (Autômato Finito Determinístico) funciona como uma máquina de estados:

1. **Estado INICIAL**: aguarda caractere
2. Lê um caractere e **transita** para novo estado
3. Continua lendo até reconhecer um **token completo**
4. Retorna ao estado inicial e repete

Exemplo:
```
"fat" → INICIAL --[f]--> ID1 --[a]--> ID2 --[t]--> ID3 --[espaço]--> TOKEN_ID
```

Veja o diagrama completo em `AFD.md`!

### Analisador Sintático (Parser LL(1))

O parser descendente recursivo funciona com:

1. **Uma função para cada regra da gramática**
2. **Olha apenas 1 token à frente** para decidir (LL(1))
3. **Chamadas recursivas** seguem a estrutura da gramática
4. **Valida** se o programa segue as regras sintáticas

Exemplo de análise de `LEIA x`:
```
programa()
  └─> comandos()
        └─> comando()
              └─> leitura()
                    ├─> consome LEIA ✅
                    └─> consome x (ID) ✅
```


