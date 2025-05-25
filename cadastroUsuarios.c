#include "biblioteca.h"
// Função para criar um novo usuário
Usuario* criarUsuario(int id, char* nome, char* email) {
    Usuario* novoUsuario = (Usuario*)malloc(sizeof(Usuario));
    if (novoUsuario == NULL) {
        printf("Erro: Memoria insuficiente!\n");
        return NULL;
    }
    
    novoUsuario->id = id;
    strcpy(novoUsuario->nome, nome);
    strcpy(novoUsuario->email, email);
    novoUsuario->prox = NULL;
    
    return novoUsuario;
}

// Função para inserir um usuário na tabela hash
int inserirUsuario(int id, char* nome, char* email) {
    // Verificar se o ID já existe
    if (buscarUsuarioPorID(id) != NULL) {
        printf("Erro: Usuario com ID %d ja cadastrado!\n", id);
        return 0;
    }
    
    // Calcular índice na tabela hash
    int indice = hashID(id);
    
    // Criar o novo usuário
    Usuario* novoUsuario = criarUsuario(id, nome, email);
    if (novoUsuario == NULL) {
        return 0;
    }
    
    // Inserir no início da lista encadeada (tratamento de colisão)
    if (tabelaUsuarios[indice] == NULL) {
        tabelaUsuarios[indice] = novoUsuario;
    } else {
        novoUsuario->prox = tabelaUsuarios[indice];
        tabelaUsuarios[indice] = novoUsuario;
    }
    
    // Salvar em arquivo
    salvarUsuariosEmArquivo();
    
    printf("Usuario cadastrado com sucesso!\n");
    return 1;
}

// Função auxiliar para buscar um usuário pelo ID
Usuario* buscarUsuarioPorID(int id) {
    int indice = hashID(id);
    Usuario* atual = tabelaUsuarios[indice];
    
    while (atual != NULL) {
        if (atual->id == id) {
            return atual; // Usuário encontrado
        }
        atual = atual->prox;
    }
    
    return NULL; // Usuário não encontrado
}

// Função para buscar um usuário pelo nome
Usuario* buscarUsuarioPorNome(char* nome) {
    // Percorrer toda a tabela hash
    for (int i = 0; i < MAX_TAM; i++) {
        Usuario* atual = tabelaUsuarios[i];
        
        // Percorrer cada lista encadeada
        while (atual != NULL) {
            if (strcmp(atual->nome, nome) == 0) {
                return atual; // Usuário encontrado
            }
            atual = atual->prox;
        }
    }
    
    return NULL; // Usuário não encontrado
}

// Função para salvar todos os usuários em arquivo
void salvarUsuariosEmArquivo() {
    FILE* arquivo = fopen("usuarios.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para gravacao!\n");
        return;
    }
    
    // Percorrer toda a tabela hash
    for (int i = 0; i < MAX_TAM; i++) {
        Usuario* atual = tabelaUsuarios[i];
        
        // Percorrer cada lista encadeada
        while (atual != NULL) {
            // Escrever o usuário no arquivo (excluindo o ponteiro 'prox')
            fwrite(atual, sizeof(Usuario) - sizeof(Usuario*), 1, arquivo);
            atual = atual->prox;
        }
    }
    
    fclose(arquivo);
}

// Função para carregar usuários do arquivo para a tabela hash
void carregarUsuariosDoArquivo() {
    FILE* arquivo = fopen("usuarios.dat", "rb");
    if (arquivo == NULL) {
        printf("Arquivo de usuarios nao encontrado. Sera criado um novo ao salvar.\n");
        return;
    }
    
    // Estrutura temporária sem o ponteiro prox
    typedef struct {
        int id;
        char nome[MAX_STR];
        char email[MAX_STR];
    } UsuarioTemp;
    
    UsuarioTemp usuarioTemp;
    
    // Ler cada usuário do arquivo
    while (fread(&usuarioTemp, sizeof(UsuarioTemp), 1, arquivo) == 1) {
        // Inserir na tabela hash (mas evitar recursão infinita com salvarUsuariosEmArquivo)
        Usuario* novoUsuario = criarUsuario(usuarioTemp.id, usuarioTemp.nome, usuarioTemp.email);
        if (novoUsuario != NULL) {
            int indice = hashID(usuarioTemp.id);
            
            if (tabelaUsuarios[indice] == NULL) {
                tabelaUsuarios[indice] = novoUsuario;
            } else {
                novoUsuario->prox = tabelaUsuarios[indice];
                tabelaUsuarios[indice] = novoUsuario;
            }
        }
    }
    
    fclose(arquivo);
}

// Função para o menu de cadastro de usuários
void menuCadastroUsuario() {
    int id;
    char nome[MAX_STR];
    char email[MAX_STR];
    
    printf("\n=== CADASTRO DE USUARIOS ===\n");
    
    printf("ID: ");
    scanf("%d", &id);
    getchar(); // Consumir o '\n' pendente
    
    printf("Nome: ");
    fgets(nome, MAX_STR, stdin);
    nome[strcspn(nome, "\n")] = '\0'; // Remover o '\n' do fgets
    
    printf("Email: ");
    fgets(email, MAX_STR, stdin);
    email[strcspn(email, "\n")] = '\0'; // Remover o '\n' do fgets
    
    inserirUsuario(id, nome, email);
}
