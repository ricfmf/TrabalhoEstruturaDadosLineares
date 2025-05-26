#include "biblioteca.h"

// Função para "remover" (marcar como inativo) um livro
int removerLivro(char* isbn) {
    int indice = hashISBN(isbn);
    Livro* atual = tabelaLivros[indice];

    while (atual != NULL) {
        if (strcmp(atual->isbn, isbn) == 0 && atual->ativo) {
            // Verificar se há empréstimos ativos para este livro
            Emprestimo* emp = listaEmprestimos;
            while (emp != NULL) {
                if (strcmp(emp->isbn, isbn) == 0 && emp->status == 1) {
                    printf("AVISO: Este livro possui emprestimos ativos!\n");
                    printf("Recomenda-se aguardar a devolucao antes de excluir.\n");
                    break;
                }
                emp = emp->prox;
            }
            
            atual->ativo = 0; // Marca como inativo
            printf("Livro marcado como inativo no sistema.\n");
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
            // Verificar se há empréstimos ativos para este usuário
            Emprestimo* emp = listaEmprestimos;
            int temEmprestimosAtivos = 0;
            while (emp != NULL) {
                if (emp->idUsuario == id && emp->status == 1) {
                    temEmprestimosAtivos = 1;
                    break;
                }
                emp = emp->prox;
            }
            
            if (temEmprestimosAtivos) {
                printf("ERRO: Usuario possui emprestimos ativos!\n");
                printf("Nao e possivel excluir usuario com emprestimos pendentes.\n");
                return 0; // Falha - não pode remover
            }
            
            // Verificar se há multas não pagas
            Multa* multa = listaMultas;
            int temMultasNaoPagas = 0;
            while (multa != NULL) {
                if (multa->idUsuario == id && !multa->paga) {
                    temMultasNaoPagas = 1;
                    break;
                }
                multa = multa->prox;
            }
            
            if (temMultasNaoPagas) {
                printf("AVISO: Usuario possui multas nao pagas!\n");
                printf("Recomenda-se quitar as multas antes de excluir.\n");
            }
            
            atual->ativo = 0; // Marca como inativo
            printf("Usuario marcado como inativo no sistema.\n");
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
            
            // Salvar alterações no arquivo
            salvarLivrosEmArquivo();
            printf("Livro atualizado com sucesso!\n");
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
            strcpy(atual->telefone, novosDados.telefone);
            
            // Salvar alterações no arquivo
            salvarUsuariosEmArquivo();
            printf("Usuario atualizado com sucesso!\n");
            return 1; // Sucesso
        }
        atual = atual->prox;
    }
    return 0; // Não encontrado
}

// Função para listar todos os livros ativos (útil para verificação)
void listarLivrosAtivos() {
    printf("\n=== LIVROS ATIVOS NO SISTEMA ===\n");
    int encontrados = 0;
    
    for (int i = 0; i < MAX_TAM; i++) {
        Livro* atual = tabelaLivros[i];
        while (atual != NULL) {
            if (atual->ativo) {
                printf("ISBN: %s | Titulo: %s | Autor: %s | Copias: %d\n",
                       atual->isbn, atual->titulo, atual->autor, atual->copias);
                encontrados++;
            }
            atual = atual->prox;
        }
    }
    
    if (encontrados == 0) {
        printf("Nenhum livro ativo encontrado.\n");
    } else {
        printf("\nTotal de livros ativos: %d\n", encontrados);
    }
}

// Função para listar todos os usuários ativos (útil para verificação)
void listarUsuariosAtivos() {
    printf("\n=== USUARIOS ATIVOS NO SISTEMA ===\n");
    int encontrados = 0;
    
    for (int i = 0; i < MAX_TAM; i++) {
        Usuario* atual = tabelaUsuarios[i];
        while (atual != NULL) {
            if (atual->ativo) {
                printf("ID: %d | Nome: %s | Email: %s\n",
                       atual->id, atual->nome, atual->email);
                encontrados++;
            }
            atual = atual->prox;
        }
    }
    
    if (encontrados == 0) {
        printf("Nenhum usuario ativo encontrado.\n");
    } else {
        printf("\nTotal de usuarios ativos: %d\n", encontrados);
    }
}
