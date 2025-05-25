#include "biblioteca.h"

// Função para emprestar um livro
int emprestarLivro(char* isbn, int idUsuario) {
    // Verificar se o livro existe
    Livro* livro = buscarLivroPorISBN(isbn);
    if (livro == NULL) {
        printf("Erro: Livro com ISBN %s nao encontrado!\n", isbn);
        return 0;
    }
    
    // Verificar se o usuário existe
    Usuario* usuario = buscarUsuarioPorID(idUsuario);
    if (usuario == NULL) {
        printf("Erro: Usuario com ID %d nao encontrado!\n", idUsuario);
        return 0;
    }
    
    // Verificar se há cópias disponíveis
    if (livro->copias <= 0) {
        printf("Erro: Nao ha copias disponiveis para o livro '%s'!\n", livro->titulo);
        return 0;
    }
    
    // Verificar se o usuário já tem este livro emprestado
    Emprestimo* atual = listaEmprestimos;
    while (atual != NULL) {
        if (strcmp(atual->isbn, isbn) == 0 && atual->idUsuario == idUsuario && atual->status == 1) {
            printf("Erro: Este usuario ja possui uma copia deste livro emprestada!\n");
            return 0;
        }
        atual = atual->prox;
    }
    
    // Criar novo registro de empréstimo
    Emprestimo* novoEmprestimo = (Emprestimo*)malloc(sizeof(Emprestimo));
    if (novoEmprestimo == NULL) {
        printf("Erro: Memoria insuficiente!\n");
        return 0;
    }
    
    // Preencher dados do empréstimo
    strcpy(novoEmprestimo->isbn, isbn);
    novoEmprestimo->idUsuario = idUsuario;
    novoEmprestimo->dataEmprestimo = time(NULL);
    
    // Definir data de devolução para 14 dias após a data atual
    novoEmprestimo->dataDevolucaoPrevista = novoEmprestimo->dataEmprestimo + (14 * 24 * 60 * 60);
    novoEmprestimo->status = 1; // Empréstimo ativo
    
    // Inserir na lista de empréstimos
    novoEmprestimo->prox = listaEmprestimos;
    listaEmprestimos = novoEmprestimo;
    
    // Atualizar número de cópias disponíveis
    livro->copias--;
    
    // Salvar alterações
    salvarLivrosEmArquivo();
    salvarEmprestimosEmArquivo();
    
    printf("Emprestimo realizado com sucesso!\n");
    printf("Livro: %s\n", livro->titulo);
    printf("Usuario: %s\n", usuario->nome);
    
    // Exibir data de devolução prevista
    char dataFormatada[100];
    struct tm* infoData = localtime(&novoEmprestimo->dataDevolucaoPrevista);
    strftime(dataFormatada, sizeof(dataFormatada), "%d/%m/%Y", infoData);
    printf("Data de devolucao prevista: %s\n", dataFormatada);
    
    return 1;
}

// Função para salvar empréstimos em arquivo
void salvarEmprestimosEmArquivo() {
    FILE* arquivo = fopen("emprestimos.log", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de emprestimos para gravacao!\n");
        return;
    }
    
    Emprestimo* atual = listaEmprestimos;
    
    // Estrutura temporária sem o ponteiro prox
    typedef struct {
        char isbn[20];
        int idUsuario;
        time_t dataEmprestimo;
        time_t dataDevolucaoPrevista;
        int status;
    } EmprestimoTemp;
    
    // Salvar cada empréstimo
    while (atual != NULL) {
        EmprestimoTemp temp;
        strcpy(temp.isbn, atual->isbn);
        temp.idUsuario = atual->idUsuario;
        temp.dataEmprestimo = atual->dataEmprestimo;
        temp.dataDevolucaoPrevista = atual->dataDevolucaoPrevista;
        temp.status = atual->status;
        
        fwrite(&temp, sizeof(EmprestimoTemp), 1, arquivo);
        atual = atual->prox;
    }
    
    fclose(arquivo);
}

// Função para carregar empréstimos do arquivo
void carregarEmprestimosDoArquivo() {
    FILE* arquivo = fopen("emprestimos.log", "rb");
    if (arquivo == NULL) {
        printf("Arquivo de emprestimos nao encontrado. Sera criado um novo ao salvar.\n");
        return;
    }
    
    // Liberar a lista atual
    while (listaEmprestimos != NULL) {
        Emprestimo* temp = listaEmprestimos;
        listaEmprestimos = listaEmprestimos->prox;
        free(temp);
    }
    
    // Estrutura temporária sem o ponteiro prox
    typedef struct {
        char isbn[20];
        int idUsuario;
        time_t dataEmprestimo;
        time_t dataDevolucaoPrevista;
        int status;
    } EmprestimoTemp;
    
    EmprestimoTemp temp;
    
    // Ler cada empréstimo do arquivo
    while (fread(&temp, sizeof(EmprestimoTemp), 1, arquivo) == 1) {
        Emprestimo* novoEmprestimo = (Emprestimo*)malloc(sizeof(Emprestimo));
        if (novoEmprestimo == NULL) {
            continue;
        }
        
        // Copiar dados
        strcpy(novoEmprestimo->isbn, temp.isbn);
        novoEmprestimo->idUsuario = temp.idUsuario;
        novoEmprestimo->dataEmprestimo = temp.dataEmprestimo;
        novoEmprestimo->dataDevolucaoPrevista = temp.dataDevolucaoPrevista;
        novoEmprestimo->status = temp.status;
        
        // Inserir na lista
        novoEmprestimo->prox = listaEmprestimos;
        listaEmprestimos = novoEmprestimo;
    }
    
    fclose(arquivo);
}

// Função para exibir a data formatada
void formatarData(time_t data, char* buffer, int tamanho) {
    struct tm* infoData = localtime(&data);
    strftime(buffer, tamanho, "%d/%m/%Y", infoData);
}

// Função para o menu de empréstimo de livros
void menuEmprestimoLivro() {
    char isbn[20];
    int idUsuario;
    
    printf("\n=== EMPRESTIMO DE LIVROS ===\n");
    
    printf("ISBN do livro: ");
    scanf("%19s", isbn);
    
    printf("ID do usuario: ");
    scanf("%d", &idUsuario);
    
    emprestarLivro(isbn, idUsuario);
}

// Função para listar empréstimos ativos de um usuário
void listarEmprestimosAtivos(int idUsuario) {
    Usuario* usuario = buscarUsuarioPorID(idUsuario);
    if (usuario == NULL) {
        printf("Usuario com ID %d nao encontrado!\n", idUsuario);
        return;
    }
    
    printf("\n=== EMPRESTIMOS ATIVOS DE %s ===\n", usuario->nome);
    
    Emprestimo* atual = listaEmprestimos;
    int encontrou = 0;
    
    while (atual != NULL) {
        if (atual->idUsuario == idUsuario && atual->status == 1) {
            Livro* livro = buscarLivroPorISBN(atual->isbn);
            if (livro != NULL) {
                encontrou = 1;
                
                char dataEmprestimo[20];
                char dataDevolucao[20];
                formatarData(atual->dataEmprestimo, dataEmprestimo, sizeof(dataEmprestimo));
                formatarData(atual->dataDevolucaoPrevista, dataDevolucao, sizeof(dataDevolucao));
                
                printf("Livro: %s\n", livro->titulo);
                printf("Autor: %s\n", livro->autor);
                printf("Emprestado em: %s\n", dataEmprestimo);
                printf("Devolucao prevista: %s\n", dataDevolucao);
                printf("------------------------------\n");
            }
        }
        atual = atual->prox;
    }
    
    if (!encontrou) {
        printf("Nenhum emprestimo ativo para este usuario.\n");
    }
}
