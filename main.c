#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"
// Declaração das variáveis globais
Livro* tabelaLivros[MAX_TAM];
Usuario* tabelaUsuarios[MAX_TAM];
Emprestimo* listaEmprestimos = NULL;
Multa* listaMultas = NULL;

// Menu principal do sistema utilizando interface em terminal
void menuPrincipal() {
    int opcao;
    char isbn[20];
    int idUsuario;
    char titulo[MAX_STR];
    char confirmacao;
    
    do {
        //system("cls || clear"); // Limpar tela (funciona em Windows e Linux/Mac)//
        
        printf("\n=== SISTEMA DE GERENCIAMENTO DE BIBLIOTECA ===\n");
        printf("1. Cadastrar Livro\n");
        printf("2. Cadastrar Usuario\n");
        printf("3. Emprestar Livro\n");
        printf("4. Devolver Livro\n");
        printf("5. Consultar Livro\n");
        printf("6. Consultar Usuario\n");
        printf("7. Listar Emprestimos Ativos\n");
        printf("8. Relatorios\n");
        printf("9. Historico de Emprestimos\n");
        printf("10. Gerenciar Multas\n");
        printf("11. Excluir Livro\n");
        printf("12. Excluir Usuario\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Consumir o '\n' pendente
        
        switch (opcao) {
            case 1:
                menuCadastroLivro();
                break;
            case 2:
                menuCadastroUsuario();
                break;
            case 3:
                menuEmprestimoLivro();
                break;
            case 4:
                menuDevolucaoLivro();
                break;
            case 5:
                printf("\n=== CONSULTAR LIVRO ===\n");
                printf("1. Buscar por ISBN\n");
                printf("2. Buscar por Titulo\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &opcao);
                getchar(); // Consumir o '\n' pendente
                
                if (opcao == 1) {
                    printf("Digite o ISBN: ");
                    scanf("%19s", isbn);
                    
                    Livro* livro = buscarLivroPorISBN(isbn);
                    if (livro != NULL) {
                        printf("\n=== LIVRO ENCONTRADO ===\n");
                        printf("Titulo: %s\n", livro->titulo);
                        printf("Autor: %s\n", livro->autor);
                        printf("Ano: %d\n", livro->ano);
                        printf("ISBN: %s\n", livro->isbn);
                        printf("Copias disponiveis: %d\n", livro->copias);
                    } else {
                        printf("Livro nao encontrado!\n");
                    }
                } else if (opcao == 2) {
                    printf("Digite o Titulo: ");
                    fgets(titulo, MAX_STR, stdin);
                    titulo[strcspn(titulo, "\n")] = '\0';
                    
                    // Função auxiliar para buscar por título (busca sequencial em toda tabela)
                    int encontrados = 0;
                    for (int i = 0; i < MAX_TAM; i++) {
                        Livro* atual = tabelaLivros[i];
                        while (atual != NULL) {
                            // Busca parcial case-insensitive
                            if (strstr(strlwr(atual->titulo), strlwr(titulo)) != NULL) {
                                encontrados++;
                                printf("\n=== LIVRO ENCONTRADO ===\n");
                                printf("Titulo: %s\n", atual->titulo);
                                printf("Autor: %s\n", atual->autor);
                                printf("Ano: %d\n", atual->ano);
                                printf("ISBN: %s\n", atual->isbn);
                                printf("Copias disponiveis: %d\n", atual->copias);
                            }
                            atual = atual->prox;
                        }
                    }
                    
                    if (encontrados == 0) {
                        printf("Nenhum livro encontrado com este titulo!\n");
                    }
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
            case 6:
                printf("\n=== CONSULTAR USUARIO ===\n");
                printf("1. Buscar por ID\n");
                printf("2. Buscar por Nome\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &opcao);
                getchar(); // Consumir o '\n' pendente
                
                if (opcao == 1) {
                    printf("Digite o ID: ");
                    scanf("%d", &idUsuario);
                    
                    Usuario* usuario = buscarUsuarioPorID(idUsuario);
                    if (usuario != NULL) {
                        printf("\n=== USUARIO ENCONTRADO ===\n");
                        printf("Nome: %s\n", usuario->nome);
                        printf("ID: %d\n", usuario->id);
                        printf("Email: %s\n", usuario->email);
                    } else {
                        printf("Usuario nao encontrado!\n");
                    }
                } else if (opcao == 2) {
                    char nome[MAX_STR];
                    printf("Digite o Nome: ");
                    fgets(nome, MAX_STR, stdin);
                    nome[strcspn(nome, "\n")] = '\0';
                    
                    // Função auxiliar para buscar por nome (busca sequencial)
                    int encontrados = 0;
                    for (int i = 0; i < MAX_TAM; i++) {
                        Usuario* atual = tabelaUsuarios[i];
                        while (atual != NULL) {
                            // Busca parcial case-insensitive
                            if (strstr(strlwr(atual->nome), strlwr(nome)) != NULL) {
                                encontrados++;
                                printf("\n=== USUARIO ENCONTRADO ===\n");
                                printf("Nome: %s\n", atual->nome);
                                printf("ID: %d\n", atual->id);
                                printf("Email: %s\n", atual->email);
                            }
                            atual = atual->prox;
                        }
                    }
                    
                    if (encontrados == 0) {
                        printf("Nenhum usuario encontrado com este nome!\n");
                    }
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
            case 7:
                printf("\n=== LISTAR EMPRESTIMOS ATIVOS ===\n");
                printf("Digite o ID do usuario: ");
                scanf("%d", &idUsuario);
                listarEmprestimosAtivos(idUsuario);
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
            case 8:
                printf("\n=== RELATORIOS ===\n");
                printf("1. Livros mais emprestados\n");
                printf("2. Usuarios mais ativos\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &opcao);
                
                if (opcao == 1) {
                    relatorioLivrosMaisEmprestados();
                } else if (opcao == 2) {
                    relatorioUsuariosAtivos();
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
            case 9:
                printf("\n=== HISTORICO DE EMPRESTIMOS ===\n");
                printf("1. Por Usuario\n");
                printf("2. Por Livro\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &opcao);
                
                if (opcao == 1) {
                    printf("Digite o ID do usuario: ");
                    scanf("%d", &idUsuario);
                    historicoEmprestimosUsuario(idUsuario);
                } else if (opcao == 2) {
                    printf("Digite o ISBN do livro: ");
                    scanf("%19s", isbn);
                    historicoEmprestimosLivro(isbn);
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
            case 10:
                printf("\n=== GERENCIAR MULTAS ===\n");
                printf("1. Listar multas de um usuario\n");
                printf("2. Pagar multa\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &opcao);
                
                if (opcao == 1) {
                    printf("Digite o ID do usuario: ");
                    scanf("%d", &idUsuario);
                    listarMultasUsuario(idUsuario);
                } else if (opcao == 2) {
                    printf("Digite o ID do usuario: ");
                    scanf("%d", &idUsuario);
                    
                    // Primeiro, listar multas para o usuário escolher
                    listarMultasUsuario(idUsuario);
                    
                    printf("Digite o ISBN do livro para pagar a multa: ");
                    scanf("%19s", isbn);
                    pagarMulta(idUsuario, isbn);
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
            case 11:
                printf("\n=== EXCLUIR LIVRO ===\n");
                printf("Digite o ISBN do livro a ser excluido: ");
                scanf("%19s", isbn);
                
                // Primeiro, mostrar os dados do livro
                Livro* livro = buscarLivroPorISBN(isbn);
                if (livro != NULL && livro->ativo) {
                    printf("\n=== DADOS DO LIVRO ===\n");
                    printf("Titulo: %s\n", livro->titulo);
                    printf("Autor: %s\n", livro->autor);
                    printf("Ano: %d\n", livro->ano);
                    printf("ISBN: %s\n", livro->isbn);
                    printf("Copias disponiveis: %d\n", livro->copias);
                    
                    printf("\nTem certeza que deseja excluir este livro? (s/n): ");
                    scanf(" %c", &confirmacao);
                    
                    if (confirmacao == 's' || confirmacao == 'S') {
                        if (removerLivro(isbn)) {
                            printf("Livro excluido com sucesso!\n");
                            salvarLivrosEmArquivo(); // Atualizar arquivo
                        } else {
                            printf("Erro ao excluir livro!\n");
                        }
                    } else {
                        printf("Operacao cancelada.\n");
                    }
                } else {
                    printf("Livro nao encontrado ou ja foi excluido!\n");
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                getchar(); // Consumir o '\n' pendente do scanf anterior
                break;
            case 12:
                printf("\n=== EXCLUIR USUARIO ===\n");
                printf("Digite o ID do usuario a ser excluido: ");
                scanf("%d", &idUsuario);
                
                // Primeiro, mostrar os dados do usuário
                Usuario* usuario = buscarUsuarioPorID(idUsuario);
                if (usuario != NULL && usuario->ativo) {
                    printf("\n=== DADOS DO USUARIO ===\n");
                    printf("Nome: %s\n", usuario->nome);
                    printf("ID: %d\n", usuario->id);
                    printf("Email: %s\n", usuario->email);
                    
                    // Verificar se há empréstimos ativos
                    int temEmprestimosAtivos = 0;
                    Emprestimo* emp = listaEmprestimos;
                    while (emp != NULL) {
                        if (emp->idUsuario == idUsuario && emp->status == 1) {
                            temEmprestimosAtivos = 1;
                            break;
                        }
                        emp = emp->prox;
                    }
                    
                    if (temEmprestimosAtivos) {
                        printf("\nATENCAO: Este usuario possui emprestimos ativos!\n");
                        printf("Nao e possivel excluir usuario com emprestimos pendentes.\n");
                    } else {
                        printf("\nTem certeza que deseja excluir este usuario? (s/n): ");
                        scanf(" %c", &confirmacao);
                        
                        if (confirmacao == 's' || confirmacao == 'S') {
                            if (removerUsuario(idUsuario)) {
                                printf("Usuario excluido com sucesso!\n");
                                salvarUsuariosEmArquivo(); // Atualizar arquivo
                            } else {
                                printf("Erro ao excluir usuario!\n");
                            }
                        } else {
                            printf("Operacao cancelada.\n");
                        }
                    }
                } else {
                    printf("Usuario nao encontrado ou ja foi excluido!\n");
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                getchar(); // Consumir o '\n' pendente do scanf anterior
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                printf("\nPressione Enter para continuar...");
                getchar();
        }
        
    } while (opcao != 0);
}

// Função auxiliar para converter strings para minúsculas
char* strlwr(char* str) {
    char* p = str;
    while (*p) {
        *p = tolower((unsigned char)*p);
        p++;
    }
    return str;
}

// Função principal
int main() {
    // Inicializar tabelas hash
    inicializarTabelas();
    
    // Carregar dados dos arquivos
    carregarLivrosDoArquivo();
    carregarUsuariosDoArquivo();
    carregarEmprestimosDoArquivo();
    carregarMultasDoArquivo();
    
    // Iniciar menu principal
    menuPrincipal();
    
    return 0;
}
