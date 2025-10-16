# Analisador Léxico para Linguagem X25a

## 📋 Descrição

Este projeto implementa um **analisador léxico (scanner)** para a linguagem X25a, utilizando um **Autômato Finito Determinístico (AFD)** implementado manualmente em C.

## 🗂️ Estrutura do Projeto

```
AtividadeAvaliativa/
├── AFD.md              # Documentação completa do AFD (com diagrama Mermaid)
├── lexico.h            # Definições de tipos e funções
├── lexico.c            # Implementação do analisador léxico
├── main.c              # Programa principal
├── teste1.x25a         # Exemplo 1: Fatorial
├── teste2.x25a         # Exemplo 2: Maior de dois números
├── teste3.x25a         # Exemplo 3: Menu de banco
├── teste4.x25a         # Exemplo 4: Média de números
└── README.md           # Este arquivo
```

## 🔧 Como Compilar

### Compilação simples (sem Makefile):

```bash
gcc -o analisador main.c lexico.c -Wall
```

**Explicação:**
- `gcc`: compilador C
- `-o analisador`: nome do executável gerado
- `main.c lexico.c`: arquivos a compilar
- `-Wall`: mostra todos os warnings (avisos)

## ▶️ Como Executar

### Executar com arquivo de entrada:

```bash
./analisador teste1.x25a
```

Isso vai:
1. Ler o arquivo `teste1.x25a`
2. Fazer a análise léxica
3. Gerar o arquivo `teste1.x25a.tokens.txt` com os tokens encontrados

### Executar especificando arquivo de saída:

```bash
./analisador teste1.x25a resultado.txt
```

Isso vai salvar os tokens em `resultado.txt` ao invés do nome padrão.

## 📝 Exemplos de Uso

```bash
# Testar todos os exemplos
./analisador teste1.x25a
./analisador teste2.x25a
./analisador teste3.x25a
./analisador teste4.x25a
```

## 📊 Formato do Arquivo de Saída

O arquivo de saída contém:

```
=== ANÁLISE LÉXICA - LINGUAGEM X25a ===
Arquivo de entrada: teste1.x25a
========================================

Linha  Coluna       Token                Lexema
------------------------------------------------------
2      1            LEIA                 LEIA
2      6            ID                   num
2      9            VIRGULA              ,
3      1            ID                   fat
3      5            ATRIB                :=
...
========================================
Total de tokens: 35
Erros léxicos: 0
✓ Análise léxica concluída sem erros!
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

2. Execute o analisador:

```bash
./analisador meu_programa.x25a
```

3. Veja o resultado:

```bash
cat meu_programa.x25a.tokens.txt
```

## 🐛 Tratamento de Erros

O analisador detecta:

1. **Identificador muito longo** (mais de 3 letras consecutivas que não seja palavra-chave)
2. **':' sem '='** (dois pontos sozinho)
3. **String não fechada** (falta de `'` no final)
4. **Caracteres inválidos**

Erros são marcados como `TOKEN_ERRO` no arquivo de saída.

## 📖 Visualizar o AFD

O diagrama completo do Autômato Finito Determinístico está no arquivo `AFD.md`.

Para visualizar o diagrama Mermaid:
- Abra `AFD.md` no VS Code
- Ou visualize no GitHub
- Ou use um visualizador Mermaid online

## 🔍 Estrutura do Código

### `lexico.h`
Define:
- Enumeração `TipoToken` com todos os tipos de tokens
- Estrutura `Token` (tipo, lexema, linha, coluna)
- Estrutura `AnalisadorLexico` (estado do analisador)
- Protótipos das funções

### `lexico.c`
Implementa:
- `criar_analisador()`: inicializa o analisador
- `proximo_token()`: retorna o próximo token (implementação do AFD)
- `reconhecer_id()`: reconhece identificadores/palavras-chave
- `reconhecer_numero()`: reconhece números
- `reconhecer_string()`: reconhece strings
- `pular_comentario()`: ignora comentários
- `salvar_tokens()`: processa arquivo e salva tokens

### `main.c`
- Processa argumentos da linha de comando
- Chama o analisador léxico
- Exibe mensagens ao usuário

## 💡 Como Funciona o AFD

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

## ✅ Checklist de Funcionamento

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

## 📚 Referências

- Especificação da linguagem X25a (documento da atividade)
- Conceitos de compiladores e análise léxica
- Autômatos Finitos Determinísticos (AFD)

## 👨‍💻 Autor

Desenvolvido como parte da Atividade Avaliativa de Compiladores - UNIFEI

---

**Dúvidas?** Consulte a documentação do AFD em `AFD.md` ou os comentários no código!
