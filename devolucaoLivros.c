#include "biblioteca.h"
// Função para devolver um livro
int devolverLivro(char* isbn, int idUsuario) {
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
    
    // Procurar o empréstimo na lista
    Emprestimo* atual = listaEmprestimos;
    Emprestimo* encontrado = NULL;
    
    while (atual != NULL) {
        if (strcmp(atual->isbn, isbn) == 0 && atual->idUsuario == idUsuario && atual->status == 1) {
            encontrado = atual;
            break;
        }
        atual = atual->prox;
    }
    
    if (encontrado == NULL) {
        printf("Erro: Este usuario nao possui emprestimo ativo deste livro!\n");
        return 0;
    }
    
    // Calcular multa (se houver atraso)
    time_t dataAtual = time(NULL);
    double multa = 0.0;
    
    if (dataAtual > encontrado->dataDevolucaoPrevista) {
        // Calcular dias de atraso
        double diferenca = difftime(dataAtual, encontrado->dataDevolucaoPrevista);
        int diasAtraso = (int)(diferenca / (24 * 60 * 60));
        
        // Valor da multa: R$ 1,00 por dia de atraso
        multa = diasAtraso * 1.0;
    }
    
    // Atualizar status do empréstimo
    encontrado->status = 0; // Marcado como devolvido
    
    // Incrementar número de cópias disponíveis
    livro->copias++;
    
    // Salvar alterações
    salvarLivrosEmArquivo();
    salvarEmprestimosEmArquivo();
    
    // Exibir informações sobre a devolução
    printf("\nDevolucao realizada com sucesso!\n");
    printf("Livro: %s\n", livro->titulo);
    printf("Usuario: %s\n", usuario->nome);
    
    char dataEmprestimo[20];
    char dataDevolucaoPrevista[20];
    char dataAtualFormatada[20];
    
    formatarData(encontrado->dataEmprestimo, dataEmprestimo, sizeof(dataEmprestimo));
    formatarData(encontrado->dataDevolucaoPrevista, dataDevolucaoPrevista, sizeof(dataDevolucaoPrevista));
    formatarData(dataAtual, dataAtualFormatada, sizeof(dataAtualFormatada));
    
    printf("Data do emprestimo: %s\n", dataEmprestimo);
    printf("Data prevista de devolucao: %s\n", dataDevolucaoPrevista);
    printf("Data de devolucao efetiva: %s\n", dataAtualFormatada);
    
    if (multa > 0) {
        printf("MULTA POR ATRASO: R$ %.2f\n", multa);
    } else {
        printf("Devolucao dentro do prazo. Nenhuma multa aplicada.\n");
    }
    
    // Opcionalmente, registrar a multa em um arquivo separado ou em uma estrutura
    if (multa > 0) {
        registrarMulta(idUsuario, isbn, multa, dataAtual);
    }
    
    return 1;
}

// Estrutura para registrar multas
typedef struct Multa {
    int idUsuario;
    char isbn[20];
    double valor;
    time_t dataDevolucao;
    int paga; // 0 = não paga, 1 = paga
    struct Multa* prox;
} Multa;

Multa* listaMultas = NULL;

// Função para registrar uma multa
void registrarMulta(int idUsuario, char* isbn, double valor, time_t dataDevolucao) {
    Multa* novaMulta = (Multa*)malloc(sizeof(Multa));
    if (novaMulta == NULL) {
        printf("Erro: Memoria insuficiente ao registrar multa!\n");
        return;
    }
    
    novaMulta->idUsuario = idUsuario;
    strcpy(novaMulta->isbn, isbn);
    novaMulta->valor = valor;
    novaMulta->dataDevolucao = dataDevolucao;
    novaMulta->paga = 0; // Inicialmente não paga
    
    // Inserir no início da lista
    novaMulta->prox = listaMultas;
    listaMultas = novaMulta;
    
    // Salvar em arquivo
    salvarMultasEmArquivo();
}

// Função para salvar multas em arquivo
void salvarMultasEmArquivo() {
    FILE* arquivo = fopen("multas.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de multas para gravacao!\n");
        return;
    }
    
    Multa* atual = listaMultas;
    
    // Estrutura temporária sem o ponteiro prox
    typedef struct {
        int idUsuario;
        char isbn[20];
        double valor;
        time_t dataDevolucao;
        int paga;
    } MultaTemp;
    
    // Salvar cada multa
    while (atual != NULL) {
        MultaTemp temp;
        temp.idUsuario = atual->idUsuario;
        strcpy(temp.isbn, atual->isbn);
        temp.valor = atual->valor;
        temp.dataDevolucao = atual->dataDevolucao;
        temp.paga = atual->paga;
        
        fwrite(&temp, sizeof(MultaTemp), 1, arquivo);
        atual = atual->prox;
    }
    
    fclose(arquivo);
}

// Função para carregar multas do arquivo
void carregarMultasDoArquivo() {
    FILE* arquivo = fopen("multas.dat", "rb");
    if (arquivo == NULL) {
        printf("Arquivo de multas nao encontrado. Sera criado um novo ao salvar.\n");
        return;
    }
    
    // Liberar a lista atual
    while (listaMultas != NULL) {
        Multa* temp = listaMultas;
        listaMultas = listaMultas->prox;
        free(temp);
    }
    
    // Estrutura temporária sem o ponteiro prox
    typedef struct {
        int idUsuario;
        char isbn[20];
        double valor;
        time_t dataDevolucao;
        int paga;
    } MultaTemp;
    
    MultaTemp temp;
    
    // Ler cada multa do arquivo
    while (fread(&temp, sizeof(MultaTemp), 1, arquivo) == 1) {
        Multa* novaMulta = (Multa*)malloc(sizeof(Multa));
        if (novaMulta == NULL) {
            continue;
        }
        
        // Copiar dados
        novaMulta->idUsuario = temp.idUsuario;
        strcpy(novaMulta->isbn, temp.isbn);
        novaMulta->valor = temp.valor;
        novaMulta->dataDevolucao = temp.dataDevolucao;
        novaMulta->paga = temp.paga;
        
        // Inserir na lista
        novaMulta->prox = listaMultas;
        listaMultas = novaMulta;
    }
    
    fclose(arquivo);
}

// Função para o menu de devolução de livros
void menuDevolucaoLivro() {
    char isbn[20];
    int idUsuario;
    
    printf("\n=== DEVOLUCAO DE LIVROS ===\n");
    
    printf("ISBN do livro: ");
    scanf("%19s", isbn);
    
    printf("ID do usuario: ");
    scanf("%d", &idUsuario);
    
    devolverLivro(isbn, idUsuario);
}

// Função para listar multas de um usuário
void listarMultasUsuario(int idUsuario) {
    Usuario* usuario = buscarUsuarioPorID(idUsuario);
    if (usuario == NULL) {
        printf("Usuario com ID %d nao encontrado!\n", idUsuario);
        return;
    }
    
    printf("\n=== MULTAS DE %s ===\n", usuario->nome);
    
    Multa* atual = listaMultas;
    int encontrou = 0;
    double total = 0.0;
    
    while (atual != NULL) {
        if (atual->idUsuario == idUsuario && atual->paga == 0) {
            Livro* livro = buscarLivroPorISBN(atual->isbn);
            encontrou = 1;
            
            char dataDevolucao[20];
            formatarData(atual->dataDevolucao, dataDevolucao, sizeof(dataDevolucao));
            
            printf("Livro: %s\n", livro ? livro->titulo : "Desconhecido");
            printf("Data de devolucao: %s\n", dataDevolucao);
            printf("Valor da multa: R$ %.2f\n", atual->valor);
            printf("Status: %s\n", atual->paga ? "Paga" : "Nao paga");
            printf("------------------------------\n");
            
            total += atual->valor;
        }
        atual = atual->prox;
    }
    
    if (!encontrou) {
        printf("Nenhuma multa pendente para este usuario.\n");
    } else {
        printf("\nTotal de multas pendentes: R$ %.2f\n", total);
    }
}

// Função para pagar multa
void pagarMulta(int idUsuario, char* isbn) {
    Usuario* usuario = buscarUsuarioPorID(idUsuario);
    if (usuario == NULL) {
        printf("Usuario com ID %d nao encontrado!\n", idUsuario);
        return;
    }
    
    // Procurar a multa na lista
    Multa* atual = listaMultas;
    Multa* encontrada = NULL;
    
    while (atual != NULL) {
        if (atual->idUsuario == idUsuario && strcmp(atual->isbn, isbn) == 0 && atual->paga == 0) {
            encontrada = atual;
            break;
        }
        atual = atual->prox;
    }
    
    if (encontrada == NULL) {
        printf("Nenhuma multa pendente encontrada para este usuario e livro.\n");
        return;
    }
    
    // Marcar como paga
    encontrada->paga = 1;
    
    // Salvar alterações
    salvarMultasEmArquivo();
    
    printf("Multa de R$ %.2f paga com sucesso!\n", encontrada->valor);
}
