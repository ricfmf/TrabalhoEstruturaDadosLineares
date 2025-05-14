// Função para gerar relatório de livros mais emprestados
void relatorioLivrosMaisEmprestados() {
    printf("\n=== RELATORIO: LIVROS MAIS EMPRESTADOS ===\n");
    
    // Estrutura para contar empréstimos por livro
    typedef struct {
        char isbn[20];
        int contador;
    } ContadorLivro;
    
    // Alocar array de contadores
    int capacidade = 100; // Capacidade inicial
    ContadorLivro* contadores = (ContadorLivro*)malloc(capacidade * sizeof(ContadorLivro));
    if (contadores == NULL) {
        printf("Erro: Memoria insuficiente!\n");
        return;
    }
    
    int totalLivros = 0;
    
    // Percorrer todos os empréstimos (ativos e anteriores)
    Emprestimo* emprestimo = listaEmprestimos;
    while (emprestimo != NULL) {
        // Verificar se o livro já está na lista de contadores
        int encontrado = 0;
        for (int i = 0; i < totalLivros; i++) {
            if (strcmp(contadores[i].isbn, emprestimo->isbn) == 0) {
                contadores[i].contador++;
                encontrado = 1;
                break;
            }
        }
        
        // Se não encontrou, adicionar novo livro
        if (!encontrado) {
            // Verificar se precisa aumentar a capacidade
            if (totalLivros >= capacidade) {
                capacidade *= 2;
                ContadorLivro* temp = (ContadorLivro*)realloc(contadores, capacidade * sizeof(ContadorLivro));
                if (temp == NULL) {
                    printf("Erro: Memoria insuficiente!\n");
                    free(contadores);
                    return;
                }
                contadores = temp;
            }
            
            // Adicionar novo contador
            strcpy(contadores[totalLivros].isbn, emprestimo->isbn);
            contadores[totalLivros].contador = 1;
            totalLivros++;
        }
        
        emprestimo = emprestimo->prox;
    }
    
    // Ordenar contadores em ordem decrescente (algoritmo simples de bubble sort)
    for (int i = 0; i < totalLivros - 1; i++) {
        for (int j = 0; j < totalLivros - i - 1; j++) {
            if (contadores[j].contador < contadores[j + 1].contador) {
                // Trocar posições
                ContadorLivro temp = contadores[j];
                contadores[j] = contadores[j + 1];
                contadores[j + 1] = temp;
            }
        }
    }
    
    // Exibir TOP 10 (ou menos se não houver 10)
    int limite = (totalLivros < 10) ? totalLivros : 10;
    
    if (limite == 0) {
        printf("Nenhum emprestimo registrado no sistema.\n");
    } else {
        printf("TOP %d LIVROS MAIS EMPRESTADOS:\n\n", limite);
        
        for (int i = 0; i < limite; i++) {
            Livro* livro = buscarLivroPorISBN(contadores[i].isbn);
            if (livro != NULL) {
                printf("%d. \"%s\" - %s\n", i + 1, livro->titulo, livro->autor);
                printf("   ISBN: %s\n", livro->isbn);
                printf("   Total de emprestimos: %d\n\n", contadores[i].contador);
            }
        }
    }
    
    free(contadores);
}

// Função para gerar relatório de usuários ativos
void relatorioUsuariosAtivos() {
    printf("\n=== RELATORIO: USUARIOS MAIS ATIVOS ===\n");
    
    // Estrutura para contar empréstimos por usuário
    typedef struct {
        int idUsuario;
        int contador;
    } ContadorUsuario;
    
    // Alocar array de contadores
    int capacidade = 100; // Capacidade inicial
    ContadorUsuario* contadores = (ContadorUsuario*)malloc(capacidade * sizeof(ContadorUsuario));
    if (contadores == NULL) {
        printf("Erro: Memoria insuficiente!\n");
        return;
    }
    
    int totalUsuarios = 0;
    
    // Percorrer todos os empréstimos
    Emprestimo* emprestimo = listaEmprestimos;
    while (emprestimo != NULL) {
        // Verificar se o usuário já está na lista de contadores
        int encontrado = 0;
        for (int i = 0; i < totalUsuarios; i++) {
            if (contadores[i].idUsuario == emprestimo->idUsuario) {
                contadores[i].contador++;
                encontrado = 1;
                break;
            }
        }
        
        // Se não encontrou, adicionar novo usuário
        if (!encontrado) {
            // Verificar se precisa aumentar a capacidade
            if (totalUsuarios >= capacidade) {
                capacidade *= 2;
                ContadorUsuario* temp = (ContadorUsuario*)realloc(contadores, capacidade * sizeof(ContadorUsuario));
                if (temp == NULL) {
                    printf("Erro: Memoria insuficiente!\n");
                    free(contadores);
                    return;
                }
                contadores = temp;
            }
            
            // Adicionar novo contador
            contadores[totalUsuarios].idUsuario = emprestimo->idUsuario;
            contadores[totalUsuarios].contador = 1;
            totalUsuarios++;
        }
        
        emprestimo = emprestimo->prox;
    }
    
    // Ordenar contadores em ordem decrescente
    for (int i = 0; i < totalUsuarios - 1; i++) {
        for (int j = 0; j < totalUsuarios - i - 1; j++) {
            if (contadores[j].contador < contadores[j + 1].contador) {
                // Trocar posições
                ContadorUsuario temp = contadores[j];
                contadores[j] = contadores[j + 1];
                contadores[j + 1] = temp;
            }
        }
    }
    
    // Exibir TOP 10 (ou menos se não houver 10)
    int limite = (totalUsuarios < 10) ? totalUsuarios : 10;
    
    if (limite == 0) {
        printf("Nenhum emprestimo registrado no sistema.\n");
    } else {
        printf("TOP %d USUARIOS MAIS ATIVOS:\n\n", limite);
        
        for (int i = 0; i < limite; i++) {
            Usuario* usuario = buscarUsuarioPorID(contadores[i].idUsuario);
            if (usuario != NULL) {
                printf("%d. %s (ID: %d)\n", i + 1, usuario->nome, usuario->id);
                printf("   Email: %s\n", usuario->email);
                printf("   Total de emprestimos: %d\n\n", contadores[i].contador);
            }
        }
    }
    
    free(contadores);
}
