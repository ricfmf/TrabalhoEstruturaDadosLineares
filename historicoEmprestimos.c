#include "biblioteca.h"

void historicoEmprestimosUsuario(int idUsuario) {
    Usuario* usuario = buscarUsuarioPorID(idUsuario);
    if (usuario == NULL) {
        printf("Usuario com ID %d nao encontrado!\n", idUsuario);
        return;
    }
    
    printf("\n=== HISTORICO DE EMPRESTIMOS: %s ===\n", usuario->nome);
    
    Emprestimo* atual = listaEmprestimos;
    int encontrou = 0;
    
    while (atual != NULL) {
        if (atual->idUsuario == idUsuario) {
            Livro* livro = buscarLivroPorISBN(atual->isbn);
            if (livro != NULL) {
                encontrou = 1;
                
                char dataEmprestimo[20];
                char dataDevolucaoPrev[20];
                formatarData(atual->dataEmprestimo, dataEmprestimo, sizeof(dataEmprestimo));
                formatarData(atual->dataDevolucaoPrevista, dataDevolucaoPrev, sizeof(dataDevolucaoPrev));
                
                printf("Livro: %s\n", livro->titulo);
                printf("Autor: %s\n", livro->autor);
                printf("ISBN: %s\n", livro->isbn);
                printf("Data do emprestimo: %s\n", dataEmprestimo);
                printf("Data prevista para devolucao: %s\n", dataDevolucaoPrev);
                printf("Status: %s\n", atual->status ? "Em andamento" : "Devolvido");
                printf("------------------------------\n");
            }
        }
        atual = atual->prox;
    }
    
    if (!encontrou) {
        printf("Nenhum emprestimo encontrado para este usuario.\n");
    }
}

// Função para exibir histórico de empréstimos de um livro
void historicoEmprestimosLivro(char* isbn) {
    Livro* livro = buscarLivroPorISBN(isbn);
    if (livro == NULL) {
        printf("Livro com ISBN %s nao encontrado!\n", isbn);
        return;
    }
    
    printf("\n=== HISTORICO DE EMPRESTIMOS: %s ===\n", livro->titulo);
    
    Emprestimo* atual = listaEmprestimos;
    int encontrou = 0;
    
    while (atual != NULL) {
        if (strcmp(atual->isbn, isbn) == 0) {
            Usuario* usuario = buscarUsuarioPorID(atual->idUsuario);
            if (usuario != NULL) {
                encontrou = 1;
                
                char dataEmprestimo[20];
                char dataDevolucaoPrev[20];
                formatarData(atual->dataEmprestimo, dataEmprestimo, sizeof(dataEmprestimo));
                formatarData(atual->dataDevolucaoPrevista, dataDevolucaoPrev, sizeof(dataDevolucaoPrev));
                
                printf("Usuario: %s (ID: %d)\n", usuario->nome, usuario->id);
                printf("Data do emprestimo: %s\n", dataEmprestimo);
                printf("Data prevista para devolucao: %s\n", dataDevolucaoPrev);
                printf("Status: %s\n", atual->status ? "Em andamento" : "Devolvido");
                printf("------------------------------\n");
            }
        }
        atual = atual->prox;
    }
    
    if (!encontrou) {
        printf("Nenhum emprestimo encontrado para este livro.\n");
    }
}
