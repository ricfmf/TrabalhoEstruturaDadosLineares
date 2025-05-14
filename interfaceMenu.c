// Menu principal do sistema utilizando interface em terminal
void menuPrincipal() {
    int opcao;
    char isbn[20];
    int idUsuario;
    char titulo[MAX_STR];
    
    do {
        system("cls || clear"); // Limpar tela (funciona em Windows e Linux/Mac)
        
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
                break;
            case 7:
                printf("\n=== LISTAR EMPRESTIMOS ATIVOS ===\n");
                printf("Digite o ID do usuario: ");
                scanf("%d", &idUsuario);
                listarEmprestimosAtivos(idUsuario);
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
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
        
        if (opcao != 0) {
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
