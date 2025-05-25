#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Constantes
#define MAX_STR 100
#define MAX_TAM 101

// Estruturas
typedef struct Livro {
    char isbn[20];
    char titulo[MAX_STR];
    char autor[MAX_STR];
    int ano;
    int copias;
    int ativo; // 1 para ativo, 0 para inativo
    struct Livro* prox;
} Livro;

typedef struct Usuario {
    int id;
    char nome[MAX_STR];
    char email[MAX_STR];
    char telefone[11];
    int ativo; // 1 para ativo, 0 para inativo
    struct Usuario* prox;
} Usuario;

typedef struct Emprestimo {
    char isbn[20];
    int idUsuario;
    time_t dataEmprestimo;
    time_t dataDevolucaoPrevista;
    int status; // 1 = ativo, 0 = devolvido
    struct Emprestimo* prox;
} Emprestimo;

typedef struct Multa {
    int idUsuario;
    char isbn[20];
    double valor;
    time_t dataDevolucao;
    int paga; // 0 = não paga, 1 = paga
    struct Multa* prox;
} Multa;

// Variáveis globais
extern Livro* tabelaLivros[MAX_TAM];
extern Usuario* tabelaUsuarios[MAX_TAM];
extern Emprestimo* listaEmprestimos;
extern Multa* listaMultas;

// Protótipos das funções - Inicialização
void inicializarTabelas();

// Protótipos das funções - Hash
int hashISBN(char* isbn);
int hashID(int id);

// Protótipos das funções - Cadastro de Livros
Livro* criarLivro(char* isbn, char* titulo, char* autor, int ano, int copias);
int inserirLivro(char* isbn, char* titulo, char* autor, int ano, int copias);
Livro* buscarLivroPorISBN(char* isbn);
void salvarLivrosEmArquivo();
void carregarLivrosDoArquivo();
void menuCadastroLivro();

// Protótipos das funções - Cadastro de Usuários
Usuario* criarUsuario(int id, char* nome, char* email);
int inserirUsuario(int id, char* nome, char* email);
Usuario* buscarUsuarioPorID(int id);
Usuario* buscarUsuarioPorNome(char* nome);
void salvarUsuariosEmArquivo();
void carregarUsuariosDoArquivo();
void menuCadastroUsuario();

// Protótipos das funções - Consulta Rápida
Livro* buscarLivro(char* chave, int porISBN);
Usuario* buscarUsuario(char* chave, int porID);

// Protótipos das funções - Empréstimo
int emprestarLivro(char* isbn, int idUsuario);
void salvarEmprestimosEmArquivo();
void carregarEmprestimosDoArquivo();
void formatarData(time_t data, char* buffer, int tamanho);
void menuEmprestimoLivro();
void listarEmprestimosAtivos(int idUsuario);

// Protótipos das funções - Devolução e Multas
int devolverLivro(char* isbn, int idUsuario);
void registrarMulta(int idUsuario, char* isbn, double valor, time_t dataDevolucao);
void salvarMultasEmArquivo();
void carregarMultasDoArquivo();
void menuDevolucaoLivro();
void listarMultasUsuario(int idUsuario);
void pagarMulta(int idUsuario, char* isbn);

// Protótipos das funções - Histórico
void historicoEmprestimosUsuario(int idUsuario);
void historicoEmprestimosLivro(char* isbn);

// Protótipos das funções - Relatórios
void relatorioLivrosMaisEmprestados();
void relatorioUsuariosAtivos();

// Protótipos das funções - Remoção e Atualização
int removerLivro(char* isbn);
int removerUsuario(int id);
int atualizarLivro(char* isbn, Livro novosDados);
int atualizarUsuario(int id, Usuario novosDados);

// Protótipos das funções - Interface
void menuPrincipal();
char* strlwr(char* str);
int main();

#endif // BIBLIOTECA_H
