# Compilador para Linguagem X25a

## 📋 Descrição

Este projeto implementa um **compilador completo** para a linguagem X25a, incluindo:
- **Analisador Léxico (Scanner)** - Parte (a)
- **Gramática LL(1)** - Parte (b)
- **Analisador Sintático (Parser)** - Parte (c)

O analisador léxico utiliza um **Autômato Finito Determinístico (AFD)** implementado manualmente em C, e o analisador sintático utiliza a técnica **descendente recursiva preditiva LL(1)**.

## 🗂️ Estrutura do Projeto

```
AtividadeAvaliativa/
├── === DOCUMENTAÇÃO ===
├── README.md                    # Este arquivo
├── INDICE.md                    # Navegação completa do projeto
├── AFD.md                       # Diagrama do AFD
├── GRAMATICA.md                 # Gramática explicada
├── EXPLICACAO_CODIGO.md         # Como funciona o léxico
├── EXPLICACAO_PARSER.md         # Como funciona o sintático
├── RESPOSTA_PARTE_A.md          # Resposta formal - Parte A
├── RESPOSTA_PARTE_B.md          # Resposta formal - Parte B
├── RESPOSTA_PARTE_C.md          # Resposta formal - Parte C
├── NOTA_VIRGULAS_TRAILING.md    # Explicação sobre vírgulas trailing
├── RESUMO_FINAL.md              # Resumo geral do projeto
│
├── === ANALISADOR LÉXICO ===
├── lexico.h                     # Definições de tipos e funções
├── lexico.c                     # Implementação do AFD
├── main.c                       # Programa de teste do léxico
├── analisador                   # Executável do léxico
│
├── === ANALISADOR SINTÁTICO ===
├── sintatico.h                  # Definições do parser
├── sintatico.c                  # Implementação LL(1)
├── main_parser.c                # Programa de teste do parser
├── sintatico                    # Executável do parser
│
└── === ARQUIVOS DE TESTE ===
    ├── teste1.x25a              # Exemplo 1: Fatorial
    ├── teste2.x25a              # Exemplo 2: Maior de dois números
    ├── teste3.x25a              # Exemplo 3: Menu de banco
    ├── teste4.x25a              # Exemplo 4: Média de números
    └── *.Results.txt            # Resultados da análise léxica
```

## 🔧 Como Compilar

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

## 📊 Formato das Saídas

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
========================================
  ANALISADOR SINTATICO - LINGUAGEM X25a
========================================

Arquivo de entrada: teste1.x25a
Total de tokens: 39
========================================

Iniciando analise sintatica...

========================================
Analise sintatica concluida SEM ERROS!
Programa VALIDO segundo a gramatica X25a.
========================================
```

**Programa com erros:**
```
========================================
  ANALISADOR SINTATICO - LINGUAGEM X25a
========================================

Arquivo de entrada: teste_erro.x25a
Total de tokens: 15
========================================

Iniciando analise sintatica...
ERRO SINTATICO (linha 3, coluna 5): Esperado :=
Token encontrado: NUM (10)

========================================
Analise sintatica concluida COM ERROS!
Total de erros encontrados: 1
========================================
```

## 🎯 Tokens Reconhecidos

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
- `(`, `)` (parênteses)

### Literais:
- Strings: `'texto entre aspas simples'`

### Comentários:
- `[comentário]` - são ignorados

## 🧪 Testando com seu próprio código

1. Crie um arquivo `.x25a` com seu programa:

```bash
nano meu_programa.x25a
```

2. Execute o analisador léxico:

```bash
./analisador meu_programa.x25a
```

3. Execute o analisador sintático:

```bash
./sintatico meu_programa.x25a
```

4. Veja os resultados:

```bash
# Tokens gerados
cat meu_programa.x25a.Results.txt

# Validação sintática (aparece no terminal)
```

## 🐛 Tratamento de Erros

### Erros Léxicos:

O analisador léxico detecta:
1. **Identificador muito longo** (mais de 3 letras consecutivas que não seja palavra-chave)
2. **':' sem '='** (dois pontos sozinho)
3. **String não fechada** (falta de `'` no final)
4. **Caracteres inválidos**

Erros são marcados como `TOKEN_ERRO` no arquivo de saída.

### Erros Sintáticos:

O analisador sintático detecta:
1. **Comando inválido** (token inesperado)
2. **Palavra-chave faltando** (esperado SE, ENTAO, etc.)
3. **Operador faltando** (esperado :=, <, =, etc.)
4. **Estrutura incorreta** (comando incompleto)

Erros são reportados com **linha e coluna** exatas.

## 📖 Documentação

### Para Analisador Léxico:
- **`AFD.md`** - Diagrama completo do Autômato Finito Determinístico
- **`EXPLICACAO_CODIGO.md`** - Explicação detalhada do código
- **`RESPOSTA_PARTE_A.md`** - Resposta formal da Parte A

### Para Gramática:
- **`GRAMATICA.md`** - Gramática explicada didaticamente
- **`RESPOSTA_PARTE_B.md`** - Resposta formal da Parte B

### Para Analisador Sintático:
- **`EXPLICACAO_PARSER.md`** - Explicação passo a passo do parser
- **`RESPOSTA_PARTE_C.md`** - Resposta formal da Parte C
- **`NOTA_VIRGULAS_TRAILING.md`** - Explicação sobre vírgulas trailing

### Navegação:
- **`INDICE.md`** - Índice completo do projeto
- **`RESUMO_FINAL.md`** - Resumo de tudo que foi feito

Para visualizar diagramas Mermaid:
- Abra os arquivos `.md` no VS Code
- Ou visualize no GitHub
- Ou use um visualizador Mermaid online

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

Veja explicação completa em `EXPLICACAO_PARSER.md`!

## ✅ Checklist de Funcionamento

### Analisador Léxico:
- [x] Reconhece palavras-chave
- [x] Reconhece identificadores (1-3 letras)
- [x] Reconhece números inteiros
- [x] Reconhece operador de atribuição `:=`
- [x] Reconhece operadores aritméticos
- [x] Reconhece operadores relacionais
- [x] Reconhece delimitadores
- [x] Reconhece strings entre `'...'`
- [x] Ignora comentários `[...]`
- [x] Ignora espaços em branco
- [x] Detecta erros léxicos
- [x] Gera arquivo de saída formatado
- [x] Conta linha e coluna dos tokens

### Analisador Sintático:
- [x] Valida estrutura de comandos
- [x] Valida atribuições (id := expressão)
- [x] Valida leitura (LEIA id)
- [x] Valida escrita (ESCREVA saída)
- [x] Valida SE-ENTAO-SENAO-FIM
- [x] Valida FACA-ENQUANTO
- [x] Valida expressões aritméticas
- [x] Valida expressões booleanas
- [x] Respeita precedência de operadores
- [x] Aceita vírgulas trailing (opcionais)
- [x] Detecta erros sintáticos
- [x] Reporta linha e coluna dos erros

### Resultados dos Testes:
- [x] teste1.x25a: ✅ 39 tokens, 0 erros
- [x] teste2.x25a: ✅ 30 tokens, 0 erros
- [x] teste3.x25a: ✅ 42 tokens, 0 erros
- [x] teste4.x25a: ✅ 44 tokens, 0 erros

## 📚 Referências

- Especificação da linguagem X25a (documento da atividade)
- Conceitos de compiladores e análise léxica
- Conceitos de análise sintática descendente
- Autômatos Finitos Determinísticos (AFD)
- Gramáticas LL(1) e parsing preditivo

## 🎓 Características Especiais

### Vírgulas Trailing
A linguagem X25a aceita **vírgulas trailing** (opcionais) antes de palavras-chave estruturais como `ENQUANTO`, `SENAO`, e `FIM`. Isso permite flexibilidade no estilo de codificação.

Exemplo - ambos são válidos:
```x25a
FACA
  x := x + 1,      ← com vírgula
ENQUANTO x < 10

FACA
  x := x + 1       ← sem vírgula
ENQUANTO x < 10
```

Veja `NOTA_VIRGULAS_TRAILING.md` para detalhes!

## 👨‍💻 Autor

Desenvolvido como parte da Atividade Avaliativa de Compiladores - UNIFEI

---

**Dúvidas?** 
- Consulte `INDICE.md` para navegação completa
- Leia `RESUMO_FINAL.md` para visão geral do projeto
- Veja os arquivos de documentação específicos de cada parte
