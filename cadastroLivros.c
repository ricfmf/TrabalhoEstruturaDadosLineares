typedef struct Livro {
    char isbn[20];
    char titulo[MAX_STR];
    char autor[MAX_STR];
    int ano;
    int copias;
    struct Livro* prox; // para tratamento de colisões (encadeamento)
} Livro;
 
typedef struct Usuario {
    int id;
    char nome[MAX_STR];
    char email[MAX_STR];
    char telefone[11];
    struct Usuario* prox;
} Usuario;
 
// Tabelas Hash
Livro* tabelaLivros[MAX_TAM];
Usuario* tabelaUsuarios[MAX_TAM];
// Tabelas Hash Livro* tabelaLivros[MAX_TAM]; Usuario* tabelaUsuarios[MAX_TAM];

void inicializarTabelas() {
    for (int i = 0; i < MAX_TAM; i++) {
        tabelaLivros[i] = NULL;
        tabelaUsuarios[i] = NULL;
    }
}

int hashISBN(char* isbn) {
    int soma = 0;
    for (int i = 0; isbn[i] != '\0'; i++) {
        soma += isbn[i];
    }
    return soma % MAX_TAM;
}
int hashID(int id) {
    return id % MAX_TAM;
}


// Função para cadastrar um novo livro na tabela hash
Livro* criarLivro(char* isbn, char* titulo, char* autor, int ano, int copias) {
    Livro* novoLivro = (Livro*)malloc(sizeof(Livro));
    if (novoLivro == NULL) {
        printf("Erro: Memoria insuficiente!\n");
        return NULL;
    }
    
    strcpy(novoLivro->isbn, isbn);
    strcpy(novoLivro->titulo, titulo);
    strcpy(novoLivro->autor, autor);
    novoLivro->ano = ano;
    novoLivro->copias = copias;
    novoLivro->prox = NULL;
    
    return novoLivro;
}

// Função para inserir um livro na tabela hash
int inserirLivro(char* isbn, char* titulo, char* autor, int ano, int copias) {
    // Verificar se o ISBN já existe
    if (buscarLivroPorISBN(isbn) != NULL) {
        printf("Erro: Livro com ISBN %s ja cadastrado!\n", isbn);
        return 0;
    }
    
    // Calcular índice na tabela hash
    int indice = hashISBN(isbn);
    
    // Criar o novo livro
    Livro* novoLivro = criarLivro(isbn, titulo, autor, ano, copias);
    if (novoLivro == NULL) {
        return 0;
    }
    
    // Inserir no início da lista encadeada (tratamento de colisão)
    if (tabelaLivros[indice] == NULL) {
        tabelaLivros[indice] = novoLivro;
    } else {
        novoLivro->prox = tabelaLivros[indice];
        tabelaLivros[indice] = novoLivro;
    }
    
    // Salvar em arquivo
    salvarLivrosEmArquivo();
    
    printf("Livro cadastrado com sucesso!\n");
    return 1;
}

// Função auxiliar para buscar um livro pelo ISBN
Livro* buscarLivroPorISBN(char* isbn) {
    int indice = hashISBN(isbn);
    Livro* atual = tabelaLivros[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->isbn, isbn) == 0) {
            return atual; // Livro encontrado
        }
        atual = atual->prox;
    }
    
    return NULL; // Livro não encontrado
}

// Função para salvar todos os livros em arquivo
void salvarLivrosEmArquivo() {
    FILE* arquivo = fopen("livros.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para gravacao!\n");
        return;
    }
    
    // Percorrer toda a tabela hash
    for (int i = 0; i < MAX_TAM; i++) {
        Livro* atual = tabelaLivros[i];
        
        // Percorrer cada lista encadeada
        while (atual != NULL) {
            // Escrever o livro no arquivo
            fwrite(atual, sizeof(Livro) - sizeof(Livro*), 1, arquivo);
            atual = atual->prox;
        }
    }
    
    fclose(arquivo);
}

// Função para carregar livros do arquivo para a tabela hash
void carregarLivrosDoArquivo() {
    FILE* arquivo = fopen("livros.dat", "rb");
    if (arquivo == NULL) {
        printf("Arquivo de livros nao encontrado. Sera criado um novo ao salvar.\n");
        return;
    }
    
    // Estrutura temporária sem o ponteiro prox
    typedef struct {
        char isbn[20];
        char titulo[MAX_STR];
        char autor[MAX_STR];
        int ano;
        int copias;
    } LivroTemp;
    
    LivroTemp livroTemp;
    
    // Ler cada livro do arquivo
    while (fread(&livroTemp, sizeof(LivroTemp), 1, arquivo) == 1) {
        // Inserir na tabela hash
        inserirLivro(livroTemp.isbn, livroTemp.titulo, livroTemp.autor, livroTemp.ano, livroTemp.copias);
    }
    
    fclose(arquivo);
}

// Função para o menu de cadastro de livros
void menuCadastroLivro() {
    char isbn[20];
    char titulo[MAX_STR];
    char autor[MAX_STR];
    int ano, copias;
    
    printf("\n=== CADASTRO DE LIVROS ===\n");
    
    printf("ISBN: ");
    scanf("%19s", isbn);
    getchar(); // Consumir o '\n' pendente
    
    printf("Titulo: ");
    fgets(titulo, MAX_STR, stdin);
    titulo[strcspn(titulo, "\n")] = '\0'; // Remover o '\n' do fgets
    
    printf("Autor: ");
    fgets(autor, MAX_STR, stdin);
    autor[strcspn(autor, "\n")] = '\0'; // Remover o '\n' do fgets
    
    printf("Ano: ");
    scanf("%d", &ano);
    
    printf("Numero de copias: ");
    scanf("%d", &copias);
    
    inserirLivro(isbn, titulo, autor, ano, copias);
}
