#include "biblioteca.h"

 
// Função para "remover" (marcar como inativo) um livro

int removerLivro(char* isbn) {

    int indice = hashISBN(isbn);

    Livro* atual = tabelaLivros[indice];

    while (atual != NULL) {

        if (strcmp(atual->isbn, isbn) == 0 && atual->ativo) {

            atual->ativo = 0; // Marca como inativo

            // Aqui também atualizaria o arquivo se necessário

            return 1; // Sucesso

        }

        atual = atual->prox;

    }

    return 0; // Não encontrado

}
 
// Função para "remover" (marcar como inativo) um usuário

int removerUsuario(int id) {

    int indice = hashID(id);

    Usuario* atual = tabelaUsuarios[indice];

    while (atual != NULL) {

        if (atual->id == id && atual->ativo) {

            atual->ativo = 0; // Marca como inativo

            // Aqui também atualizaria o arquivo se necessário

            return 1; // Sucesso

        }

        atual = atual->prox;

    }

    return 0; // Não encontrado

}
 
// Função para atualizar um livro

int atualizarLivro(char* isbn, Livro novosDados) {

    int indice = hashISBN(isbn);

    Livro* atual = tabelaLivros[indice];

    while (atual != NULL) {

        if (strcmp(atual->isbn, isbn) == 0 && atual->ativo) {

            // Atualiza os campos (exceto ISBN que é a chave)

            strcpy(atual->titulo, novosDados.titulo);

            strcpy(atual->autor, novosDados.autor);

            atual->ano = novosDados.ano;

            atual->copias = novosDados.copias;

            // Aqui também atualizaria o arquivo se necessário

            return 1; // Sucesso

        }

        atual = atual->prox;

    }

    return 0; // Não encontrado

}
 
// Função para atualizar um usuário

int atualizarUsuario(int id, Usuario novosDados) {

    int indice = hashID(id);

    Usuario* atual = tabelaUsuarios[indice];

    while (atual != NULL) {

        if (atual->id == id && atual->ativo) {

            // Atualiza os campos (exceto ID que é a chave)

            strcpy(atual->nome, novosDados.nome);

            strcpy(atual->email, novosDados.email);

            // Aqui também atualizaria o arquivo se necessário

            return 1; // Sucesso

        }

        atual = atual->prox;

    }

    return 0; // Não encontrado

}
 
