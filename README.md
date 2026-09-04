# 📚 Sistema de Gerenciamento de Biblioteca

> Trabalho acadêmico desenvolvido para a disciplina de **Estrutura de Dados Lineares**.  
> Sistema de gerenciamento de biblioteca implementado em **linguagem C**, com foco na aplicação prática de estruturas de dados lineares como **listas encadeadas** e **tabelas hash**.

---

## 📋 Sobre o Projeto

O sistema simula o funcionamento de uma biblioteca, permitindo o gerenciamento completo de livros, usuários, empréstimos, devoluções e multas por atraso — tudo via interface interativa em terminal.

As principais estruturas de dados utilizadas são:

- **Tabela Hash** com encadeamento externo para armazenar livros (indexados por ISBN) e usuários (indexados por ID), garantindo buscas eficientes.
- **Lista Encadeada Simples** para o controle de empréstimos ativos e multas pendentes.

---

## ✨ Funcionalidades

- **Cadastro de Livros** — título, autor, ano, ISBN e quantidade de cópias
- **Cadastro de Usuários** — nome, e-mail e telefone
- **Empréstimo de Livros** — com registro de data de devolução prevista
- **Devolução de Livros** — com cálculo automático de multa por atraso
- **Consulta Rápida** — busca de livros por ISBN ou título (parcial, sem distinção de maiúsculas) e de usuários por ID ou nome
- **Histórico de Empréstimos** — por usuário ou por livro
- **Relatórios** — livros mais emprestados e usuários mais ativos
- **Gerenciamento de Multas** — listagem e registro de pagamento
- **Exclusão com validação** — impede remoção de usuário com empréstimos ativos
- **Persistência em arquivos** — dados salvos e carregados automaticamente a cada execução

---

## 🗂️ Estrutura do Projeto

```
TrabalhoEstruturaDadosLineares/
│
├── biblioteca.h            # Header principal: structs, constantes e protótipos
├── main.c                  # Ponto de entrada e menu interativo do sistema
├── cadastroLivros.c        # Inserção, busca e persistência de livros
├── cadastroUsuarios.c      # Inserção, busca e persistência de usuários
├── consultaRapida.c        # Busca por título/nome com suporte a busca parcial
├── emprestimoLivros.c      # Lógica de empréstimo e listagem de ativos
├── devolucaoLivros.c       # Lógica de devolução, multas e pagamento
├── historicoEmprestimos.c  # Histórico filtrado por usuário ou livro
├── relatorios.c            # Relatórios de uso do acervo e dos usuários
└── remocaoAtualizacao.c    # Remoção (soft delete) e atualização de registros
```

---

## 🏗️ Estruturas de Dados

Definidas em `biblioteca.h`:

```c
// Nó da Tabela Hash de Livros (encadeamento externo)
typedef struct Livro {
    char isbn[20];
    char titulo[100];
    char autor[100];
    int ano;
    int copias;
    int ativo;          // soft delete: 1 = ativo, 0 = removido
    struct Livro* prox;
} Livro;

// Nó da Tabela Hash de Usuários
typedef struct Usuario {
    int id;
    char nome[100];
    char email[100];
    char telefone[11];
    int ativo;
    struct Usuario* prox;
} Usuario;

// Nó da Lista de Empréstimos
typedef struct Emprestimo {
    char isbn[20];
    int idUsuario;
    time_t dataEmprestimo;
    time_t dataDevolucaoPrevista;
    int status;         // 1 = ativo, 0 = devolvido
    struct Emprestimo* prox;
} Emprestimo;

// Nó da Lista de Multas
typedef struct Multa {
    int idUsuario;
    char isbn[20];
    double valor;
    time_t dataDevolucao;
    int paga;           // 0 = pendente, 1 = paga
    struct Multa* prox;
} Multa;
```

**Tamanho da tabela hash:** `MAX_TAM = 101` (número primo para melhor distribuição)

---

## 🚀 Como Compilar e Executar

### Pré-requisitos

- Compilador C (GCC recomendado)
- Sistema operacional: Windows, Linux ou macOS

### Compilação

```bash
gcc main.c cadastroLivros.c cadastroUsuarios.c consultaRapida.c \
    emprestimoLivros.c devolucaoLivros.c historicoEmprestimos.c \
    relatorios.c remocaoAtualizacao.c -o biblioteca
```

### Execução

```bash
# Linux / macOS
./biblioteca

# Windows
biblioteca.exe
```

---

## 🖥️ Menu do Sistema

```
=== SISTEMA DE GERENCIAMENTO DE BIBLIOTECA ===
 1. Cadastrar Livro
 2. Cadastrar Usuario
 3. Emprestar Livro
 4. Devolver Livro
 5. Consultar Livro
 6. Consultar Usuario
 7. Listar Emprestimos Ativos
 8. Relatorios
 9. Historico de Emprestimos
10. Gerenciar Multas
11. Excluir Livro
12. Excluir Usuario
 0. Sair
```

---

## 💾 Persistência de Dados

O sistema salva e carrega automaticamente todos os dados em arquivos ao iniciar e encerrar:

| Arquivo | Conteúdo |
|---|---|
| `livros.dat` | Acervo completo de livros |
| `usuarios.dat` | Cadastro de usuários |
| `emprestimos.dat` | Histórico de empréstimos |
| `multas.dat` | Registro de multas |

---

## 👥 Autores

Desenvolvido por **ricfmf** e equipe para a disciplina de Estrutura de Dados Lineares.

---

## 📄 Licença

Projeto acadêmico — sem licença de distribuição definida.
